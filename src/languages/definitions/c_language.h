/**
 * @file c_language.h
 * @brief C language support (C89/C99/C11/C17/C23).
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_C_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_C_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class CLanguageSupport
 * @brief Standard C language support with modern C11/C17/C23 features.
 */
class CLanguageSupport final : public CFamilyLanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "c"; }
    [[nodiscard]] QString displayName() const override { return "C"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-csrc"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"c", "h"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {};  // No specific filenames for C
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    
    [[nodiscard]] QStringList shebangPatterns() const override { return {}; }
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_C_LANGUAGE_H
