/**
 * @file test_text_operations.cpp
 * @brief Unit tests for the TextOperations utility class.
 */

#include "utils/text_operations.h"

#include <QTest>

using NovaPad::Utils::TextOperations;

class TestTextOperations : public QObject
{
    Q_OBJECT

private slots:
    // -- Case transformations ------------------------------------------------
    void testToUpperCase()
    {
        QCOMPARE(TextOperations::toUpperCase("hello World"), QString("HELLO WORLD"));
    }

    void testToLowerCase()
    {
        QCOMPARE(TextOperations::toLowerCase("Hello WORLD"), QString("hello world"));
    }

    void testToTitleCase()
    {
        QCOMPARE(TextOperations::toTitleCase("hello world foo"), QString("Hello World Foo"));
    }

    void testInvertCase()
    {
        QCOMPARE(TextOperations::invertCase("Hello World"), QString("hELLO wORLD"));
    }

    // -- Whitespace operations -----------------------------------------------
    void testTrimTrailing()
    {
        QCOMPARE(TextOperations::trimTrailing("hello   \nworld  "), QString("hello\nworld"));
    }

    void testTrimLeading()
    {
        QCOMPARE(TextOperations::trimLeading("   hello\n  world"), QString("hello\nworld"));
    }

    void testTrimBoth()
    {
        QCOMPARE(TextOperations::trimBoth("  hello  \n  world  "), QString("hello\nworld"));
    }

    void testTabsToSpaces()
    {
        QCOMPARE(TextOperations::tabsToSpaces("\thello", 4), QString("    hello"));
    }

    void testSpacesToTabs()
    {
        QCOMPARE(TextOperations::spacesToTabs("    hello", 4), QString("\thello"));
    }

    void testEolToSpace()
    {
        QCOMPARE(TextOperations::eolToSpace("hello\nworld"), QString("hello world"));
    }

    void testRemoveBlankLines()
    {
        QCOMPARE(TextOperations::removeBlankLines("hello\n\n\nworld\n\n"),
                 QString("hello\nworld"));
    }

    // -- Line operations -----------------------------------------------------
    void testSortLinesAscending()
    {
        QCOMPARE(TextOperations::sortLinesAscending("cherry\napple\nbanana"),
                 QString("apple\nbanana\ncherry"));
    }

    void testSortLinesDescending()
    {
        QCOMPARE(TextOperations::sortLinesDescending("apple\nbanana\ncherry"),
                 QString("cherry\nbanana\napple"));
    }

    void testRemoveDuplicateLines()
    {
        QCOMPARE(TextOperations::removeDuplicateLines("aaa\nbbb\naaa\nccc"),
                 QString("aaa\nbbb\nccc"));
    }

    void testReverseLines()
    {
        QCOMPARE(TextOperations::reverseLines("first\nsecond\nthird"),
                 QString("third\nsecond\nfirst"));
    }

    void testJoinLines()
    {
        QCOMPARE(TextOperations::joinLines("a\nb\nc", ", "), QString("a, b, c"));
    }

    // -- Encoding helpers ----------------------------------------------------
    void testBase64RoundTrip()
    {
        const QString original = "Hello NovaPad!";
        const QString encoded = TextOperations::base64Encode(original);
        QCOMPARE(TextOperations::base64Decode(encoded), original);
    }

    void testUrlEncodeRoundTrip()
    {
        const QString original = "hello world&foo=bar";
        const QString encoded = TextOperations::urlEncode(original);
        QCOMPARE(TextOperations::urlDecode(encoded), original);
    }
};

QTEST_MAIN(TestTextOperations)
#include "test_text_operations.moc"
