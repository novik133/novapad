/**
 * @file go_language.h
 * @brief Go language support with Go 1.18+ generics.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_GO_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_GO_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class GoLanguageSupport
 * @brief Go language support with modern features including generics.
 */
class GoLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "go"; }
    [[nodiscard]] QString displayName() const override { return "Go"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-go"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"go"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"go.mod", "go.sum", "go.work"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    [[nodiscard]] bool isCaseSensitive() const override { return true; }
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_GO_LANGUAGE_H
