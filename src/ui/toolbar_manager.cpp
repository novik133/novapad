/**
 * @file toolbar_manager.cpp
 * @brief Implementation of the configurable toolbar manager.
 */

#include "toolbar_manager.h"

namespace NovaPad::UI {

ToolbarManager::ToolbarManager(QToolBar *toolbar, QObject *parent)
    : QObject(parent)
    , m_toolbar(toolbar)
{
}

void ToolbarManager::loadFromString(const QString &actionString)
{
    if (!m_toolbar) return;

    m_toolbar->clear();

    const QStringList items = actionString.split('|', Qt::SkipEmptyParts);
    for (const QString &item : items) {
        const QString trimmed = item.trimmed();
        if (trimmed.isEmpty()) {
            m_toolbar->addSeparator();
        } else {
            // Look up the action by object name in the parent window
            auto *parent = m_toolbar->parentWidget();
            if (!parent) continue;

            QAction *action = parent->findChild<QAction *>(trimmed);
            if (action) {
                m_toolbar->addAction(action);
            }
        }
    }
}

QString ToolbarManager::saveToString() const
{
    if (!m_toolbar) return {};

    QStringList items;
    for (auto *action : m_toolbar->actions()) {
        if (action->isSeparator()) {
            items.append(QString());
        } else {
            items.append(action->objectName());
        }
    }
    return items.join('|');
}

} // namespace NovaPad::UI
