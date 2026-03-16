/**
 * @file preferences_dialog.h
 * @brief Application-wide preferences dialog with tabbed settings pages.
 *
 * Mirrors the full feature set of Notepadqq's frmpreferences — general
 * settings, appearance (theme, font, colours), language-specific indentation,
 * search behaviour, keyboard shortcuts, and extension runtime paths.
 */

#ifndef NOVAPAD_UI_DIALOGS_PREFERENCES_DIALOG_H
#define NOVAPAD_UI_DIALOGS_PREFERENCES_DIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QTabWidget>

namespace NovaPad::UI::Dialogs {

/**
 * @class PreferencesDialog
 * @brief Modal dialog for editing all NovaPad settings.
 *
 * Changes are applied immediately (live preview where possible) and
 * persisted to NovaPadSettings when the user clicks OK.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog() override = default;

private slots:
    void onAccepted();
    void onRejected();

private:
    void setupUi();
    QWidget *createGeneralPage();
    QWidget *createAppearancePage();
    QWidget *createLanguagesPage();
    QWidget *createSearchPage();
    QWidget *createShortcutsPage();
    QWidget *createExtensionsPage();

    void loadSettings();
    void saveSettings();

    QTabWidget    *m_tabs              = nullptr;

    // -- General page widgets ------------------------------------------------
    QCheckBox     *m_chkWarnIndent     = nullptr;
    QCheckBox     *m_chkExitOnLastTab  = nullptr;
    QCheckBox     *m_chkRememberTabs   = nullptr;
    QCheckBox     *m_chkSmartIndent    = nullptr;
    QCheckBox     *m_chkMathRendering  = nullptr;
    QSpinBox      *m_spnAutosave      = nullptr;
    QSpinBox      *m_spnWarnFileSize   = nullptr;
    QComboBox     *m_cmbLocale         = nullptr;

    // -- Appearance page widgets ---------------------------------------------
    QComboBox     *m_cmbColorScheme    = nullptr;
    QFontComboBox *m_cmbFontFamily     = nullptr;
    QSpinBox      *m_spnFontSize       = nullptr;
    QCheckBox     *m_chkShowLineNumbers = nullptr;

    // -- Search page widgets -------------------------------------------------
    QCheckBox     *m_chkSearchAsIType  = nullptr;
    QCheckBox     *m_chkSaveHistory    = nullptr;
};

} // namespace NovaPad::UI::Dialogs

#endif // NOVAPAD_UI_DIALOGS_PREFERENCES_DIALOG_H
