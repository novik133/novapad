/**
 * @file theme_manager.h
 * @brief Manages application-wide colour themes and editor colour schemes.
 *
 * The ThemeManager provides both the application palette (light/dark) and
 * the syntax-highlighting colour scheme (Monokai, Solarized, etc.).
 * Themes can be loaded from built-in resources or from user-supplied
 * JSON files in the themes directory.
 */

#ifndef NOVAPAD_THEMES_THEME_MANAGER_H
#define NOVAPAD_THEMES_THEME_MANAGER_H

#include "themes/color_scheme.h"

#include <QMap>
#include <QObject>
#include <QPalette>
#include <QString>

namespace NovaPad::Themes {

/**
 * @class ThemeManager
 * @brief Singleton that registers, selects, and applies colour themes.
 */
class ThemeManager : public QObject
{
    Q_OBJECT

public:
    [[nodiscard]] static ThemeManager &instance();

    ThemeManager(const ThemeManager &) = delete;
    ThemeManager &operator=(const ThemeManager &) = delete;

    /** @brief Loads all built-in and user-defined themes. */
    void loadThemes();

    /** @brief Loads themes from a directory of JSON files. */
    void loadThemesFromDirectory(const QString &dir);

    /** @brief Returns a list of all registered theme IDs. */
    [[nodiscard]] QStringList availableThemes() const;

    /** @brief Returns the colour scheme for the given theme ID. */
    [[nodiscard]] const ColorScheme *scheme(const QString &themeId) const;

    /** @brief Returns the currently active theme ID. */
    [[nodiscard]] QString currentThemeId() const;

    /** @brief Applies the theme with the given ID. */
    void setCurrentTheme(const QString &themeId);

    /** @brief Returns a QPalette suitable for the current theme. */
    [[nodiscard]] QPalette palette() const;

signals:
    void themeChanged(const QString &themeId);

private:
    ThemeManager();

    /** @brief Registers the built-in default themes. */
    void registerBuiltinThemes();

    QMap<QString, ColorScheme> m_themes;
    QString                    m_currentThemeId;
};

} // namespace NovaPad::Themes

#endif // NOVAPAD_THEMES_THEME_MANAGER_H
