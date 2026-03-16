/**
 * @file advanced_search_dock.cpp
 * @brief Implementation of the multi-file search dock widget.
 */

#include "advanced_search_dock.h"
#include "search/search_result_model.h"
#include "search/search_worker.h"

#include <QDir>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>

namespace NovaPad::Search {

AdvancedSearchDock::AdvancedSearchDock(QWidget *parent)
    : QObject(parent)
{
    setupUi();
}

QDockWidget *AdvancedSearchDock::dockWidget() const { return m_dock; }

void AdvancedSearchDock::show(bool focusSearch)
{
    m_dock->show();
    m_dock->raise();
    if (focusSearch) {
        m_cmbSearchTerm->setFocus();
    }
}

void AdvancedSearchDock::setupUi()
{
    m_dock = new QDockWidget(tr("Find in Files"), qobject_cast<QWidget *>(parent()));
    m_dock->setObjectName("advancedSearchDock");

    auto *container = new QWidget(m_dock);
    auto *layout    = new QVBoxLayout(container);
    layout->setContentsMargins(4, 4, 4, 4);

    // -- Search input row ----------------------------------------------------
    auto *inputForm = new QFormLayout();

    m_cmbSearchTerm = new QComboBox(container);
    m_cmbSearchTerm->setEditable(true);
    m_cmbSearchTerm->setMaxCount(20);
    inputForm->addRow(tr("Search:"), m_cmbSearchTerm);

    m_cmbReplaceTerm = new QComboBox(container);
    m_cmbReplaceTerm->setEditable(true);
    m_cmbReplaceTerm->setMaxCount(20);
    inputForm->addRow(tr("Replace:"), m_cmbReplaceTerm);

    m_edtDirectory = new QLineEdit(QDir::currentPath(), container);
    inputForm->addRow(tr("Directory:"), m_edtDirectory);

    m_edtFilter = new QLineEdit("*.*", container);
    inputForm->addRow(tr("Filter:"), m_edtFilter);

    layout->addLayout(inputForm);

    // -- Options row ---------------------------------------------------------
    auto *optLayout = new QHBoxLayout();
    m_chkMatchCase = new QCheckBox(tr("Match case"), container);
    m_chkRegex     = new QCheckBox(tr("Regex"), container);
    m_chkWholeWord = new QCheckBox(tr("Whole word"), container);
    optLayout->addWidget(m_chkMatchCase);
    optLayout->addWidget(m_chkRegex);
    optLayout->addWidget(m_chkWholeWord);

    m_btnSearch  = new QPushButton(tr("Search"), container);
    m_btnReplace = new QPushButton(tr("Replace All"), container);
    optLayout->addStretch();
    optLayout->addWidget(m_btnSearch);
    optLayout->addWidget(m_btnReplace);
    layout->addLayout(optLayout);

    // -- Results tree --------------------------------------------------------
    m_resultModel = new SearchResultModel(container);
    m_resultView  = new QTreeView(container);
    m_resultView->setModel(m_resultModel);
    m_resultView->setHeaderHidden(true);
    m_resultView->setRootIsDecorated(true);
    m_resultView->setAlternatingRowColors(true);
    layout->addWidget(m_resultView, 1);

    m_dock->setWidget(container);

    // -- Connections ---------------------------------------------------------
    connect(m_btnSearch, &QPushButton::clicked,
            this, &AdvancedSearchDock::onSearch);
    connect(m_btnReplace, &QPushButton::clicked,
            this, &AdvancedSearchDock::onReplace);
    connect(m_resultView, &QTreeView::doubleClicked,
            this, &AdvancedSearchDock::onResultDoubleClicked);
}

void AdvancedSearchDock::onSearch()
{
    const QString term = m_cmbSearchTerm->currentText();
    if (term.isEmpty()) return;

    SearchWorker::Options opts;
    opts.searchTerm    = term;
    opts.directory     = m_edtDirectory->text();
    opts.filter        = m_edtFilter->text();
    opts.caseSensitive = m_chkMatchCase->isChecked();
    opts.regex         = m_chkRegex->isChecked();
    opts.wholeWord     = m_chkWholeWord->isChecked();

    m_resultModel->clear();

    // Run synchronously for now; a production version would use QtConcurrent
    auto results = SearchWorker::search(opts);
    m_resultModel->setResults(results);
    m_resultView->expandAll();
}

void AdvancedSearchDock::onReplace()
{
    // TODO: batch replace across files
}

void AdvancedSearchDock::onResultDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    const auto result = m_resultModel->resultAt(index);
    if (!result.filePath.isEmpty()) {
        emit resultActivated(result.filePath, result.line, result.column);
    }
}

} // namespace NovaPad::Search
