/**
 * @file search_replace_dialog.h
 * @brief In-document search and replace dialog.
 *
 * Provides find, find-and-replace, and find-all functionality with support
 * for plain text, regex, case sensitivity, whole-word matching, and
 * wrap-around modes.  Mirrors Notepadqq's frmSearchReplace.
 */

#ifndef NOVAPAD_SEARCH_SEARCH_REPLACE_DIALOG_H
#define NOVAPAD_SEARCH_SEARCH_REPLACE_DIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QPushButton>
#include <QTabWidget>

namespace NovaPad::UI { class TopEditorContainer; }

namespace NovaPad::Search {

/**
 * @class SearchReplaceDialog
 * @brief Modal or modeless dialog for searching and replacing within the
 *        currently active editor.
 */
class SearchReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    enum Tab { TabSearch = 0, TabReplace = 1 };

    explicit SearchReplaceDialog(NovaPad::UI::TopEditorContainer *container,
                                 QWidget *parent = nullptr);
    ~SearchReplaceDialog() override = default;

    /** @brief Shows the dialog, optionally switching to a specific tab. */
    void show(Tab tab = TabSearch);

    /** @brief Triggers a find operation using the current UI values. */
    void findFromUI(bool forward);

signals:
    void searchCompleted(int matchCount);

private slots:
    void onFindNext();
    void onFindPrevious();
    void onFindAll();
    void onReplace();
    void onReplaceAll();
    void onCountAll();

private:
    void setupUi();

    NovaPad::UI::TopEditorContainer *m_container = nullptr;

    QTabWidget  *m_tabWidget       = nullptr;
    QComboBox   *m_cmbSearchTerm   = nullptr;
    QComboBox   *m_cmbReplaceTerm  = nullptr;
    QCheckBox   *m_chkMatchCase    = nullptr;
    QCheckBox   *m_chkWholeWord    = nullptr;
    QCheckBox   *m_chkRegex        = nullptr;
    QCheckBox   *m_chkWrapAround   = nullptr;
    QPushButton *m_btnFindNext     = nullptr;
    QPushButton *m_btnFindPrev     = nullptr;
    QPushButton *m_btnFindAll      = nullptr;
    QPushButton *m_btnReplace      = nullptr;
    QPushButton *m_btnReplaceAll   = nullptr;
    QPushButton *m_btnCount        = nullptr;
};

} // namespace NovaPad::Search

#endif // NOVAPAD_SEARCH_SEARCH_REPLACE_DIALOG_H
