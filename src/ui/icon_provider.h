/**
 * @file icon_provider.h
 * @brief Centralised icon loading with custom theme fallback support.
 *
 * Provides a consistent API for retrieving icons across the application.
 * Falls back to the bundled NovaPad icon theme when the desktop theme
 * lacks a requested icon.
 */

#ifndef NOVAPAD_UI_ICON_PROVIDER_H
#define NOVAPAD_UI_ICON_PROVIDER_H

#include <QIcon>
#include <QString>

namespace NovaPad::UI {

/**
 * @class IconProvider
 * @brief Static helper for loading icons from the theme or bundled resources.
 */
class IconProvider final
{
    Q_DISABLE_COPY_MOVE(IconProvider)
    IconProvider() = delete;

public:
    /**
     * @brief Returns a QIcon for the given theme name, falling back to the
     *        bundled SVG if the system theme does not provide one.
     * @param name  Icon name (e.g. "document-new", "edit-find").
     */
    [[nodiscard]] static QIcon icon(const QString &name);

    /**
     * @brief Registers the bundled NovaPad icon theme as a fallback.
     *
     * Should be called once during application startup.
     */
    static void installTheme();

private:
    /**
     * @brief Determines if the current theme is dark based on background luminance.
     */
    [[nodiscard]] static bool isDarkTheme();
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_ICON_PROVIDER_H
