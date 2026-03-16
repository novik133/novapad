/**
 * @file swift_language.h
 * @brief Swift language support (Swift 5.9+).
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_SWIFT_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_SWIFT_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class SwiftLanguageSupport
 * @brief Swift language support with modern features.
 *
 * Handles:
 * - Optionals ? and !
 * - Property observers willSet/didSet
 * - Result builders (@ViewBuilder)
 * - async/await
 * - Property wrappers (@State, @Binding)
 * - Generics and associated types
 */
class SwiftLanguageSupport final : public CFamilyLanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "swift"; }
    [[nodiscard]] QString displayName() const override { return "Swift"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-swift"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"swift"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"Package.swift"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_SWIFT_LANGUAGE_H
