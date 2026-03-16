/**
 * @file platform_utils.cpp
 * @brief Implementation of platform-specific utility functions.
 */

#include "platform_utils.h"

#include <QApplication>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QPalette>
#include <QProcess>
#include <QSysInfo>
#include <QUrl>

namespace NovaPad::Utils {

void PlatformUtils::showInFileManager(const QString &filePath)
{
#ifdef Q_OS_WIN
    QProcess::startDetached("explorer.exe", { "/select,", QDir::toNativeSeparators(filePath) });
#elif defined(Q_OS_MACOS)
    QProcess::startDetached("open", { "-R", filePath });
#else
    // Linux / FreeBSD — try xdg-open on the parent directory
    const QString dir = QFileInfo(filePath).absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
#endif
}

void PlatformUtils::openUrl(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}

QString PlatformUtils::defaultTerminal()
{
#ifdef Q_OS_WIN
    return QStringLiteral("cmd.exe");
#elif defined(Q_OS_MACOS)
    return QStringLiteral("/Applications/Utilities/Terminal.app");
#else
    // Try common Linux terminals in order of preference
    static const QStringList candidates = {
        "konsole", "gnome-terminal", "xfce4-terminal",
        "mate-terminal", "lxterminal", "xterm"
    };
    for (const QString &term : candidates) {
        QProcess proc;
        proc.start("which", { term });
        proc.waitForFinished(1000);
        if (proc.exitCode() == 0) {
            return term;
        }
    }
    return QStringLiteral("xterm");
#endif
}

QString PlatformUtils::defaultMonospaceFont()
{
    // Try common programmer fonts in order of preference
    static const QStringList candidates = {
        "JetBrains Mono", "Fira Code", "Source Code Pro",
        "Cascadia Code", "Ubuntu Mono", "DejaVu Sans Mono",
        "Consolas", "Menlo", "Monaco", "Courier New"
    };

    const QStringList available = QFontDatabase::families();
    for (const QString &font : candidates) {
        if (available.contains(font, Qt::CaseInsensitive)) {
            return font;
        }
    }

    // Ultimate fallback: Qt's generic monospace family
    return QFont("monospace").family();
}

bool PlatformUtils::isDarkTheme()
{
    const QPalette pal = QApplication::palette();
    const QColor bg = pal.color(QPalette::Window);
    // A simple heuristic: if the window background is darker than mid-grey,
    // the theme is considered "dark".
    return bg.lightness() < 128;
}

QString PlatformUtils::osDescription()
{
    return QStringLiteral("%1 %2 (%3)")
        .arg(QSysInfo::productType(),
             QSysInfo::productVersion(),
             QSysInfo::currentCpuArchitecture());
}

} // namespace NovaPad::Utils
