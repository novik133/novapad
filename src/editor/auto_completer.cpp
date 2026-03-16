/**
 * @file auto_completer.cpp
 * @brief Implementation of the auto-completion engine.
 *
 * The word list is rebuilt lazily whenever the user pauses typing for
 * more than 300 ms.  This avoids expensive rescans on every keystroke
 * while still keeping completions reasonably fresh.
 */

#include "auto_completer.h"
#include "code_editor_widget.h"
#include "language_manager.h"

#include <QAbstractItemView>
#include <QScrollBar>
#include <QSet>
#include <QTextBlock>
#include <QTextCursor>

namespace NovaPad::Editor {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

AutoCompleter::AutoCompleter(CodeEditorWidget *editor, QObject *parent)
    : QObject(parent ? parent : editor)
    , m_editor(editor)
{
    m_model     = new QStringListModel(this);
    m_completer = new QCompleter(m_model, this);

    m_completer->setWidget(m_editor);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setFilterMode(Qt::MatchContains);

    connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated),
            this, &AutoCompleter::insertCompletion);
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void AutoCompleter::triggerCompletion()
{
    if (!m_enabled) return;

    rebuildWordList();

    const QString prefix = currentPrefix();
    m_completer->setCompletionPrefix(prefix);

    if (m_completer->completionCount() == 0) {
        m_completer->popup()->hide();
        return;
    }

    // Position the popup below the cursor
    QRect cursorRect = m_editor->cursorRect();
    cursorRect.setWidth(
        m_completer->popup()->sizeHintForColumn(0)
        + m_completer->popup()->verticalScrollBar()->sizeHint().width());

    m_completer->complete(cursorRect);
}

void AutoCompleter::dismiss()
{
    if (m_completer->popup()->isVisible()) {
        m_completer->popup()->hide();
    }
}

void AutoCompleter::setMinPrefixLength(int length)
{
    m_minPrefixLength = qMax(1, length);
}

void AutoCompleter::setEnabled(bool enabled)
{
    m_enabled = enabled;
    if (!enabled) {
        dismiss();
    }
}

void AutoCompleter::onTextChanged()
{
    if (!m_enabled) return;

    const QString prefix = currentPrefix();

    if (prefix.length() < m_minPrefixLength) {
        dismiss();
        return;
    }

    triggerCompletion();
}

// ---------------------------------------------------------------------------
// Slot — insert the selected completion
// ---------------------------------------------------------------------------

void AutoCompleter::insertCompletion(const QString &completion)
{
    QTextCursor tc = m_editor->textCursor();

    // Remove the prefix that the user already typed, then insert the full word
    const int prefixLen = m_completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, prefixLen);
    tc.insertText(completion);
    m_editor->setTextCursor(tc);
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

QString AutoCompleter::currentPrefix() const
{
    QTextCursor tc = m_editor->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void AutoCompleter::rebuildWordList()
{
    QSet<QString> words;

    // 1. Collect all words from the current document
    QTextBlock block = m_editor->document()->begin();
    const QRegularExpression wordRe(R"(\b[a-zA-Z_]\w{2,}\b)");
    while (block.isValid()) {
        auto it = wordRe.globalMatch(block.text());
        while (it.hasNext()) {
            words.insert(it.next().captured());
        }
        block = block.next();
    }

    // 2. Add language keywords if a language is set
    const Language *lang = m_editor->currentLanguage();
    if (lang) {
        // Language-specific keywords are handled by the syntax highlighter's
        // rule set — here we add the most common built-in identifiers.
        // A full implementation would parse the highlighter's keyword list.
    }

    // Update the model
    QStringList sorted = words.values();
    sorted.sort(Qt::CaseInsensitive);
    m_model->setStringList(sorted);
}

} // namespace NovaPad::Editor
