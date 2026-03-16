/**
 * @file shortcut_manager.h
 * @brief Manages user-configurable keyboard shortcuts.
 *
 * Stores per-action key sequences in QSettings and applies them to the
 * corresponding QActions at startup.  The PreferencesDialog's Shortcuts
 * page uses this class to display and edit the shortcut map.
 */

#ifndef NOVAPAD_SETTINGS_SHORTCUT_MANAGER_H
#define NOVAPAD_SETTINGS_SHORTCUT_MANAGER_H

#include <QAction>
#include <QKeySequence>
#include <QMap>
#include <QObject>
#include <QString>

namespace NovaPad::Settings {

/**
 * @class ShortcutManager
 * @brief Singleton that maps action object names to key sequences.
 */
class ShortcutManager : public QObject
{
    Q_OBJECT

public:
    [[nodiscard]] static ShortcutManager &instance();

    ShortcutManager(const ShortcutManager &) = delete;
    ShortcutManager &operator=(const ShortcutManager &) = delete;

    /** @brief Loads saved shortcuts from QSettings and applies them. */
    void loadShortcuts(const QList<QAction *> &actions);

    /** @brief Saves current shortcuts to QSettings. */
    void saveShortcuts(const QList<QAction *> &actions);

    /** @brief Returns the shortcut for the given action name. */
    [[nodiscard]] QKeySequence shortcut(const QString &actionName) const;

    /** @brief Sets (overrides) the shortcut for the given action name. */
    void setShortcut(const QString &actionName, const QKeySequence &seq);

    /** @brief Resets all shortcuts to their defaults. */
    void resetToDefaults(const QList<QAction *> &actions);

    /** @brief Returns the full shortcut map (action name → key sequence). */
    [[nodiscard]] const QMap<QString, QKeySequence> &shortcutMap() const;

private:
    ShortcutManager();

    QMap<QString, QKeySequence> m_shortcuts;
};

} // namespace NovaPad::Settings

#endif // NOVAPAD_SETTINGS_SHORTCUT_MANAGER_H
