/**
 * @file perl_language.h
 * @brief Perl language support.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_PERL_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_PERL_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class PerlLanguageSupport
 * @brief Perl 5 language support with modern features.
 */
class PerlLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "perl"; }
    [[nodiscard]] QString displayName() const override { return "Perl"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-perl"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"pl", "pm", "t", "pod"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"Makefile.PL", "Build.PL", "cpanfile"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override { return {}; }
    [[nodiscard]] QStringList shebangPatterns() const override;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_PERL_LANGUAGE_H
