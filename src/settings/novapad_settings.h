/**
 * @file novapad_settings.h
 * @brief Centralised settings management for NovaPad.
 *
 * Provides strongly-typed accessors for all application settings, organised
 * into logical groups (General, Appearance, Search, Languages, MainWindow,
 * Shortcuts, Extensions, Run).  Backed by QSettings (INI format).
 *
 * This is the C++23 equivalent of Notepadqq's NqqSettings with its
 * DECLARE_SETTING macros — here replaced by explicit getter/setter pairs
 * for better IDE support and type safety.
 */

#ifndef NOVAPAD_SETTINGS_NOVAPAD_SETTINGS_H
#define NOVAPAD_SETTINGS_NOVAPAD_SETTINGS_H

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariant>

namespace NovaPad::Settings {

// ---------------------------------------------------------------------------
// Setting group classes — each encapsulates a QSettings group
// ---------------------------------------------------------------------------

/**
 * @class GeneralSettings
 * @brief Settings under the [General] group.
 */
class GeneralSettings
{
public:
    explicit GeneralSettings(QSettings &s) : m_s(s) {}

    // -- Version tracking ----------------------------------------------------
    [[nodiscard]] QString novaPadVersion() const         { return get("novaPadVersion").toString(); }
    void setNovaPadVersion(const QString &v)             { set("novaPadVersion", v); }

    // -- Locale --------------------------------------------------------------
    [[nodiscard]] QString localization() const            { return get("localization").toString(); }
    void setLocalization(const QString &v)                { set("localization", v); }

    // -- Behaviour -----------------------------------------------------------
    [[nodiscard]] bool warnForDifferentIndentation() const { return get("warnForDifferentIndentation", true).toBool(); }
    void setWarnForDifferentIndentation(bool v)            { set("warnForDifferentIndentation", v); }

    [[nodiscard]] bool exitOnLastTabClose() const        { return get("exitOnLastTabClose", false).toBool(); }
    void setExitOnLastTabClose(bool v)                   { set("exitOnLastTabClose", v); }

    [[nodiscard]] bool rememberTabsOnExit() const        { return get("rememberTabsOnExit", true).toBool(); }
    void setRememberTabsOnExit(bool v)                   { set("rememberTabsOnExit", v); }

    [[nodiscard]] bool smartIndentation() const          { return get("smartIndentation", true).toBool(); }
    void setSmartIndentation(bool v)                     { set("smartIndentation", v); }

    [[nodiscard]] bool mathRendering() const             { return get("mathRendering", false).toBool(); }
    void setMathRendering(bool v)                        { set("mathRendering", v); }

    [[nodiscard]] int autosaveInterval() const           { return get("autosaveInterval", 30).toInt(); }
    void setAutosaveInterval(int v)                      { set("autosaveInterval", v); }

    [[nodiscard]] int warnIfFileLargerThan() const       { return get("warnIfFileLargerThan", 10).toInt(); }
    void setWarnIfFileLargerThan(int v)                  { set("warnIfFileLargerThan", v); }

    // -- Recent documents ----------------------------------------------------
    [[nodiscard]] QStringList recentDocuments() const    { return get("recentDocuments").toStringList(); }
    void setRecentDocuments(const QStringList &v)        { set("recentDocuments", v); }

    // -- Last selected directory ---------------------------------------------
    [[nodiscard]] QString lastSelectedDir() const        { return get("lastSelectedDir").toString(); }
    void setLastSelectedDir(const QString &v)            { set("lastSelectedDir", v); }

private:
    [[nodiscard]] QVariant get(const QString &key, const QVariant &def = {}) const
    { m_s.beginGroup("General"); auto v = m_s.value(key, def); m_s.endGroup(); return v; }
    void set(const QString &key, const QVariant &val)
    { m_s.beginGroup("General"); m_s.setValue(key, val); m_s.endGroup(); }
    QSettings &m_s;
};

/**
 * @class AppearanceSettings
 * @brief Settings under the [Appearance] group.
 */
class AppearanceSettings
{
public:
    explicit AppearanceSettings(QSettings &s) : m_s(s) {}

