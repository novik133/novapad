/**
 * @file test_encoding_detector.cpp
 * @brief Unit tests for the EncodingDetector utility class.
 */

#include "utils/encoding_detector.h"

#include <QTest>
#include <QTextCodec>

using NovaPad::Utils::EncodingDetector;

class TestEncodingDetector : public QObject
{
    Q_OBJECT

private slots:
    void testEmptyData()
    {
        auto result = EncodingDetector::detect({});
        QVERIFY(result.codec != nullptr);
        QCOMPARE(result.codec->name(), QByteArray("UTF-8"));
        QCOMPARE(result.hasBom, false);
    }

    void testUtf8Bom()
    {
        QByteArray data("\xEF\xBB\xBF" "Hello", 8);
        auto result = EncodingDetector::detect(data);
        QVERIFY(result.codec != nullptr);
        QCOMPARE(result.codec->name(), QByteArray("UTF-8"));
        QCOMPARE(result.hasBom, true);
    }

    void testUtf16BeBom()
    {
        QByteArray data("\xFE\xFF\x00H\x00e\x00l\x00l\x00o", 12);
        auto result = EncodingDetector::detect(data);
        QVERIFY(result.codec != nullptr);
        QCOMPARE(result.hasBom, true);
        // The codec name should contain "UTF-16BE"
        QVERIFY(result.codec->name().toUpper().contains("UTF-16"));
    }

    void testUtf16LeBom()
    {
        QByteArray data("\xFF\xFE" "H\x00" "e\x00" "l\x00" "l\x00" "o\x00", 12);
        auto result = EncodingDetector::detect(data);
        QVERIFY(result.codec != nullptr);
        QCOMPARE(result.hasBom, true);
    }

    void testPlainAscii()
    {
        QByteArray data("Hello world! This is plain ASCII text.\n");
        auto result = EncodingDetector::detect(data);
        QVERIFY(result.codec != nullptr);
        QCOMPARE(result.hasBom, false);
        // Plain ASCII is valid UTF-8
        QCOMPARE(result.codec->name(), QByteArray("UTF-8"));
    }

    void testUtf8Multibyte()
    {
        // Polish characters encoded as UTF-8 (no BOM)
        QByteArray data("Zażółć gęślą jaźń");
        auto result = EncodingDetector::detect(data);
        QVERIFY(result.codec != nullptr);
        QCOMPARE(result.hasBom, false);
        QCOMPARE(result.codec->name(), QByteArray("UTF-8"));
    }

    void testBomForCodecUtf8()
    {
        auto *codec = QTextCodec::codecForName("UTF-8");
        QByteArray bom = EncodingDetector::bomForCodec(codec);
        QCOMPARE(bom, QByteArray("\xEF\xBB\xBF", 3));
    }

    void testBomForCodecNull()
    {
        QByteArray bom = EncodingDetector::bomForCodec(nullptr);
        QVERIFY(bom.isEmpty());
    }

    void testBomForCodecLatin1()
    {
        auto *codec = QTextCodec::codecForName("ISO-8859-1");
        QByteArray bom = EncodingDetector::bomForCodec(codec);
        QVERIFY(bom.isEmpty());
    }
};

QTEST_MAIN(TestEncodingDetector)
#include "test_encoding_detector.moc"
