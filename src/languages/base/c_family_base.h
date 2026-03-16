/**
 * @file c_family_base.h
 * @brief Base class for C-family languages (C, C++, C#, Objective-C, Java, etc.).
 *
 * Provides common functionality for brace-delimited languages with C-style syntax:
 * - Curly brace blocks for scoping
 * - Line comments with slash-slash and block comments with slash-star
 * - Preprocessor directives (where applicable)
 * - Case-sensitive identifiers
 * - Similar operator sets
 */

#ifndef NOVAPAD_LANGUAGES_BASE_C_FAMILY_BASE_H
#define NOVAPAD_LANGUAGES_BASE_C_FAMILY_BASE_H

#include "language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class CFamilyLanguageSupport
 * @brief Abstract base for C-style languages sharing common syntax patterns.
 *
 * Implements common features like:
 * - Curly brace matching { }
 * - Parentheses matching ( )
 * - Square bracket matching [ ]
 * - Angle bracket matching < > (for templates/generics)
 * - Slash-slash and slash-star comment styles
 * - Preprocessor directive highlighting
 * - Standard indentation rules
 */
class CFamilyLanguageSupport : public LanguageSupport {
public:
    ~CFamilyLanguageSupport() override = default;
    
    // -- Brace Matching ----------------------------------------------------
    
    /**
     * @brief Returns standard C-family brace pairs.
     * 
     * Includes:
     * - Curly braces { } for blocks, classes, namespaces
     * - Round parentheses ( ) for expressions, function calls
     * - Square brackets [ ] for arrays, attributes, indexing
     * - Angle brackets < > for templates, generics, includes
     */
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    
    // -- Comments ----------------------------------------------------------
    
    /**
     * @brief Standard C-style comments: slash-slash for line, slash-star for block.
     * Block comments do not nest in standard C, C++, and C sharp.
     */
    [[nodiscard]] CommentRule commentRule() const override;
    
    /**
     * @brief Standard string delimiters: double and single quotes.
     * Derived classes may add raw string literals (C++ R"...", C# @"...").
     */
    [[nodiscard]] QStringList stringDelimiters() const override;
    
    // -- Indentation -------------------------------------------------------
    
    /**
     * @brief Standard C-family indentation:
     * - Indent after opening brace
     * - Indent after control flow keywords (if, for, while, etc.)
     * - Outdent on closing brace
     * - Default 4 spaces, no tabs
     */
    [[nodiscard]] IndentationRule indentationRule() const override;
    
    // -- Common Properties -------------------------------------------------
    
    [[nodiscard]] bool isCaseSensitive() const override { return true; }
    [[nodiscard]] bool hasPreprocessor() const override { return true; }
    [[nodiscard]] QString preprocessorPrefix() const override { return "#"; }
    
protected:
    /**
     * @brief Builds common C-family token rules (keywords, operators, numbers).
     * Derived classes call this and add language-specific rules.
     */
    [[nodiscard]] std::vector<TokenRule> buildCommonRules() const;
    
    /**
     * @brief Returns common C-family control flow keywords.
     */
    [[nodiscard]] static QStringList controlFlowKeywords();
    
    /**
     * @brief Returns common C-family type keywords.
     */
    [[nodiscard]] static QStringList typeKeywords();
    
    /**
     * @brief Returns common C-family storage class specifiers.
     */
    [[nodiscard]] static QStringList storageKeywords();
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_BASE_C_FAMILY_BASE_H
