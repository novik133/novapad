/**
 * @file search_result_model.cpp
 * @brief Implementation of the tree model for multi-file search results.
 *
 * Results are grouped by file path.  The internal pointer stored in each
 * QModelIndex distinguishes top-level (file) nodes from child (match) nodes:
 *   - Top-level: internalId() == -1
 *   - Child:     internalId() == index of the parent FileGroup
 */

#include "search_result_model.h"

#include <QFileInfo>

namespace NovaPad::Search {

SearchResultModel::SearchResultModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

void SearchResultModel::setResults(const QVector<SearchMatch> &matches)
{
    beginResetModel();
    m_groups.clear();

    // Group matches by file path
    QMap<QString, int> fileIndex;
    for (const auto &match : matches) {
        int idx;
        if (fileIndex.contains(match.filePath)) {
            idx = fileIndex[match.filePath];
        } else {
            idx = m_groups.size();
            fileIndex[match.filePath] = idx;
            m_groups.append({ match.filePath, {} });
        }
        m_groups[idx].matches.append(match);
    }

    endResetModel();
}

void SearchResultModel::clear()
{
    beginResetModel();
    m_groups.clear();
    endResetModel();
}

ResultEntry SearchResultModel::resultAt(const QModelIndex &index) const
{
    if (!index.isValid()) return {};

    const quintptr id = index.internalId();

    if (id == quintptr(-1)) {
        // Top-level (file) node
        if (index.row() < m_groups.size()) {
            return { m_groups[index.row()].filePath, -1, -1, {} };
        }
    } else {
        // Child (match) node
        const int groupIdx = static_cast<int>(id);
        if (groupIdx < m_groups.size() && index.row() < m_groups[groupIdx].matches.size()) {
            const auto &m = m_groups[groupIdx].matches[index.row()];
            return { m.filePath, m.line, m.column, m.lineText };
        }
    }

    return {};
}

// ---------------------------------------------------------------------------
// QAbstractItemModel interface
// ---------------------------------------------------------------------------

QModelIndex SearchResultModel::index(int row, int column,
                                     const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) return {};

    if (!parent.isValid()) {
        // Top-level node — internalId = -1
        return createIndex(row, column, quintptr(-1));
    }

    // Child node — internalId = parent row (group index)
    return createIndex(row, column, quintptr(parent.row()));
}

QModelIndex SearchResultModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) return {};

    const quintptr id = child.internalId();
    if (id == quintptr(-1)) {
        // Top-level node has no parent
        return {};
    }

    // Child node — parent is the top-level node at index `id`
    return createIndex(static_cast<int>(id), 0, quintptr(-1));
}

int SearchResultModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        // Root → number of file groups
        return m_groups.size();
    }

    const quintptr id = parent.internalId();
    if (id == quintptr(-1)) {
        // Top-level node → number of matches in this group
        if (parent.row() < m_groups.size()) {
            return m_groups[parent.row()].matches.size();
        }
    }

    // Child nodes have no children
    return 0;
}

int SearchResultModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant SearchResultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) return {};

    const quintptr id = index.internalId();

    if (id == quintptr(-1)) {
        // Top-level: show file name + match count
        if (index.row() < m_groups.size()) {
            const auto &group = m_groups[index.row()];
            return QStringLiteral("%1 (%2 matches)")
                .arg(QFileInfo(group.filePath).fileName())
                .arg(group.matches.size());
        }
    } else {
        // Child: show "line N: <text>"
        const int groupIdx = static_cast<int>(id);
        if (groupIdx < m_groups.size()
            && index.row() < m_groups[groupIdx].matches.size()) {
            const auto &m = m_groups[groupIdx].matches[index.row()];
            return QStringLiteral("Line %1: %2").arg(m.line).arg(m.lineText);
        }
    }

    return {};
}

} // namespace NovaPad::Search
