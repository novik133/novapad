/**
 * @file cpp_language.h
 * @brief C++ language support with C++23 features.
 *
 * Comprehensive support for modern C++ including:
 * - C++11/14/17/20/23 keywords and features
 * - Template syntax highlighting
 * - Raw string literals (R"...")
 * - User-defined literals
 * - Attributes [[...]]
 * - Concepts and constraints
 * - Modules (import/export)
 * - Coroutines (co_await, co_yield, co_return)
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_CPP_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_CPP_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class CppLanguageSupport
 * @brief Complete C++ language support with modern C++23 features.
 *
 * Handles:
 * - All C++ keywords including C++11/14/17/20/23 additions
 * - Template syntax with angle bracket matching
 * - Raw string literals (R"delimiter(...)delimiter")
 * - User-defined literals (123_km, "hello"s)
 * - Attributes [[nodiscard]], [[likely]], etc.
 * - Module syntax (import, export, module)
 * - Coroutines and concepts
 */
class CppLanguageSupport final : public CFamilyLanguageSupport {
public:
    // -- Identity ----------------------------------------------------------
    [[nodiscard]] QString id() const override { return "cpp"; }
    [[nodiscard]] QString displayName() const override { return "C++"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-c++src"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"cpp", "cc", "cxx", "c++", "hpp", "hh", "hxx", "h++", 
                "inl", "tpp", "ipp"};
    }
    
    [[nodiscard]] QStringList fileNames() const override { return {}; }
    
    // -- Syntax Highlighting ----------------------------------------------
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    
    // -- C++ Specific Features ---------------------------------------------
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    
    /**
     * @brief Returns C++ standard library types and classes.
     */
    [[nodiscard]] QStringList standardLibraryTypes() const;
    
    /**
     * @brief Returns C++ standard library functions.
     */
    [[nodiscard]] QStringList standardLibraryFunctions() const;
    
private:
    /**
     * @brief Adds template-specific rules (typename, template, concept).
     */
    void addTemplateRules(std::vector<TokenRule> &rules) const;
    
    /**
     * @brief Adds C++11/14/17/20/23 specific keyword rules.
     */
    void addModernCppRules(std::vector<TokenRule> &rules) const;
    
    /**
     * @brief Adds raw string literal highlighting.
     */
    void addRawStringRules(std::vector<TokenRule> &rules) const;
    
    /**
     * @brief Adds attribute highlighting ([[...]]).
     */
    void addAttributeRules(std::vector<TokenRule> &rules) const;
    
    /**
     * @brief Adds user-defined literal highlighting.
     */
    void addUserDefinedLiteralRules(std::vector<TokenRule> &rules) const;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_CPP_LANGUAGE_H
