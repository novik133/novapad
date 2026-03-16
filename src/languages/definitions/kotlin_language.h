/**
 * @file kotlin_language.h
 * @brief Kotlin language support with modern features.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_KOTLIN_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_KOTLIN_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class KotlinLanguageSupport
 * @brief Kotlin language support with coroutines and multiplatform features.
 */
class KotlinLanguageSupport final : public CFamilyLanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "kotlin"; }
    [[nodiscard]] QString displayName() const override { return "Kotlin"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-kotlin"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"kt", "kts"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"build.gradle.kts", "settings.gradle.kts"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_KOTLIN_LANGUAGE_H
