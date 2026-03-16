/**
 * @file status_bar_manager.h
 * @brief Manages the status bar widgets and their updates.
 *
 * Displays cursor position (line/col), file encoding, EOL format,
 * language name, and insert/overwrite mode — all updated in real time
 * as the user edits or switches tabs.
 */

#ifndef NOVAPAD_UI_STATUS_BAR_MANAGER_H
#define NOVAPAD_UI_STATUS_BAR_MANAGER_H

#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QStatusBar>

namespace NovaPad::Editor { class CodeEditorWidget; }

namespace NovaPad::UI {

/**
 * @class StatusBarManager
 * @brief Creates, owns, and refreshes all status-bar widgets.
 */
class StatusBarManager : public QObject
{
    Q_OBJECT

public:
    explicit StatusBarManager(QStatusBar *statusBar, QObject *parent = nullptr);
    ~StatusBarManager() override = default;

    /** @brief Updates all widgets from the given editor's state. */
    void refresh(NovaPad::Editor::CodeEditorWidget *editor);

    /** @brief Toggles the insert/overwrite indicator. */
    void setOverwriteMode(bool overwrite);

private:
    QStatusBar  *m_statusBar      = nullptr;
    QLabel      *m_cursorLabel    = nullptr;   ///< "Ln X, Col Y"
    QPushButton *m_encodingBtn    = nullptr;   ///< "UTF-8"
    QPushButton *m_eolBtn         = nullptr;   ///< "Unix (LF)"
    QPushButton *m_languageBtn    = nullptr;   ///< "C++"
    QPushButton *m_overtypeBtn    = nullptr;   ///< "INS" / "OVR"
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_STATUS_BAR_MANAGER_H
