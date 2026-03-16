/**
 * @file icon_provider.cpp
 * @brief Implementation of the centralised icon loading service.
 */

#include "icon_provider.h"

#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QPalette>

namespace NovaPad::UI {

bool IconProvider::isDarkTheme()
{
    // Determine if we're in dark theme by checking background luminance
    const QColor bgColor = QApplication::palette().color(QPalette::Window);
    const int luminance = (0.299 * bgColor.red() + 0.587 * bgColor.green() + 0.114 * bgColor.blue());
    return luminance < 128;
}

QIcon IconProvider::icon(const QString &name)
{
    // Try the system theme first, then fall back to bundled resources
    if (QIcon::hasThemeIcon(name)) {
        return QIcon::fromTheme(name);
    }
    
    // Select white icons for dark theme, default (dark) for light theme
    const QString subdir = isDarkTheme() ? QStringLiteral("white") : QStringLiteral("");
    const QString resourcePath = subdir.isEmpty()
        ? QStringLiteral(":/icons/novapad/scalable/%1.svg").arg(name)
        : QStringLiteral(":/icons/novapad/scalable/%1/%2.svg").arg(subdir, name);
    
    return QIcon(resourcePath);
}

void IconProvider::installTheme()
{
    // Register our bundled icon directory as a fallback theme search path
    QIcon::setFallbackSearchPaths(
        QIcon::fallbackSearchPaths() << QStringLiteral(":/icons"));
    QIcon::setFallbackThemeName(QStringLiteral("novapad"));
}

} // namespace NovaPad::UI