    [[nodiscard]] QString colorScheme() const            { return get("colorScheme", "default").toString(); }
    void setColorScheme(const QString &v)                { set("colorScheme", v); }

    [[nodiscard]] QString fontFamily() const             { return get("fontFamily", "Monospace").toString(); }
    void setFontFamily(const QString &v)                 { set("fontFamily", v); }

    [[nodiscard]] int fontSize() const                   { return get("fontSize", 12).toInt(); }
    void setFontSize(int v)                              { set("fontSize", v); }

    [[nodiscard]] bool showLineNumbers() const           { return get("showLineNumbers", true).toBool(); }
    void setShowLineNumbers(bool v)                      { set("showLineNumbers", v); }

    [[nodiscard]] bool showMinimap() const               { return get("showMinimap", false).toBool(); }
    void setShowMinimap(bool v)                          { set("showMinimap", v); }

    [[nodiscard]] int tabSize() const                    { return get("tabSize", 4).toInt(); }
    void setTabSize(int v)                               { set("tabSize", v); }

    [[nodiscard]] bool indentWithSpaces() const          { return get("indentWithSpaces", true).toBool(); }
    void setIndentWithSpaces(bool v)                     { set("indentWithSpaces", v); }

private:
    [[nodiscard]] QVariant get(const QString &key, const QVariant &def = {}) const
    { m_s.beginGroup("Appearance"); auto v = m_s.value(key, def); m_s.endGroup(); return v; }
    void set(const QString &key, const QVariant &val)
    { m_s.beginGroup("Appearance"); m_s.setValue(key, val); m_s.endGroup(); }
    QSettings &m_s;
};

/**
 * @class SearchSettings
 * @brief Settings under the [Search] group.
 */
class SearchSettings
{
public:
    explicit SearchSettings(QSettings &s) : m_s(s) {}

    [[nodiscard]] bool searchAsIType() const             { return get("searchAsIType", true).toBool(); }
    void setSearchAsIType(bool v)                        { set("searchAsIType", v); }

    [[nodiscard]] bool saveHistory() const               { return get("saveHistory", true).toBool(); }
    void setSaveHistory(bool v)                          { set("saveHistory", v); }

    [[nodiscard]] QStringList searchHistory() const      { return get("searchHistory").toStringList(); }
    void setSearchHistory(const QStringList &v)          { set("searchHistory", v); }

    [[nodiscard]] QStringList replaceHistory() const     { return get("replaceHistory").toStringList(); }
    void setReplaceHistory(const QStringList &v)         { set("replaceHistory", v); }

private:
    [[nodiscard]] QVariant get(const QString &key, const QVariant &def = {}) const
    { m_s.beginGroup("Search"); auto v = m_s.value(key, def); m_s.endGroup(); return v; }
    void set(const QString &key, const QVariant &val)
    { m_s.beginGroup("Search"); m_s.setValue(key, val); m_s.endGroup(); }
    QSettings &m_s;
};

/**
 * @class LanguageSpecificSettings
 * @brief Per-language settings under the [Languages/<langId>] group.
 */
class LanguageSpecificSettings
{
public:
    explicit LanguageSpecificSettings(QSettings &s) : m_s(s) {}

    [[nodiscard]] bool hasUseDefaultSettings(const QString &langId) const
    { return m_s.contains(group(langId) + "/useDefaultSettings"); }

    [[nodiscard]] bool useDefaultSettings(const QString &langId) const
    { return get(langId, "useDefaultSettings", true).toBool(); }
    void setUseDefaultSettings(const QString &langId, bool v)
    { set(langId, "useDefaultSettings", v); }

    [[nodiscard]] int tabSize(const QString &langId) const
    { return get(langId, "tabSize", 4).toInt(); }
    void setTabSize(const QString &langId, int v)
    { set(langId, "tabSize", v); }

