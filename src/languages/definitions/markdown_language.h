/**
 * @file markdown_language.h
 * @brief Markdown language support.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_MARKDOWN_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_MARKDOWN_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class MarkdownLanguageSupport
 * @brief Markdown/CommonMark language support.
 *
 * Supports:
 * - Headers (# to ######)
 * - Emphasis (*, _, **, __)
 * - Code blocks (indented and fenced)
 * - Links [text](url)
 * - Images ![alt](url)
 * - Lists (-, *, +, 1.)
 * - Blockquotes (>
 * - Tables (|)
 * - Task lists (- [x])
 * - Strikethrough (~~)
 * - Footnotes ([^1])
 */
class MarkdownLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "markdown"; }
    [[nodiscard]] QString displayName() const override { return "Markdown"; }
    [[nodiscard]] QString mimeType() const override { return "text/markdown"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"md", "mkd", "markdown", "mdown", "mdwn"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"README.md", "CHANGELOG.md", "CONTRIBUTING.md", "LICENSE.md"};
    }
    
    [[nodiscard]] QStringList keywords() const override { return {}; }
    [[nodiscard]] QStringList builtinTypes() const override { return {}; }
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override { return {}; }
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override { return {}; }
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override { return {}; }
    [[nodiscard]] bool usesIndentBasedFolding() const override { return true; }
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_MARKDOWN_LANGUAGE_H
