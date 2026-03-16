/**
 * @file search_result_model.h
 * @brief Tree model for displaying multi-file search results.
 *
 * Groups matches by file path, with each file as a parent node and
 * individual matches as child nodes.  Used by the AdvancedSearchDock's
 * QTreeView.
 */

#ifndef NOVAPAD_SEARCH_SEARCH_RESULT_MODEL_H
#define NOVAPAD_SEARCH_SEARCH_RESULT_MODEL_H

#include "search/search_worker.h"

#include <QAbstractItemModel>
#include <QVector>

namespace NovaPad::Search {

/**
 * @struct ResultEntry
 * @brief A single entry returned by resultAt() — can be either a file-level
 *        or match-level node.
 */
struct ResultEntry
{
    QString filePath;
    int     line   = -1;
    int     column = -1;
    QString text;
};

/**
 * @class SearchResultModel
 * @brief QAbstractItemModel that presents SearchMatch data in a tree layout.
 */
class SearchResultModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit SearchResultModel(QObject *parent = nullptr);
    ~SearchResultModel() override = default;

    /** @brief Replaces all results with the given list. */
    void setResults(const QVector<SearchMatch> &matches);

    /** @brief Clears all results. */
    void clear();

    /** @brief Returns a ResultEntry for the given model index. */
    [[nodiscard]] ResultEntry resultAt(const QModelIndex &index) const;

    // -- QAbstractItemModel interface ----------------------------------------
    [[nodiscard]] QModelIndex index(int row, int column,
                                    const QModelIndex &parent = {}) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;
    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    /** @brief Internal node for the file grouping level. */
    struct FileGroup
    {
        QString filePath;
        QVector<SearchMatch> matches;
    };

    QVector<FileGroup> m_groups;
};

} // namespace NovaPad::Search

#endif // NOVAPAD_SEARCH_SEARCH_RESULT_MODEL_H