    [[nodiscard]] bool indentWithSpaces(const QString &langId) const
    { return get(langId, "indentWithSpaces", true).toBool(); }
    void setIndentWithSpaces(const QString &langId, bool v)
    { set(langId, "indentWithSpaces", v); }

private:
    [[nodiscard]] static QString group(const QString &langId)
    { return QStringLiteral("Languages/%1").arg(langId); }
    [[nodiscard]] QVariant get(const QString &langId, const QString &key, const QVariant &def = {}) const
    { return m_s.value(group(langId) + "/" + key, def); }
    void set(const QString &langId, const QString &key, const QVariant &val)
    { m_s.setValue(group(langId) + "/" + key, val); }
    QSettings &m_s;
};

/**
 * @class MainWindowSettings
 * @brief Settings under the [MainWindow] group (geometry, state, toolbar).
 */
class MainWindowSettings
{
public:
    explicit MainWindowSettings(QSettings &s) : m_s(s) {}

    [[nodiscard]] QByteArray geometry() const             { return get("geometry").toByteArray(); }
    void setGeometry(const QByteArray &v)                 { set("geometry", v); }

    [[nodiscard]] QByteArray windowState() const          { return get("windowState").toByteArray(); }
    void setWindowState(const QByteArray &v)              { set("windowState", v); }

    [[nodiscard]] QString toolBarItems() const            { return get("toolBarItems").toString(); }
    void setToolBarItems(const QString &v)                { set("toolBarItems", v); }

    [[nodiscard]] bool showMenuBar() const                { return get("showMenuBar", true).toBool(); }
    void setShowMenuBar(bool v)                           { set("showMenuBar", v); }

    [[nodiscard]] bool showToolBar() const                { return get("showToolBar", true).toBool(); }
    void setShowToolBar(bool v)                           { set("showToolBar", v); }

private:
    [[nodiscard]] QVariant get(const QString &key, const QVariant &def = {}) const
    { m_s.beginGroup("MainWindow"); auto v = m_s.value(key, def); m_s.endGroup(); return v; }
    void set(const QString &key, const QVariant &val)
    { m_s.beginGroup("MainWindow"); m_s.setValue(key, val); m_s.endGroup(); }
    QSettings &m_s;
};

// ---------------------------------------------------------------------------
// Top-level settings facade
// ---------------------------------------------------------------------------

/**
 * @class NovaPadSettings
 * @brief Singleton facade providing access to all setting groups.
 */
class NovaPadSettings final
{
public:
    /** @brief Returns the singleton instance. */
    [[nodiscard]] static NovaPadSettings &instance();

    NovaPadSettings(const NovaPadSettings &) = delete;
    NovaPadSettings &operator=(const NovaPadSettings &) = delete;

    // -- Group accessors -----------------------------------------------------
    [[nodiscard]] GeneralSettings          &general()    { return m_general; }
    [[nodiscard]] AppearanceSettings       &appearance() { return m_appearance; }
    [[nodiscard]] SearchSettings           &search()     { return m_search; }
    [[nodiscard]] LanguageSpecificSettings &languages()  { return m_languages; }
    [[nodiscard]] MainWindowSettings       &mainWindow() { return m_mainWindow; }

    /** @brief Flushes all pending changes to disk immediately. */
    void sync() { m_settings.sync(); }

    /** @brief Returns the underlying QSettings (for advanced use). */
    [[nodiscard]] QSettings &raw() { return m_settings; }

private:
    NovaPadSettings();

    QSettings                 m_settings;
    GeneralSettings           m_general;
    AppearanceSettings        m_appearance;
    SearchSettings            m_search;
    LanguageSpecificSettings  m_languages;
    MainWindowSettings        m_mainWindow;
};

} // namespace NovaPad::Settings

#endif // NOVAPAD_SETTINGS_NOVAPAD_SETTINGS_H
