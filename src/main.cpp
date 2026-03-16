/**
 * @file main.cpp
 * @brief Entry point for the NovaPad application.
 *
 * Initializes the Qt application, handles single-instance enforcement,
 * restores previous sessions, processes command-line arguments, and
 * launches the main window.
 *
 * This is the C++23 / Qt 6.10 rewrite of the original Notepadqq project.
 */

#include "core/application.h"
#include "core/command_line_parser.h"
#include "core/globals.h"
#include "core/single_instance.h"
#include "editor/language_manager.h"
#include "extensions/extension_loader.h"
#include "sessions/backup_service.h"
#include "sessions/persistent_cache.h"
#include "sessions/session_manager.h"
#include "settings/novapad_settings.h"
#include "themes/theme_manager.h"
#include "ui/main_window.h"

#include <novapad_version.h>

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QTimer>
#include <QTranslator>

#ifdef NOVAPAD_DEBUG
#include <QElapsedTimer>
#endif

#include <cstdlib>

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

/**
 * @brief Applies sensible default indentation settings for well-known languages.
 *
 * For example, Makefiles always require hard tabs, and Ruby conventionally
 * uses 2-space indentation.  These defaults are written only on first run
 * so that user overrides are never clobbered.
 */
static void applyDefaultLanguageSettings()
{
    auto &settings = NovaPad::Settings::NovaPadSettings::instance();

    // Makefiles: always use hard tabs
    if (!settings.languages().hasUseDefaultSettings("makefile")) {
        settings.languages().setUseDefaultSettings("makefile", false);
        settings.languages().setIndentWithSpaces("makefile", false);
    }

    // Ruby: 2-space soft tabs by convention
    if (!settings.languages().hasUseDefaultSettings("ruby")) {
        settings.languages().setUseDefaultSettings("ruby", false);
        settings.languages().setTabSize("ruby", 2);
        settings.languages().setIndentWithSpaces("ruby", true);
    }

    // Python: 4-space soft tabs (PEP 8)
    if (!settings.languages().hasUseDefaultSettings("python")) {
        settings.languages().setUseDefaultSettings("python", false);
        settings.languages().setTabSize("python", 4);
        settings.languages().setIndentWithSpaces("python", true);
    }

    // Rust: 4-space soft tabs
    if (!settings.languages().hasUseDefaultSettings("rust")) {
        settings.languages().setUseDefaultSettings("rust", false);
        settings.languages().setTabSize("rust", 4);
        settings.languages().setIndentWithSpaces("rust", true);
    }
}

/**
 * @brief Application entry point.
 *
 * The startup sequence is:
 *  1. Create QApplication (with single-instance guard)
 *  2. Parse CLI arguments for early-exit flags (-v, -h, --print-debug-info)
 *  3. Load user settings and translations
 *  4. Optionally restore a previous session from autosave backup
 *  5. Open the main window
 *  6. Start the extension runtime (if available)
 *  7. Enter the Qt event loop
 */
