/**
 * @file bracket_matcher.cpp
 * @brief Implementation of the bracket-matching engine.
 *
 * The matcher supports the standard delimiter pairs: (), [], {}.
 * It performs a linear scan in the appropriate direction, maintaining a
 * nesting counter to correctly handle nested constructs.  The scan is
 * bounded to 10 000 characters to avoid blocking on very large files.
 */

#include "bracket_matcher.h"
#include "code_editor_widget.h"

#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

namespace NovaPad::Editor {

// Maximum distance (in characters) to scan when looking for a match.
// This prevents the UI from freezing on enormous files.
static constexpr int kMaxScanDistance = 10'000;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

BracketMatcher::BracketMatcher(CodeEditorWidget *editor, QObject *parent)
    : QObject(parent ? parent : editor)
    , m_editor(editor)
{
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void BracketMatcher::highlightMatchingBrackets(
    QList<QTextEdit::ExtraSelection> &selections)
{
    if (!m_editor) return;

    const QTextCursor cursor = m_editor->textCursor();
    const QTextDocument *doc = m_editor->document();
    const int pos = cursor.position();

    // Inspect the character at and before the cursor position
    auto tryMatch = [&](int charPos) -> bool {
        if (charPos < 0 || charPos >= doc->characterCount()) return false;

        const QChar ch = doc->characterAt(charPos);
        int matchPos = -1;

        if (isOpenBracket(ch)) {
            matchPos = findForward(charPos + 1, ch, matchingBracket(ch));
        } else if (isCloseBracket(ch)) {
            matchPos = findBackward(charPos - 1, matchingBracket(ch), ch);
        } else {
            return false;
        }

        if (matchPos < 0) return false;

        // Highlight both brackets with a subtle background colour
        const QColor matchColor(80, 120, 200, 80);

        auto makeSelection = [&](int p) {
            QTextEdit::ExtraSelection sel;
            sel.format.setBackground(matchColor);
            QTextCursor tc(const_cast<QTextDocument*>(doc));
            tc.setPosition(p);
            tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            sel.cursor = tc;
            return sel;
        };

        selections.append(makeSelection(charPos));
        selections.append(makeSelection(matchPos));
        return true;
    };

    // Try the character at cursor position first, then the one before it
    if (!tryMatch(pos)) {
        tryMatch(pos - 1);
    }
}

// ---------------------------------------------------------------------------
// Bracket classification
// ---------------------------------------------------------------------------

bool BracketMatcher::isOpenBracket(QChar ch)
{
    return ch == QLatin1Char('(')
        || ch == QLatin1Char('[')
        || ch == QLatin1Char('{');
}

bool BracketMatcher::isCloseBracket(QChar ch)
{
    return ch == QLatin1Char(')')
        || ch == QLatin1Char(']')
        || ch == QLatin1Char('}');
}

QChar BracketMatcher::matchingBracket(QChar ch)
{
    switch (ch.unicode()) {
    case '(': return QLatin1Char(')');
    case ')': return QLatin1Char('(');
    case '[': return QLatin1Char(']');
    case ']': return QLatin1Char('[');
    case '{': return QLatin1Char('}');
    case '}': return QLatin1Char('{');
    default:  return QChar();
    }
}

// ---------------------------------------------------------------------------
// Scanning helpers
// ---------------------------------------------------------------------------

int BracketMatcher::findForward(int pos, QChar open, QChar close) const
{
    const QTextDocument *doc = m_editor->document();
    const int end = qMin(pos + kMaxScanDistance, doc->characterCount());
    int depth = 1;

    for (int i = pos; i < end; ++i) {
        const QChar ch = doc->characterAt(i);
        if (ch == open)  ++depth;
        if (ch == close) --depth;
        if (depth == 0) return i;
    }
    return -1;
}

int BracketMatcher::findBackward(int pos, QChar open, QChar close) const
{
    const QTextDocument *doc = m_editor->document();
    const int begin = qMax(pos - kMaxScanDistance, 0);
    int depth = 1;

    for (int i = pos; i >= begin; --i) {
        const QChar ch = doc->characterAt(i);
        if (ch == close) ++depth;
        if (ch == open)  --depth;
        if (depth == 0) return i;
    }
    return -1;
}

} // namespace NovaPad::Editor
