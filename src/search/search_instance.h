/**
 * @file search_instance.h
 * @brief Lightweight value type representing a single search configuration.
 *
 * Encapsulates the search term and all associated options so they can be
 * passed around as a single unit and stored in search history.
 */

#ifndef NOVAPAD_SEARCH_SEARCH_INSTANCE_H
#define NOVAPAD_SEARCH_SEARCH_INSTANCE_H

#include <QString>

namespace NovaPad::Search {

/**
 * @struct SearchInstance
 * @brief Immutable snapshot of a search configuration.
 */
struct SearchInstance
{
    QString term;
    bool caseSensitive = false;
    bool wholeWord     = false;
    bool regex         = false;
    bool wrapAround    = true;
    bool backward      = false;

    [[nodiscard]] bool isValid() const { return !term.isEmpty(); }
    [[nodiscard]] auto operator<=>(const SearchInstance &) const = default;
};

} // namespace NovaPad::Search

#endif // NOVAPAD_SEARCH_SEARCH_INSTANCE_H
