/**
 * @file run_command_dialog.h
 * @brief Dialog for entering an external command to execute.
 *
 * Allows the user to type an arbitrary shell command, select a working
 * directory, and optionally capture stdout into the output dock.
 */

#ifndef NOVAPAD_UI_DIALOGS_RUN_COMMAND_DIALOG_H
#define NOVAPAD_UI_DIALOGS_RUN_COMMAND_DIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace NovaPad::UI::Dialogs {

class RunCommandDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RunCommandDialog(QWidget *parent = nullptr);
    ~RunCommandDialog() override = default;

    [[nodiscard]] QString command() const;
    [[nodiscard]] QString workingDirectory() const;

private:
    void setupUi();

    QLineEdit *m_cmdEdit = nullptr;
    QLineEdit *m_cwdEdit = nullptr;
};

} // namespace NovaPad::UI::Dialogs

#endif // NOVAPAD_UI_DIALOGS_RUN_COMMAND_DIALOG_H
