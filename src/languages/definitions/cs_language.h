/**
 * @file cs_language.h
 * @brief C# language support with .NET features.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_CS_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_CS_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class CsLanguageSupport
 * @brief C# language support with modern .NET features.
 *
 * Handles:
 * - C# 5.0+ async/await
 * - C# 6.0+ expression-bodied members, interpolated strings
 * - C# 7.0+ tuples, pattern matching
 * - C# 8.0+ nullable reference types, switch expressions
 * - C# 9.0+ records, init-only properties
 * - C# 10.0+ global usings, file-scoped namespaces
 * - C# 11.0+ raw string literals, generic attributes
 * - LINQ syntax highlighting
 * - Verbatim and interpolated strings
 * - Attributes and generics
 */
class CsLanguageSupport final : public CFamilyLanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "csharp"; }
    [[nodiscard]] QString displayName() const override { return "C#"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-csharp"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"cs", "csx"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {};  // No specific filenames for C#
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    
    [[nodiscard]] QStringList standardLibraryTypes() const;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_CS_LANGUAGE_H
