/**
 * @file test_color_scheme.cpp
 * @brief Unit tests for the ColorScheme JSON serialisation.
 */

#include "themes/color_scheme.h"

#include <QJsonDocument>
#include <QTest>

using NovaPad::Themes::ColorScheme;

class TestColorScheme : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultValues()
    {
        ColorScheme cs;
        QVERIFY(cs.background.isValid());
        QVERIFY(cs.keyword.isValid());
        QVERIFY(cs.comment.isValid());
    }

    void testRoundTrip()
    {
        ColorScheme original;
        original.id   = "test-theme";
        original.name = "Test Theme";
        original.background = QColor("#1E1E1E");
        original.keyword    = QColor("#569CD6");
        original.string     = QColor("#CE9178");
        original.comment    = QColor("#6A9955");

        // Serialise to JSON
        QJsonObject json = original.toJson();
        QCOMPARE(json["id"].toString(), QString("test-theme"));
        QCOMPARE(json["name"].toString(), QString("Test Theme"));

        // Deserialise back
        ColorScheme restored = ColorScheme::fromJson(json);
        QCOMPARE(restored.id, original.id);
        QCOMPARE(restored.name, original.name);
        QCOMPARE(restored.background, original.background);
        QCOMPARE(restored.keyword, original.keyword);
        QCOMPARE(restored.string, original.string);
        QCOMPARE(restored.comment, original.comment);
    }

    void testFromJsonMissingFields()
    {
        // Missing fields should use defaults
        QJsonObject minimal;
        minimal["id"]   = "minimal";
        minimal["name"] = "Minimal Theme";

        ColorScheme cs = ColorScheme::fromJson(minimal);
        QCOMPARE(cs.id, QString("minimal"));
        // Default background should still be valid
        QVERIFY(cs.background.isValid());
        QVERIFY(cs.keyword.isValid());
    }

    void testFromEmptyJson()
    {
        QJsonObject empty;
        ColorScheme cs = ColorScheme::fromJson(empty);
        QVERIFY(cs.id.isEmpty());
        // All colours should still have valid defaults
        QVERIFY(cs.background.isValid());
        QVERIFY(cs.foreground.isValid());
    }

    void testToJsonContainsAllKeys()
    {
        ColorScheme cs;
        cs.id   = "full";
        cs.name = "Full Theme";

        QJsonObject json = cs.toJson();

        QVERIFY(json.contains("id"));
        QVERIFY(json.contains("name"));
        QVERIFY(json.contains("background"));
        QVERIFY(json.contains("foreground"));
        QVERIFY(json.contains("editorBackground"));
        QVERIFY(json.contains("editorForeground"));
        QVERIFY(json.contains("lineHighlight"));
        QVERIFY(json.contains("selectionBackground"));
        QVERIFY(json.contains("selectionForeground"));
        QVERIFY(json.contains("gutterBackground"));
        QVERIFY(json.contains("gutterForeground"));
        QVERIFY(json.contains("keyword"));
        QVERIFY(json.contains("string"));
        QVERIFY(json.contains("comment"));
        QVERIFY(json.contains("number"));
        QVERIFY(json.contains("function"));
        QVERIFY(json.contains("type"));
        QVERIFY(json.contains("preprocessor"));
        QVERIFY(json.contains("operator"));
    }
};

QTEST_MAIN(TestColorScheme)
#include "test_color_scheme.moc"
