/**
 * @file persistent_cache.cpp
 * @brief Implementation of the persistent on-disk cache for session data.
 *
 * Each cache entry is stored as a file whose name is derived from the key
 * via a simple hash.  The cache directory lives inside the application
 * data folder and is cleaned up on normal shutdown.
 */

#include "persistent_cache.h"
#include "core/globals.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>

namespace NovaPad::Sessions {

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

QString PersistentCache::sessionCachePath()
{
    const QString dir = NovaPad::Core::Globals::appDataPath("cache/sessions");
    QDir().mkpath(dir);
    return dir;
}

bool PersistentCache::write(const QString &key, const QByteArray &content)
{
    const QString path = sessionCachePath() + "/" +
        QString::fromLatin1(
            QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1).toHex());

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(content);
    file.close();
    return true;
}

QByteArray PersistentCache::read(const QString &key)
{
    const QString path = sessionCachePath() + "/" +
        QString::fromLatin1(
            QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1).toHex());

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    return file.readAll();
}

void PersistentCache::remove(const QString &key)
{
    const QString path = sessionCachePath() + "/" +
        QString::fromLatin1(
            QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1).toHex());
    QFile::remove(path);
}

void PersistentCache::clearAll()
{
    QDir dir(sessionCachePath());
    const QStringList entries = dir.entryList(QDir::Files);
    for (const QString &entry : entries) {
        dir.remove(entry);
    }
}

} // namespace NovaPad::Sessions
