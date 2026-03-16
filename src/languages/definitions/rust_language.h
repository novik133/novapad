/**
 * @file rust_language.h
 * @brief Rust language support with modern features.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_RUST_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_RUST_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class RustLanguageSupport
 * @brief Rust language support with ownership, lifetimes, and macros.
 */
class RustLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "rust"; }
    [[nodiscard]] QString displayName() const override { return "Rust"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-rust"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"rs"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"Cargo.toml", "rust-toolchain", "rustfmt.toml"};
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

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_RUST_LANGUAGE_H
