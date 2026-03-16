/**
 * @file bracket_matcher.h
 * @brief Bracket / delimiter matching engine for the code editor.
 *
 * Highlights matching pairs of brackets, parentheses, and braces as
 * the cursor moves.  Supports nested constructs and configurable
 * delimiter sets.
 */

#ifndef NOVAPAD_EDITOR_BRACKET_MATCHER_H
#define NOVAPAD_EDITOR_BRACKET_MATCHER_H

#include <QList>
#include <QObject>
#include <QTextEdit>

namespace NovaPad::Editor {

class CodeEditorWidget; // forward declaration

/**
 * @class BracketMatcher
 * @brief Scans the document around the cursor and highlights matching brackets.
 *
 * Call highlightMatchingBrackets() from the editor's cursor-changed handler
 * to append extra selections for the matched pair.
 */
class BracketMatcher : public QObject
{
    Q_OBJECT

public:
    explicit BracketMatcher(CodeEditorWidget *editor, QObject *parent = nullptr);
    ~BracketMatcher() override = default;

    /**
     * @brief Appends highlight selections for matching brackets to the list.
     * @param[in,out] selections  The editor's extra-selection list to extend.
     */
    void highlightMatchingBrackets(QList<QTextEdit::ExtraSelection> &selections);

private:
    /** @brief Returns true if @p ch is an opening bracket. */
    [[nodiscard]] static bool isOpenBracket(QChar ch);

    /** @brief Returns true if @p ch is a closing bracket. */
    [[nodiscard]] static bool isCloseBracket(QChar ch);

    /** @brief Returns the matching counterpart for the given bracket character. */
    [[nodiscard]] static QChar matchingBracket(QChar ch);

    /**
     * @brief Scans forward from @p pos looking for the closing bracket.
     * @param pos    Starting position in the document.
     * @param open   The opening bracket character.
     * @param close  The closing bracket character.
     * @return Position of the match, or -1 if not found.
     */
    [[nodiscard]] int findForward(int pos, QChar open, QChar close) const;

    /**
     * @brief Scans backward from @p pos looking for the opening bracket.
     * @param pos    Starting position in the document.
     * @param open   The opening bracket character.
     * @param close  The closing bracket character.
     * @return Position of the match, or -1 if not found.
     */
    [[nodiscard]] int findBackward(int pos, QChar open, QChar close) const;

    CodeEditorWidget *m_editor = nullptr;
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_BRACKET_MATCHER_H
