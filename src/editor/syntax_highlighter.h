/**
 * @file syntax_highlighter.h
 * @brief Native syntax highlighting engine for NovaPad.
 *
 * Provides keyword-based and regex-based highlighting for 50+ programming
 * languages.  Rules are loaded from the LanguageManager and applied via
 * QSyntaxHighlighter, which hooks directly into QTextDocument — no
 * external browser engine required.
 */

#ifndef NOVAPAD_EDITOR_SYNTAX_HIGHLIGHTER_H
#define NOVAPAD_EDITOR_SYNTAX_HIGHLIGHTER_H

#include "editor/language_manager.h"

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

#include <vector>

namespace NovaPad::Editor {

/**
 * @struct HighlightRule
 * @brief A single pattern → format mapping used by the highlighter.
 */
struct HighlightRule
{
    QRegularExpression pattern;
    QTextCharFormat    format;
};

/**
 * @class SyntaxHighlighter
 * @brief QSyntaxHighlighter subclass that applies language-aware colouring.
 *
 * When a Language is assigned via setLanguage(), the highlighter rebuilds
 * its internal rule set.  Multi-line constructs (block comments, heredocs)
 * are handled through QSyntaxHighlighter's block-state mechanism.
 */
class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
    ~SyntaxHighlighter() override = default;

    /**
     * @brief Rebuilds the rule set for the given language.
     * @param language  Pointer obtained from LanguageManager (may be nullptr to clear).
     */
    void setLanguage(const Language *language);

    /**
     * @brief Sets the colour scheme used for token categories.
     *
     * Call this when the user switches themes at runtime.
     */
    void setColorScheme(const QMap<QString, QTextCharFormat> &scheme);

protected:
    /**
     * @brief Core highlighting callback invoked by Qt for every text block.
     * @param text  The text of the current block.
     */
    void highlightBlock(const QString &text) override;

private:
    /** @brief Builds generic keyword/operator rules from the Language definition. */
    void buildRulesForLanguage(const Language *language);

    /** @brief Adds common rules shared across most C-family languages. */
    void addCFamilyRules();

    /** @brief Adds Python-specific rules (decorators, f-strings, etc.). */
    void addPythonRules();

    /** @brief Adds web-language rules (HTML tags, CSS selectors, etc.). */
    void addWebRules();

    /** @brief Adds shell-script rules (variables, here-docs, etc.). */
    void addShellRules();

    /** @brief Adds SQL rules. */
    void addSqlRules();

    /** @brief Adds Rust-specific rules (lifetimes, macros, attributes). */
    void addRustRules();

    /** @brief Adds Go-specific rules. */
    void addGoRules();

    // -- Rule storage --------------------------------------------------------
    std::vector<HighlightRule> m_rules;

    // -- Multi-line comment delimiters ---------------------------------------
    QRegularExpression m_commentStartExpr;
    QRegularExpression m_commentEndExpr;

    // -- Reusable formats (populated from the colour scheme) -----------------
    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_typeFormat;
    QTextCharFormat m_stringFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_numberFormat;
    QTextCharFormat m_functionFormat;
    QTextCharFormat m_preprocessorFormat;
    QTextCharFormat m_operatorFormat;
    QTextCharFormat m_annotationFormat;
    QTextCharFormat m_variableFormat;
    QTextCharFormat m_tagFormat;
    QTextCharFormat m_attributeFormat;

    /** @brief Initialises the default colour scheme (dark-friendly). */
    void initDefaultFormats();

    const Language *m_currentLanguage = nullptr;
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_SYNTAX_HIGHLIGHTER_H
