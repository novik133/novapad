/**
 * @file php_language.h
 * @brief PHP language support (PHP 8.2+).
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_PHP_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_PHP_LANGUAGE_H

#include "languages/base/c_family_base.h"

namespace NovaPad::Languages {

/**
 * @class PhpLanguageSupport
 * @brief PHP language support with modern features.
 *
 * Handles:
 * - PHP 8 attributes #[...]
 * - Named arguments
 * - Union types
 * - Match expressions
 * - Nullsafe operator ?->
 * - Heredoc/Nowdoc
 */
class PhpLanguageSupport final : public CFamilyLanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "php"; }
    [[nodiscard]] QString displayName() const override { return "PHP"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-php"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"php", "php3", "php4", "php5", "phtml", "phps"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"composer.json", "phpunit.xml", ".php_cs", ".php-cs-fixer.php"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    [[nodiscard]] QStringList shebangPatterns() const override;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_PHP_LANGUAGE_H
