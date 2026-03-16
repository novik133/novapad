/**
 * @file about_dialog.h
 * @brief "About NovaPad" dialog showing version, credits, and license info.
 */

#ifndef NOVAPAD_UI_DIALOGS_ABOUT_DIALOG_H
#define NOVAPAD_UI_DIALOGS_ABOUT_DIALOG_H

#include <QDialog>

namespace NovaPad::UI::Dialogs {

/**
 * @class AboutDialog
 * @brief Modal dialog displaying application version, copyright, and links.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override = default;

private:
    void setupUi();
};

} // namespace NovaPad::UI::Dialogs

#endif // NOVAPAD_UI_DIALOGS_ABOUT_DIALOG_H