int main(int argc, char *argv[])
{
#ifdef NOVAPAD_DEBUG
    QElapsedTimer startupTimer;
    startupTimer.start();
    qDebug() << "[NovaPad] DEBUG build — startup benchmark started.";
#endif

    // -----------------------------------------------------------------------
    // 1. Instantiate the application with single-instance support
    // -----------------------------------------------------------------------
    NovaPad::Core::SingleInstance app(argc, argv);

    QCoreApplication::setOrganizationName(NOVAPAD_ORG_NAME);
    QCoreApplication::setApplicationName(NOVAPAD_APP_NAME);
    QCoreApplication::setApplicationVersion(NOVAPAD_VERSION_STRING);
    QGuiApplication::setDesktopFileName("novapad");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    // -----------------------------------------------------------------------
    // 2. Parse command-line arguments (early-exit flags first)
    // -----------------------------------------------------------------------
    auto parser = NovaPad::Core::CommandLineParser::create(QApplication::arguments());

    if (parser->isSet("print-debug-info")) {
        NovaPad::Core::Globals::printEnvironmentInfo();
        return EXIT_SUCCESS;
    }

#ifdef Q_OS_UNIX
    // Discourage running as root — privilege escalation is handled per-operation
    if (getuid() == 0 && !parser->isSet("allow-root")) {
        qWarning() << QObject::tr(
            "NovaPad will request elevated privileges when needed. "
            "Running as root is not recommended. "
            "Use --allow-root to override.");
        return EXIT_SUCCESS;
    }
#endif

    // -----------------------------------------------------------------------
    // 3. Try to attach to an already-running instance
    // -----------------------------------------------------------------------
    if (app.attachToOtherInstance()) {
        return EXIT_SUCCESS;
    }
    app.startServer();

    // -----------------------------------------------------------------------
    // 4. Load settings, translations, and theme
    // -----------------------------------------------------------------------
    auto &settings = NovaPad::Settings::NovaPadSettings::instance();
    settings.general().setNovaPadVersion(NOVAPAD_VERSION_STRING);
    applyDefaultLanguageSettings();

    // Locale / translations
    QTranslator translator;
    QString langCode = settings.general().localization();
    if (langCode.isEmpty()) {
        const QLocale locale;
        langCode = (locale.name().size() >= 2) ? locale.name().left(2) : QStringLiteral("en");
        settings.general().setLocalization(langCode);
    }

    if (translator.load(QLocale(langCode),
                        QCoreApplication::applicationName().toLower(),
                        QStringLiteral("_"),
                        QStringLiteral(":/translations"))) {
        app.installTranslator(&translator);
    } else {
        settings.general().setLocalization(QStringLiteral("en"));
    }

    // Load the colour / editor theme
    NovaPad::Themes::ThemeManager::instance().loadThemes();

    // -----------------------------------------------------------------------
    // 5. Extension runtime (load early so extensions can hook into window creation)
    // -----------------------------------------------------------------------
#ifdef NOVAPAD_PLUGINS_ENABLED
    if (NovaPad::Extensions::ExtensionLoader::isRuntimePresent()) {
        NovaPad::Extensions::ExtensionLoader::startServer();
        NovaPad::Extensions::ExtensionLoader::loadAll(
            NovaPad::Core::Globals::extensionsPath());
    }
#endif

    // -----------------------------------------------------------------------
    // 6. Session restoration or fresh window
    // -----------------------------------------------------------------------
    const bool autosaveActive = settings.general().autosaveInterval() > 0;
    const bool needsRestore   = autosaveActive
                                && NovaPad::Sessions::BackupService::detectImproperShutdown();

    if (needsRestore) {
        if (NovaPad::Sessions::BackupService::restoreFromBackup()) {
            // Forward any CLI-provided file URLs to the restored window
            auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
            if (win) {
                win->openCommandLineUrls(QDir::currentPath(), QApplication::arguments());
            }
        }
    }

    // If no window was created by the restoration path, open a fresh one
    if (NovaPad::UI::MainWindow::instances().isEmpty()) {
        auto *mainWin = new NovaPad::UI::MainWindow(QStringList(), nullptr);

        if (settings.general().rememberTabsOnExit()) {
            NovaPad::Sessions::SessionManager::loadSession(
                mainWin->docEngine(),
                mainWin->topEditorContainer(),
                NovaPad::Sessions::PersistentCache::sessionCachePath());
        }

        mainWin->openCommandLineUrls(QDir::currentPath(), QApplication::arguments());
        mainWin->show();
    }

    // -----------------------------------------------------------------------
    // 7. Start the autosave timer
    // -----------------------------------------------------------------------
    if (autosaveActive) {
        NovaPad::Sessions::BackupService::enableAutosave(
            settings.general().autosaveInterval());
    }

    // Handle arguments forwarded from other instances
    QObject::connect(&app, &NovaPad::Core::SingleInstance::receivedArguments,
                     &app, [](const QString &workingDir, const QStringList &args) {
        auto p = NovaPad::Core::CommandLineParser::create(args);
        if (p->isSet("new-window")) {
            auto *win = new NovaPad::UI::MainWindow(workingDir, args, nullptr);
            win->show();
        } else {
            auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
            if (win) {
                win->openCommandLineUrls(workingDir, args);
                win->activateWindow();
                win->raise();
            }
        }
    });

#ifdef NOVAPAD_DEBUG
    const qint64 elapsedMs = startupTimer.elapsed();
    qDebug() << QStringLiteral("[NovaPad] Started in %1 ms").arg(elapsedMs);
#endif

    // -----------------------------------------------------------------------
    // 8. Enter the event loop
    // -----------------------------------------------------------------------
    const int exitCode = app.exec();

    // Cleanup on normal shutdown
    NovaPad::Sessions::BackupService::clearBackupData();
    return exitCode;
}
