/**
 * @file search_worker.h
 * @brief Background worker that searches across files in a directory tree.
 *
 * Scans files matching a glob filter for occurrences of a search term,
 * returning structured results that the AdvancedSearchDock displays in
 * its tree view.
 */

#ifndef NOVAPAD_SEARCH_SEARCH_WORKER_H
#define NOVAPAD_SEARCH_SEARCH_WORKER_H

#include <QString>
#include <QVector>

namespace NovaPad::Search {

/**
 * @struct SearchMatch
 * @brief A single match within a file.
 */
struct SearchMatch
{
    QString filePath;
    int     line      = 0;   ///< 1-indexed line number
    int     column    = 0;   ///< 0-indexed column
    QString lineText;        ///< Full text of the matching line
};

/**
 * @class SearchWorker
 * @brief Performs recursive file search and returns match results.
 */
class SearchWorker final
{
    Q_DISABLE_COPY_MOVE(SearchWorker)
    SearchWorker() = delete;

public:
    /** @brief Options controlling the file search. */
    struct Options
    {
        QString searchTerm;
        QString directory;
        QString filter        = "*.*";
        bool    caseSensitive = false;
        bool    regex         = false;
        bool    wholeWord     = false;
    };

    /**
     * @brief Synchronously searches all matching files and returns results.
     * @param opts  Search configuration.
     * @return List of matches found across all files.
     */
    [[nodiscard]] static QVector<SearchMatch> search(const Options &opts);
};

} // namespace NovaPad::Search

#endif // NOVAPAD_SEARCH_SEARCH_WORKER_H
