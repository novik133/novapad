/**
 * @file test_search_helpers.cpp
 * @brief Unit tests for the SearchHelpers utility class.
 */

#include "search/search_helpers.h"
#include "editor/code_editor_widget.h"

#include <QApplication>
#include <QTest>

using NovaPad::Search::SearchHelpers;

class TestSearchHelpers : public QObject
{
    Q_OBJECT

private slots:
    void testFindPlainText()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("Hello world, hello universe");

        SearchHelpers::FindOptions opts;
        opts.caseSensitive = false;

        bool found = SearchHelpers::findInEditor(&editor, "hello", opts);
        QVERIFY(found);
        QCOMPARE(editor.textCursor().selectedText(), QString("Hello"));
    }

    void testFindCaseSensitive()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("Hello world, hello universe");

        SearchHelpers::FindOptions opts;
        opts.caseSensitive = true;

        bool found = SearchHelpers::findInEditor(&editor, "hello", opts);
        QVERIFY(found);
        QCOMPARE(editor.textCursor().selectedText(), QString("hello"));
    }

    void testFindNotFound()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("Hello world");

        SearchHelpers::FindOptions opts;
        opts.wrapAround = false;

        bool found = SearchHelpers::findInEditor(&editor, "xyz", opts);
        QVERIFY(!found);
    }

    void testCountOccurrences()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("aaa bbb aaa ccc aaa");

        SearchHelpers::FindOptions opts;
        int count = SearchHelpers::countInEditor(&editor, "aaa", opts);
        QCOMPARE(count, 3);
    }

    void testReplaceAll()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("foo bar foo baz foo");

        SearchHelpers::FindOptions opts;
        int count = SearchHelpers::replaceAllInEditor(&editor, "foo", "qux", opts);
        QCOMPARE(count, 3);
        QCOMPARE(editor.toPlainText(), QString("qux bar qux baz qux"));
    }

    void testReplaceAllCaseSensitive()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("Foo foo FOO");

        SearchHelpers::FindOptions opts;
        opts.caseSensitive = true;
        int count = SearchHelpers::replaceAllInEditor(&editor, "foo", "bar", opts);
        QCOMPARE(count, 1);
        QCOMPARE(editor.toPlainText(), QString("Foo bar FOO"));
    }

    void testFindRegex()
    {
        NovaPad::Editor::CodeEditorWidget editor;
        editor.setPlainText("int x = 42;\nfloat y = 3.14;");

        SearchHelpers::FindOptions opts;
        opts.regex = true;

        bool found = SearchHelpers::findInEditor(&editor, R"(\d+\.\d+)", opts);
        QVERIFY(found);
        QCOMPARE(editor.textCursor().selectedText(), QString("3.14"));
    }
};

QTEST_MAIN(TestSearchHelpers)
#include "test_search_helpers.moc"
