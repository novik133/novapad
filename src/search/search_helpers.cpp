/**
 * @file search_helpers.cpp
 * @brief Implementation of low-level find/replace operations.
 *
 * All search logic is centralised here so that both the in-document dialog
 * and the advanced multi-file search share the same matching engine.
 * QTextDocument::find() handles most of the heavy lifting; we layer
 * regex and whole-word support on top.
 */

#include "search_helpers.h"
#include "editor/code_editor_widget.h"

#include <QRegularExpression>
#include <QTextCursor>
#include <QTextDocument>

namespace NovaPad::Search {

// ---------------------------------------------------------------------------
// Helper: build QTextDocument::FindFlags from our options
// ---------------------------------------------------------------------------

static QTextDocument::FindFlags buildFlags(const SearchHelpers::FindOptions &opts)
{
    QTextDocument::FindFlags flags;
    if (opts.caseSensitive) flags |= QTextDocument::FindCaseSensitively;
    if (opts.wholeWord)     flags |= QTextDocument::FindWholeWords;
    if (opts.backward)      flags |= QTextDocument::FindBackward;
    return flags;
}

// ---------------------------------------------------------------------------
// findInEditor
// ---------------------------------------------------------------------------

bool SearchHelpers::findInEditor(NovaPad::Editor::CodeEditorWidget *editor,
                                 const QString &term,
                                 const FindOptions &opts)
{
    if (!editor || term.isEmpty()) return false;

    QTextDocument *doc = editor->document();
    QTextCursor cursor = editor->textCursor();
    QTextCursor found;

    if (opts.regex) {
        QRegularExpression re(
            term,
            opts.caseSensitive ? QRegularExpression::NoPatternOption
                               : QRegularExpression::CaseInsensitiveOption);
        if (!re.isValid()) return false;
        found = doc->find(re, cursor, buildFlags(opts));
    } else {
        found = doc->find(term, cursor, buildFlags(opts));
    }

    // Wrap around if enabled and no match was found
    if (found.isNull() && opts.wrapAround) {
        QTextCursor wrapped(doc);
        if (opts.backward) {
            wrapped.movePosition(QTextCursor::End);
        } else {
            wrapped.movePosition(QTextCursor::Start);
        }

        if (opts.regex) {
            QRegularExpression re(
                term,
                opts.caseSensitive ? QRegularExpression::NoPatternOption
                                   : QRegularExpression::CaseInsensitiveOption);
            found = doc->find(re, wrapped, buildFlags(opts));
        } else {
            found = doc->find(term, wrapped, buildFlags(opts));
        }
    }

    if (!found.isNull()) {
        editor->setTextCursor(found);
        editor->centerCursor();
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------
// replaceAllInEditor
// ---------------------------------------------------------------------------

int SearchHelpers::replaceAllInEditor(NovaPad::Editor::CodeEditorWidget *editor,
                                      const QString &searchTerm,
                                      const QString &replaceTerm,
                                      const FindOptions &opts)
{
    if (!editor || searchTerm.isEmpty()) return 0;

    QTextDocument *doc = editor->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);

    int count = 0;
    cursor.beginEditBlock();

    // Flags without backward (we always scan forward for replace-all)
    QTextDocument::FindFlags flags;
    if (opts.caseSensitive) flags |= QTextDocument::FindCaseSensitively;
    if (opts.wholeWord)     flags |= QTextDocument::FindWholeWords;

    while (true) {
        QTextCursor found;

        if (opts.regex) {
            QRegularExpression re(
                searchTerm,
                opts.caseSensitive ? QRegularExpression::NoPatternOption
                                   : QRegularExpression::CaseInsensitiveOption);
            found = doc->find(re, cursor, flags);
        } else {
            found = doc->find(searchTerm, cursor, flags);
        }

        if (found.isNull()) break;

        found.insertText(replaceTerm);
        cursor = found;
        ++count;
    }

    cursor.endEditBlock();
    return count;
}

// ---------------------------------------------------------------------------
// countInEditor
// ---------------------------------------------------------------------------

int SearchHelpers::countInEditor(NovaPad::Editor::CodeEditorWidget *editor,
                                 const QString &term,
                                 const FindOptions &opts)
{
    if (!editor || term.isEmpty()) return 0;

    QTextDocument *doc = editor->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);

    QTextDocument::FindFlags flags;
    if (opts.caseSensitive) flags |= QTextDocument::FindCaseSensitively;
    if (opts.wholeWord)     flags |= QTextDocument::FindWholeWords;

    int count = 0;

    while (true) {
        QTextCursor found;

        if (opts.regex) {
            QRegularExpression re(
                term,
                opts.caseSensitive ? QRegularExpression::NoPatternOption
                                   : QRegularExpression::CaseInsensitiveOption);
            found = doc->find(re, cursor, flags);
        } else {
            found = doc->find(term, cursor, flags);
        }

        if (found.isNull()) break;

        ++count;
        cursor = found;
    }

    return count;
}

} // namespace NovaPad::Search
