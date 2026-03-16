/**
 * @file advanced_syntax_highlighter.h
 * @brief Enhanced syntax highlighter using the new language support framework.
 *
 * Integrates with the LanguageRegistry to provide language-aware syntax
 * highlighting for all supported programming languages. Replaces the
 * existing syntax_highlighter.cpp with a more flexible, rule-based system.
 */

#ifndef NOVAPAD_LANGUAGES_SUPPORT_ADVANCED_SYNTAX_HIGHLIGHTER_H
#define NOVAPAD_LANGUAGES_SUPPORT_ADVANCED_SYNTAX_HIGHLIGHTER_H

#include "languages/base/language_support_base.h"
#include "languages/support/language_registry.h"

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>

namespace NovaPad::Languages {

/**
 * @class AdvancedSyntaxHighlighter
 * @brief Language-aware syntax highlighter supporting 17+ programming languages.
 *
 * Uses the LanguageSupport interface to obtain language-specific token rules
 * and applies them to the document. Supports:
 * - Multi-line comments and strings
 * - Nested block states
 * - Dynamic rule reloading when language changes
 * - Performance-optimized batch highlighting
 */
class AdvancedSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit AdvancedSyntaxHighlighter(QTextDocument *parent = nullptr);
    ~AdvancedSyntaxHighlighter() override = default;

    /**
     * @brief Sets the language for syntax highlighting.
     * @param languageId Language identifier (e.g., "cpp", "python")
     */
    void setLanguage(const QString &languageId);

    /**
     * @brief Returns the current language ID.
     */
    [[nodiscard]] QString currentLanguage() const { return m_languageId; }

    /**
     * @brief Re-applies highlighting to the entire document.
     * Call after theme changes.
     */
    void rehighlightAll();

protected:
    /**
     * @brief Core highlighting callback for each text block.
     */
    void highlightBlock(const QString &text) override;

private:
    /**
     * @brief Applies token rules to the given text block.
     */
    void applyTokenRules(const QString &text);

    /**
     * @brief Handles multi-line constructs (block comments, strings).
     */
    void handleMultiLineConstructs(const QString &text);

    /**
     * @brief Updates the block state for multi-line tracking.
     */
    void updateBlockState(const QString &text, int state);

    QString m_languageId;
    const LanguageSupport *m_language = nullptr;
    
    // Cached formats for performance
    QMap<QString, QTextCharFormat> m_formatCache;
    
    // Block state tracking
    enum BlockState {
        StateNormal = 0,
        StateComment = 1,
        StateString = 2,
        StateHeredoc = 3
    };
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_SUPPORT_ADVANCED_SYNTAX_HIGHLIGHTER_H
