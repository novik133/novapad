/**
 * @file indentation_dialog.h
 * @brief Dialog for configuring tab size and spaces-vs-tabs indentation.
 */

#ifndef NOVAPAD_UI_DIALOGS_INDENTATION_DIALOG_H
#define NOVAPAD_UI_DIALOGS_INDENTATION_DIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QSpinBox>

namespace NovaPad::UI::Dialogs {

class IndentationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IndentationDialog(QWidget *parent = nullptr);
    ~IndentationDialog() override = default;

    [[nodiscard]] bool useTabs() const;
    [[nodiscard]] int  tabSize() const;

private:
    void setupUi();
    QCheckBox *m_chkUseTabs = nullptr;
    QSpinBox  *m_spnTabSize = nullptr;
};

} // namespace NovaPad::UI::Dialogs

#endif // NOVAPAD_UI_DIALOGS_INDENTATION_DIALOG_H
