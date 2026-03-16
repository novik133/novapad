/**
 * @file go_to_line_dialog.h
 * @brief Simple dialog prompting the user for a line number to jump to.
 */

#ifndef NOVAPAD_UI_DIALOGS_GO_TO_LINE_DIALOG_H
#define NOVAPAD_UI_DIALOGS_GO_TO_LINE_DIALOG_H

#include <QDialog>
#include <QSpinBox>

namespace NovaPad::UI::Dialogs {

class GoToLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoToLineDialog(int maxLines, QWidget *parent = nullptr);
    ~GoToLineDialog() override = default;

    /** @brief Returns the 1-indexed line number chosen by the user. */
    [[nodiscard]] int selectedLine() const;

private:
    QSpinBox *m_spnLine = nullptr;
};

} // namespace NovaPad::UI::Dialogs

#endif // NOVAPAD_UI_DIALOGS_GO_TO_LINE_DIALOG_H
