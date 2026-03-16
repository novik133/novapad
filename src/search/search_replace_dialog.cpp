/**
 * @file search_replace_dialog.cpp
 * @brief Implementation of the in-document search and replace dialog.
 *
 * The dialog operates on the currently active editor obtained from the
 * TopEditorContainer.  Search history is maintained in QComboBox dropdowns
 * so the user can quickly recall previous search terms.
 */

#include "search_replace_dialog.h"
#include "search/search_helpers.h"
#include "ui/top_editor_container.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextCursor>
#include <QTimer>
#include <QVBoxLayout>

namespace NovaPad::Search {

SearchReplaceDialog::SearchReplaceDialog(
    NovaPad::UI::TopEditorContainer *container, QWidget *parent)
    : QDialog(parent)
    , m_container(container)
{
    setWindowTitle(tr("Find / Replace"));
    setMinimumWidth(480);
    setupUi();
}

void SearchReplaceDialog::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget(this);

    // -- Search tab ----------------------------------------------------------
    auto *searchPage = new QWidget(this);
    auto *searchLayout = new QVBoxLayout(searchPage);

    auto *searchForm = new QFormLayout();
    m_cmbSearchTerm = new QComboBox(searchPage);
    m_cmbSearchTerm->setEditable(true);
    m_cmbSearchTerm->setMaxCount(20);
    m_cmbSearchTerm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchForm->addRow(tr("Search for:"), m_cmbSearchTerm);
    searchLayout->addLayout(searchForm);

    // -- Options group -------------------------------------------------------
    auto *optGroup  = new QGroupBox(tr("Options"), searchPage);
    auto *optLayout = new QHBoxLayout(optGroup);

    m_chkMatchCase  = new QCheckBox(tr("Match &case"), optGroup);
    m_chkWholeWord  = new QCheckBox(tr("Whole &word"), optGroup);
    m_chkRegex      = new QCheckBox(tr("Re&gular expression"), optGroup);
    m_chkWrapAround = new QCheckBox(tr("&Wrap around"), optGroup);
    m_chkWrapAround->setChecked(true);

    optLayout->addWidget(m_chkMatchCase);
    optLayout->addWidget(m_chkWholeWord);
    optLayout->addWidget(m_chkRegex);
    optLayout->addWidget(m_chkWrapAround);
    searchLayout->addWidget(optGroup);

    // -- Search buttons ------------------------------------------------------
    auto *searchBtnLayout = new QHBoxLayout();
    m_btnFindNext = new QPushButton(tr("Find &Next"), searchPage);
    m_btnFindPrev = new QPushButton(tr("Find &Previous"), searchPage);
    m_btnFindAll  = new QPushButton(tr("Find &All"), searchPage);
    m_btnCount    = new QPushButton(tr("C&ount"), searchPage);

    searchBtnLayout->addWidget(m_btnFindNext);
    searchBtnLayout->addWidget(m_btnFindPrev);
    searchBtnLayout->addWidget(m_btnFindAll);
    searchBtnLayout->addWidget(m_btnCount);
    searchBtnLayout->addStretch();
    searchLayout->addLayout(searchBtnLayout);

    m_tabWidget->addTab(searchPage, tr("&Search"));

    // -- Replace tab ---------------------------------------------------------
    auto *replacePage   = new QWidget(this);
    auto *replaceLayout = new QVBoxLayout(replacePage);

    auto *replaceForm = new QFormLayout();
    m_cmbReplaceTerm = new QComboBox(replacePage);
    m_cmbReplaceTerm->setEditable(true);
    m_cmbReplaceTerm->setMaxCount(20);
    m_cmbReplaceTerm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    replaceForm->addRow(tr("Replace with:"), m_cmbReplaceTerm);
    replaceLayout->addLayout(replaceForm);

    auto *replaceBtnLayout = new QHBoxLayout();
    m_btnReplace    = new QPushButton(tr("&Replace"), replacePage);
    m_btnReplaceAll = new QPushButton(tr("Replace &All"), replacePage);
    replaceBtnLayout->addWidget(m_btnReplace);
    replaceBtnLayout->addWidget(m_btnReplaceAll);
    replaceBtnLayout->addStretch();
    replaceLayout->addLayout(replaceBtnLayout);

    m_tabWidget->addTab(replacePage, tr("&Replace"));

    mainLayout->addWidget(m_tabWidget);

