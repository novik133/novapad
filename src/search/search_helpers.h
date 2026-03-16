/**
 * @file search_helpers.h
 * @brief Low-level find/replace operations on a CodeEditorWidget.
 *
 * Provides static utility functions used by both the SearchReplaceDialog
 * and the AdvancedSearchDock.  Supports plain text, regex, case-sensitive,
 * whole-word, and wrap-around modes.
 */

#ifndef NOVAPAD_SEARCH_SEARCH_HELPERS_H
#define NOVAPAD_SEARCH_SEARCH_HELPERS_H

#include <QString>

namespace NovaPad::Editor { class CodeEditorWidget; }

namespace NovaPad::Search {

/**
 * @class SearchHelpers
 * @brief Static utilities for text search and replacement inside an editor.
 */
class SearchHelpers final
{
    Q_DISABLE_COPY_MOVE(SearchHelpers)
    SearchHelpers() = delete;

public:
    /** @brief Aggregated search options. */
    struct FindOptions
    {
        bool caseSensitive = false;
        bool wholeWord     = false;
        bool regex         = false;
        bool wrapAround    = true;
        bool backward      = false;
    };

    /**
     * @brief Searches for @p term in the editor and selects the match.
     * @return true if a match was found.
     */
    [[nodiscard]] static bool findInEditor(
        NovaPad::Editor::CodeEditorWidget *editor,
        const QString &term,
        const FindOptions &opts);

    /**
     * @brief Replaces all occurrences of @p searchTerm with @p replaceTerm.
     * @return The number of replacements made.
     */
    [[nodiscard]] static int replaceAllInEditor(
        NovaPad::Editor::CodeEditorWidget *editor,
        const QString &searchTerm,
        const QString &replaceTerm,
        const FindOptions &opts);

    /**
     * @brief Counts the number of occurrences of @p term in the editor.
     */
    [[nodiscard]] static int countInEditor(
        NovaPad::Editor::CodeEditorWidget *editor,
        const QString &term,
        const FindOptions &opts);
};

} // namespace NovaPad::Search

#endif // NOVAPAD_SEARCH_SEARCH_HELPERS_H
