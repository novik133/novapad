/**
 * @file persistent_cache.h
 * @brief Manages a persistent on-disk cache for session data and unsaved content.
 *
 * Provides a simple key-value store backed by files in the app-data directory.
 * Used to cache the content of unsaved tabs so they survive application restarts.
 */

#ifndef NOVAPAD_SESSIONS_PERSISTENT_CACHE_H
#define NOVAPAD_SESSIONS_PERSISTENT_CACHE_H

#include <QString>

namespace NovaPad::Sessions {

/**
 * @class PersistentCache
 * @brief Static helpers for reading/writing cached document content.
 */
class PersistentCache final
{
    Q_DISABLE_COPY_MOVE(PersistentCache)
    PersistentCache() = delete;

public:
    /** @brief Returns the directory used for session cache files. */
    [[nodiscard]] static QString sessionCachePath();

    /** @brief Writes @p content into the cache under the given @p key. */
    static bool write(const QString &key, const QByteArray &content);

    /** @brief Reads previously cached content for the given @p key. */
    [[nodiscard]] static QByteArray read(const QString &key);

    /** @brief Removes a single cached entry. */
    static void remove(const QString &key);

    /** @brief Removes all cached entries. */
    static void clearAll();
};

} // namespace NovaPad::Sessions

#endif // NOVAPAD_SESSIONS_PERSISTENT_CACHE_H