    // -- Connect signals -----------------------------------------------------
    connect(m_btnFindNext, &QPushButton::clicked, this, &SearchReplaceDialog::onFindNext);
    connect(m_btnFindPrev, &QPushButton::clicked, this, &SearchReplaceDialog::onFindPrevious);
    connect(m_btnFindAll,  &QPushButton::clicked, this, &SearchReplaceDialog::onFindAll);
    connect(m_btnCount,    &QPushButton::clicked, this, &SearchReplaceDialog::onCountAll);
    connect(m_btnReplace,    &QPushButton::clicked, this, &SearchReplaceDialog::onReplace);
    connect(m_btnReplaceAll, &QPushButton::clicked, this, &SearchReplaceDialog::onReplaceAll);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void SearchReplaceDialog::show(Tab tab)
{
    m_tabWidget->setCurrentIndex(static_cast<int>(tab));

    // Pre-populate the search field with the editor's current selection
    if (m_container) {
        auto *tw = m_container->currentTabWidget();
        if (tw) {
            auto editor = tw->currentEditor();
            if (editor) {
                const QStringList sel = editor->selectedTexts();
                if (!sel.isEmpty() && !sel.first().isEmpty()) {
                    m_cmbSearchTerm->setCurrentText(sel.first());
                }
            }
        }
    }

    QDialog::show();
    m_cmbSearchTerm->setFocus();
    m_cmbSearchTerm->lineEdit()->selectAll();
}

void SearchReplaceDialog::findFromUI(bool forward)
{
    if (forward) onFindNext();
    else         onFindPrevious();
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------

void SearchReplaceDialog::onFindNext()
{
    if (!m_container) return;
    auto *tw = m_container->currentTabWidget();
    if (!tw) return;
    auto editor = tw->currentEditor();
    if (!editor) return;

    const QString term = m_cmbSearchTerm->currentText();
    if (term.isEmpty()) return;

    SearchHelpers::FindOptions opts;
    opts.caseSensitive = m_chkMatchCase->isChecked();
    opts.wholeWord     = m_chkWholeWord->isChecked();
    opts.regex         = m_chkRegex->isChecked();
    opts.wrapAround    = m_chkWrapAround->isChecked();
    opts.backward      = false;

    if (!SearchHelpers::findInEditor(editor.get(), term, opts)) {
        // Visual feedback — flash the search field
        m_cmbSearchTerm->setStyleSheet("background-color: #FFB0B0;");
        QTimer::singleShot(800, this, [this]() {
            m_cmbSearchTerm->setStyleSheet({});
        });
    } else {
        m_cmbSearchTerm->setStyleSheet({});
    }
}

void SearchReplaceDialog::onFindPrevious()
{
    if (!m_container) return;
    auto *tw = m_container->currentTabWidget();
    if (!tw) return;
    auto editor = tw->currentEditor();
    if (!editor) return;

    const QString term = m_cmbSearchTerm->currentText();
    if (term.isEmpty()) return;

    SearchHelpers::FindOptions opts;
    opts.caseSensitive = m_chkMatchCase->isChecked();
    opts.wholeWord     = m_chkWholeWord->isChecked();
    opts.regex         = m_chkRegex->isChecked();
    opts.wrapAround    = m_chkWrapAround->isChecked();
    opts.backward      = true;

    SearchHelpers::findInEditor(editor.get(), term, opts);
}

void SearchReplaceDialog::onFindAll()
{
    // TODO: highlight all occurrences in the editor
    onFindNext();
}

void SearchReplaceDialog::onReplace()
{
    if (!m_container) return;
    auto *tw = m_container->currentTabWidget();
    if (!tw) return;
    auto editor = tw->currentEditor();
    if (!editor) return;

    const QString searchTerm  = m_cmbSearchTerm->currentText();
    const QString replaceTerm = m_cmbReplaceTerm->currentText();
    if (searchTerm.isEmpty()) return;

    QTextCursor tc = editor->textCursor();
    if (tc.hasSelection() && tc.selectedText() == searchTerm) {
        tc.insertText(replaceTerm);
        editor->setTextCursor(tc);
    }

    // Move to next occurrence
    onFindNext();
}

void SearchReplaceDialog::onReplaceAll()
{
    if (!m_container) return;
    auto *tw = m_container->currentTabWidget();
    if (!tw) return;
    auto editor = tw->currentEditor();
    if (!editor) return;

    const QString searchTerm  = m_cmbSearchTerm->currentText();
    const QString replaceTerm = m_cmbReplaceTerm->currentText();
    if (searchTerm.isEmpty()) return;

    SearchHelpers::FindOptions opts;
    opts.caseSensitive = m_chkMatchCase->isChecked();
    opts.wholeWord     = m_chkWholeWord->isChecked();
    opts.regex         = m_chkRegex->isChecked();

    int count = SearchHelpers::replaceAllInEditor(editor.get(), searchTerm, replaceTerm, opts);
    emit searchCompleted(count);

    QMessageBox::information(this, tr("Replace All"),
                             tr("Replaced %1 occurrence(s).").arg(count));
}

void SearchReplaceDialog::onCountAll()
{
    if (!m_container) return;
    auto *tw = m_container->currentTabWidget();
    if (!tw) return;
    auto editor = tw->currentEditor();
    if (!editor) return;

    const QString term = m_cmbSearchTerm->currentText();
    if (term.isEmpty()) return;

    SearchHelpers::FindOptions opts;
    opts.caseSensitive = m_chkMatchCase->isChecked();
    opts.wholeWord     = m_chkWholeWord->isChecked();
    opts.regex         = m_chkRegex->isChecked();

    int count = SearchHelpers::countInEditor(editor.get(), term, opts);
    QMessageBox::information(this, tr("Count"),
                             tr("Found %1 occurrence(s).").arg(count));
}

} // namespace NovaPad::Search
