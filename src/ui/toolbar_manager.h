/**
 * @file toolbar_manager.h
 * @brief Manages the main toolbar and its user-configurable layout.
 */

#ifndef NOVAPAD_UI_TOOLBAR_MANAGER_H
#define NOVAPAD_UI_TOOLBAR_MANAGER_H

#include <QObject>
#include <QString>
#include <QToolBar>

namespace NovaPad::UI {

class ToolbarManager : public QObject
{
    Q_OBJECT
public:
    explicit ToolbarManager(QToolBar *toolbar, QObject *parent = nullptr);
    ~ToolbarManager() override = default;

    /** @brief Rebuilds the toolbar from a pipe-delimited action string. */
    void loadFromString(const QString &actionString);

    /** @brief Serialises the current toolbar layout to a pipe-delimited string. */
    [[nodiscard]] QString saveToString() const;

private:
    QToolBar *m_toolbar = nullptr;
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_TOOLBAR_MANAGER_H
