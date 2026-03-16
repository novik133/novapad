/**
 * @file globals.cpp
 * @brief Implementation of application-wide path helpers and diagnostics.
 *
 * All platform-specific directory resolution is handled here so the rest
 * of the codebase can remain platform-agnostic.
 */

#include "globals.h"

#include <novapad_version.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QStandardPaths>
#include <QSysInfo>

namespace NovaPad::Core {

// ---------------------------------------------------------------------------
// Path helpers
// ---------------------------------------------------------------------------

QString Globals::appDataPath(const QString &fileName)
{
    // Use QStandardPaths to resolve a writable location per platform:
    //   Linux  : ~/.local/share/NovaPad/
    //   macOS  : ~/Library/Application Support/NovaPad/
    //   Windows: %APPDATA%/NovaPad/
    const QString base =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(base);
    if (!dir.exists()) {
        dir.mkpath(QStringLiteral("."));
    }

    return fileName.isEmpty() ? base : dir.filePath(fileName);
}

QString Globals::editorPath()
{
    // The embedded editor HTML lives next to the binary in a known subdir.
    // During development it can also be resolved from the source tree.
    const QString appDir = QCoreApplication::applicationDirPath();

    // Installed layout: <prefix>/share/novapad/editor/index.html
    const QString installed =
        QDir(appDir).filePath(QStringLiteral("../share/novapad/editor/index.html"));
    if (QFileInfo::exists(installed)) {
        return QFileInfo(installed).canonicalFilePath();
    }

    // Development layout: alongside the binary
    const QString devPath =
        QDir(appDir).filePath(QStringLiteral("editor/index.html"));
    return QFileInfo(devPath).absoluteFilePath();
}

QString Globals::extensionsPath()
{
    return appDataPath(QStringLiteral("extensions"));
}

QString Globals::extensionToolsPath()
{
    const QString appDir = QCoreApplication::applicationDirPath();
    return QDir(appDir).filePath(QStringLiteral("extension_tools"));
}

QString Globals::fileNameFromUrl(const QUrl &url)
{
    if (url.isLocalFile()) {
        return QFileInfo(url.toLocalFile()).fileName();
    }
    return url.toString();
}

QString Globals::copyrightNotice()
{
    return QStringLiteral("Copyright 2025-2026 Kamil 'Novik' Nowicki");
}

// ---------------------------------------------------------------------------
// Diagnostics
// ---------------------------------------------------------------------------

void Globals::printEnvironmentInfo()
{
    const auto writeLine = [](const QString &key, const QString &value) {
        qInfo().noquote() << QStringLiteral("  %1: %2").arg(key, -25).arg(value);
    };

    qInfo().noquote() << QStringLiteral("NovaPad %1 — environment info").arg(kVersion);
    qInfo().noquote() << QStringLiteral("----------------------------------------------");

    writeLine(QStringLiteral("Qt version (build)"),  QStringLiteral(QT_VERSION_STR));
    writeLine(QStringLiteral("Qt version (runtime)"), QString::fromLatin1(qVersion()));
    writeLine(QStringLiteral("OS"),
              QSysInfo::prettyProductName() + " (" + QSysInfo::currentCpuArchitecture() + ")");
    writeLine(QStringLiteral("Kernel"),               QSysInfo::kernelVersion());
    writeLine(QStringLiteral("Locale"),               QLocale::system().name());
    writeLine(QStringLiteral("App data path"),        appDataPath());
    writeLine(QStringLiteral("Editor path"),          editorPath());
    writeLine(QStringLiteral("Extensions path"),      extensionsPath());
}

} // namespace NovaPad::Core
