/**
 * @file shortcut_manager.cpp
 * @brief Implementation of the keyboard shortcut manager.
 *
 * Shortcuts are stored under the [Shortcuts] group in QSettings.  Each
 * entry maps an action's objectName() to its key sequence string.  On
 * load, the manager iterates all registered QActions and overrides their
 * default shortcuts with any user-customised values.
 */

#include "shortcut_manager.h"
#include "settings/novapad_settings.h"

#include <QSettings>

namespace NovaPad::Settings {

// ---------------------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------------------

ShortcutManager &ShortcutManager::instance()
{
    static ShortcutManager inst;
    return inst;
}

ShortcutManager::ShortcutManager()
    : QObject(nullptr)
{
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ShortcutManager::loadShortcuts(const QList<QAction *> &actions)
{
    auto &settings = NovaPadSettings::instance().raw();
    settings.beginGroup("Shortcuts");

    for (auto *action : actions) {
        const QString name = action->objectName();
        if (name.isEmpty()) continue;

        if (settings.contains(name)) {
            const QKeySequence seq(settings.value(name).toString());
            action->setShortcut(seq);
            m_shortcuts[name] = seq;
        } else {
            // Store the default shortcut in our map
            m_shortcuts[name] = action->shortcut();
        }
    }

    settings.endGroup();
}

void ShortcutManager::saveShortcuts(const QList<QAction *> &actions)
{
    auto &settings = NovaPadSettings::instance().raw();
    settings.beginGroup("Shortcuts");

    for (auto *action : actions) {
        const QString name = action->objectName();
        if (name.isEmpty()) continue;
        settings.setValue(name, action->shortcut().toString());
    }

    settings.endGroup();
    settings.sync();
}

QKeySequence ShortcutManager::shortcut(const QString &actionName) const
{
    return m_shortcuts.value(actionName);
}

void ShortcutManager::setShortcut(const QString &actionName, const QKeySequence &seq)
{
    m_shortcuts[actionName] = seq;
}

void ShortcutManager::resetToDefaults(const QList<QAction *> &actions)
{
    auto &settings = NovaPadSettings::instance().raw();
    settings.beginGroup("Shortcuts");
    settings.remove(""); // Remove all entries in the group
    settings.endGroup();

    // Clear our runtime map — the next loadShortcuts() will re-read defaults
    m_shortcuts.clear();

    // Reload default shortcuts from the action objects
    for (auto *action : actions) {
        const QString name = action->objectName();
        if (!name.isEmpty()) {
            m_shortcuts[name] = action->shortcut();
        }
    }
}

const QMap<QString, QKeySequence> &ShortcutManager::shortcutMap() const
{
    return m_shortcuts;
}

} // namespace NovaPad::Settings
