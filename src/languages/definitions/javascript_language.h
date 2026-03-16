/**
 * @file javascript_language.h
 * @brief JavaScript and TypeScript language support (ES2023/ESNext).
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_JAVASCRIPT_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_JAVASCRIPT_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class JavaScriptLanguageSupport
 * @brief JavaScript/TypeScript language support with modern ES2023 features.
 *
 * Handles:
 * - ES6+ features: async/await, arrow functions, classes, modules
 * - TypeScript: types, interfaces, generics, decorators
 * - JSX/TSX for React
 * - Template literals `...`
 * - Optional chaining ?., nullish coalescing ??
 */
class JavaScriptLanguageSupport final : public CFamilyLanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "javascript"; }
    [[nodiscard]] QString displayName() const override { return "JavaScript/TypeScript"; }
    [[nodiscard]] QString mimeType() const override { return "text/javascript"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"js", "jsx", "ts", "tsx", "mjs", "cjs", "es6", "es"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"package.json", "tsconfig.json", "jsconfig.json", ".eslintrc", ".babelrc"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    [[nodiscard]] bool isCaseSensitive() const override { return true; }
    
    [[nodiscard]] bool isTypeScript(const QString &filePath) const;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_JAVASCRIPT_LANGUAGE_H
