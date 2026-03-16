/**
 * @file test_persistent_cache.cpp
 * @brief Unit tests for the PersistentCache utility class.
 */

#include "sessions/persistent_cache.h"

#include <QDir>
#include <QTest>

using NovaPad::Sessions::PersistentCache;

class TestPersistentCache : public QObject
{
    Q_OBJECT

private slots:
    void testWriteAndRead()
    {
        const QString key = "test_key_1";
        const QByteArray content = "Hello, NovaPad cache!";

        bool ok = PersistentCache::write(key, content);
        QVERIFY(ok);

        QByteArray readBack = PersistentCache::read(key);
        QCOMPARE(readBack, content);

        // Clean up
        PersistentCache::remove(key);
    }

    void testReadNonExistent()
    {
        QByteArray data = PersistentCache::read("definitely_not_a_key_12345");
        QVERIFY(data.isEmpty());
    }

    void testRemove()
    {
        const QString key = "test_key_remove";
        PersistentCache::write(key, "data");

        PersistentCache::remove(key);
        QByteArray data = PersistentCache::read(key);
        QVERIFY(data.isEmpty());
    }

    void testOverwrite()
    {
        const QString key = "test_key_overwrite";
        PersistentCache::write(key, "first");
        PersistentCache::write(key, "second");

        QByteArray data = PersistentCache::read(key);
        QCOMPARE(data, QByteArray("second"));

        PersistentCache::remove(key);
    }

    void testClearAll()
    {
        PersistentCache::write("clear_a", "aaa");
        PersistentCache::write("clear_b", "bbb");

        PersistentCache::clearAll();

        QVERIFY(PersistentCache::read("clear_a").isEmpty());
        QVERIFY(PersistentCache::read("clear_b").isEmpty());
    }

    void testSessionCachePathExists()
    {
        const QString path = PersistentCache::sessionCachePath();
        QVERIFY(!path.isEmpty());
        QVERIFY(QDir(path).exists());
    }
};

QTEST_MAIN(TestPersistentCache)
#include "test_persistent_cache.moc"
