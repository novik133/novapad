/**
 * @file advanced_search_dock.h
 * @brief Multi-file search dock widget with tree-view results.
 *
 * Provides grep-like search across files in a directory, with results
 * displayed in a collapsible tree.  Clicking a result navigates to the
 * matching line in the editor.  Mirrors Notepadqq's AdvancedSearchDock.
 */

#ifndef NOVAPAD_SEARCH_ADVANCED_SEARCH_DOCK_H
#define NOVAPAD_SEARCH_ADVANCED_SEARCH_DOCK_H

#include <QCheckBox>
#include <QComboBox>
#include <QDockWidget>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QTreeView>

namespace NovaPad::Search {

class SearchResultModel; // forward declaration

/**
 * @class AdvancedSearchDock
 * @brief Dock widget for searching across multiple files in a directory.
 */
class AdvancedSearchDock : public QObject
{
    Q_OBJECT

public:
    explicit AdvancedSearchDock(QWidget *parent = nullptr);
    ~AdvancedSearchDock() override = default;

    /** @brief Returns the underlying QDockWidget for adding to the main window. */
    [[nodiscard]] QDockWidget *dockWidget() const;

    /** @brief Shows the dock and optionally focuses the search field. */
    void show(bool focusSearch = true);

signals:
    /** @brief Emitted when the user double-clicks a result entry. */
    void resultActivated(const QString &filePath, int line, int column);

private slots:
    void onSearch();
    void onReplace();
    void onResultDoubleClicked(const QModelIndex &index);

private:
    void setupUi();

    QDockWidget       *m_dock          = nullptr;
    QComboBox         *m_cmbSearchTerm = nullptr;
    QComboBox         *m_cmbReplaceTerm= nullptr;
    QLineEdit         *m_edtDirectory  = nullptr;
    QLineEdit         *m_edtFilter     = nullptr;
    QCheckBox         *m_chkMatchCase  = nullptr;
    QCheckBox         *m_chkRegex      = nullptr;
    QCheckBox         *m_chkWholeWord  = nullptr;
    QPushButton       *m_btnSearch     = nullptr;
    QPushButton       *m_btnReplace    = nullptr;
    QTreeView         *m_resultView    = nullptr;
    SearchResultModel *m_resultModel   = nullptr;
};

} // namespace NovaPad::Search

#endif // NOVAPAD_SEARCH_ADVANCED_SEARCH_DOCK_H
