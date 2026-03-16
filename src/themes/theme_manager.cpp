/**
 * @file theme_manager.cpp
 * @brief Implementation of the theme management singleton.
 *
 * Built-in themes are registered at construction time.  User themes are
 * loaded from JSON files in the user's themes directory.  Each JSON file
 * contains a ColorScheme definition with named colour values.
 */

#include "theme_manager.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

namespace NovaPad::Themes {

// ---------------------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------------------

ThemeManager &ThemeManager::instance()
{
    static ThemeManager inst;
    return inst;
}

ThemeManager::ThemeManager()
    : QObject(nullptr)
    , m_currentThemeId("default")
{
    registerBuiltinThemes();
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ThemeManager::loadThemes()
{
    registerBuiltinThemes();
    // User themes would be loaded from Core::Globals::appDataPath("themes")
}

void ThemeManager::loadThemesFromDirectory(const QString &dir)
{
    QDir themeDir(dir);
    const QStringList jsonFiles = themeDir.entryList({ "*.json" }, QDir::Files);

    for (const QString &file : jsonFiles) {
        QFile f(themeDir.filePath(file));
        if (!f.open(QIODevice::ReadOnly)) continue;

        QJsonParseError err;
        const QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &err);
        if (doc.isNull()) continue;

        ColorScheme scheme = ColorScheme::fromJson(doc.object());
        if (!scheme.id.isEmpty()) {
            m_themes[scheme.id] = scheme;
        }
    }
}

QStringList ThemeManager::availableThemes() const
{
    return m_themes.keys();
}

const ColorScheme *ThemeManager::scheme(const QString &themeId) const
{
    auto it = m_themes.constFind(themeId);
    return (it != m_themes.constEnd()) ? &it.value() : nullptr;
}

QString ThemeManager::currentThemeId() const
{
    return m_currentThemeId;
}

void ThemeManager::setCurrentTheme(const QString &themeId)
{
    if (!m_themes.contains(themeId)) return;

    m_currentThemeId = themeId;
    
    // Apply the theme palette to the application
    QApplication::setPalette(palette());
    
    emit themeChanged(themeId);
}

QPalette ThemeManager::palette() const
{
    const ColorScheme *cs = scheme(m_currentThemeId);
    if (!cs) return QApplication::palette();

    QPalette pal;
    pal.setColor(QPalette::Window,          cs->background);
    pal.setColor(QPalette::WindowText,      cs->foreground);
    pal.setColor(QPalette::Base,            cs->editorBackground);
    pal.setColor(QPalette::Text,            cs->editorForeground);
    pal.setColor(QPalette::Highlight,       cs->selectionBackground);
    pal.setColor(QPalette::HighlightedText, cs->selectionForeground);
    pal.setColor(QPalette::AlternateBase,   cs->lineHighlight);
    return pal;
}

// ---------------------------------------------------------------------------
// Built-in themes
// ---------------------------------------------------------------------------

void ThemeManager::registerBuiltinThemes()
{
    if (m_themes.contains("default")) return; // Already registered

    // -- Default (Light) theme -----------------------------------------------
    ColorScheme light;
    light.id                  = "default";
    light.name                = "Default Light";
    light.background          = QColor("#F5F5F5");
    light.foreground          = QColor("#333333");
    light.editorBackground    = QColor("#FFFFFF");
    light.editorForeground    = QColor("#333333");
    light.lineHighlight       = QColor("#FFFDE7");
    light.selectionBackground = QColor("#B3D4FC");
    light.selectionForeground = QColor("#000000");
    light.gutterBackground    = QColor("#F0F0F0");
    light.gutterForeground    = QColor("#999999");
    light.keyword             = QColor("#0000FF");
    light.string              = QColor("#A31515");
    light.comment             = QColor("#008000");
    light.number              = QColor("#098658");
    light.function            = QColor("#795E26");
    light.type                = QColor("#267F99");
    light.preprocessor        = QColor("#AF00DB");
    light.operator_           = QColor("#000000");
    m_themes["default"] = light;

    // -- Dark theme ----------------------------------------------------------
    ColorScheme dark;
    dark.id                  = "dark";
    dark.name                = "Dark";
    dark.background          = QColor("#1E1E1E");
    dark.foreground          = QColor("#D4D4D4");
    dark.editorBackground    = QColor("#1E1E1E");
    dark.editorForeground    = QColor("#D4D4D4");
    dark.lineHighlight       = QColor("#2A2D2E");
    dark.selectionBackground = QColor("#264F78");
    dark.selectionForeground = QColor("#FFFFFF");
    dark.gutterBackground    = QColor("#1E1E1E");
    dark.gutterForeground    = QColor("#858585");
    dark.keyword             = QColor("#569CD6");
    dark.string              = QColor("#CE9178");
    dark.comment             = QColor("#6A9955");
    dark.number              = QColor("#B5CEA8");
    dark.function            = QColor("#DCDCAA");
    dark.type                = QColor("#4EC9B0");
    dark.preprocessor        = QColor("#C586C0");
    dark.operator_           = QColor("#D4D4D4");
    m_themes["dark"] = dark;

    // -- Monokai theme -------------------------------------------------------
    ColorScheme monokai;
    monokai.id                  = "monokai";
    monokai.name                = "Monokai";
    monokai.background          = QColor("#272822");
    monokai.foreground          = QColor("#F8F8F2");
    monokai.editorBackground    = QColor("#272822");
    monokai.editorForeground    = QColor("#F8F8F2");
    monokai.lineHighlight       = QColor("#3E3D32");
    monokai.selectionBackground = QColor("#49483E");
    monokai.selectionForeground = QColor("#F8F8F2");
    monokai.gutterBackground    = QColor("#272822");
    monokai.gutterForeground    = QColor("#75715E");
    monokai.keyword             = QColor("#F92672");
    monokai.string              = QColor("#E6DB74");
    monokai.comment             = QColor("#75715E");
    monokai.number              = QColor("#AE81FF");
    monokai.function            = QColor("#A6E22E");
    monokai.type                = QColor("#66D9EF");
    monokai.preprocessor        = QColor("#F92672");
    monokai.operator_           = QColor("#F92672");
    m_themes["monokai"] = monokai;

    // -- Solarized Light theme -----------------------------------------------
    ColorScheme solLight;
    solLight.id                  = "solarized-light";
    solLight.name                = "Solarized Light";
    solLight.background          = QColor("#FDF6E3");
    solLight.foreground          = QColor("#657B83");
    solLight.editorBackground    = QColor("#FDF6E3");
    solLight.editorForeground    = QColor("#657B83");
    solLight.lineHighlight       = QColor("#EEE8D5");
    solLight.selectionBackground = QColor("#EEE8D5");
    solLight.selectionForeground = QColor("#586E75");
    solLight.gutterBackground    = QColor("#EEE8D5");
    solLight.gutterForeground    = QColor("#93A1A1");
    solLight.keyword             = QColor("#859900");
    solLight.string              = QColor("#2AA198");
    solLight.comment             = QColor("#93A1A1");
    solLight.number              = QColor("#D33682");
    solLight.function            = QColor("#268BD2");
    solLight.type                = QColor("#B58900");
    solLight.preprocessor        = QColor("#CB4B16");
    solLight.operator_           = QColor("#657B83");
    m_themes["solarized-light"] = solLight;

    // -- Solarized Dark theme ------------------------------------------------
    ColorScheme solDark;
    solDark.id                  = "solarized-dark";
    solDark.name                = "Solarized Dark";
    solDark.background          = QColor("#002B36");
    solDark.foreground          = QColor("#839496");
    solDark.editorBackground    = QColor("#002B36");
    solDark.editorForeground    = QColor("#839496");
    solDark.lineHighlight       = QColor("#073642");
    solDark.selectionBackground = QColor("#073642");
    solDark.selectionForeground = QColor("#93A1A1");
    solDark.gutterBackground    = QColor("#073642");
    solDark.gutterForeground    = QColor("#586E75");
    solDark.keyword             = QColor("#859900");
    solDark.string              = QColor("#2AA198");
    solDark.comment             = QColor("#586E75");
    solDark.number              = QColor("#D33682");
    solDark.function            = QColor("#268BD2");
    solDark.type                = QColor("#B58900");
    solDark.preprocessor        = QColor("#CB4B16");
    solDark.operator_           = QColor("#839496");
    m_themes["solarized-dark"] = solDark;
}

} // namespace NovaPad::Themes
