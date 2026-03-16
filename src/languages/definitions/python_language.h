/**
 * @file python_language.h
 * @brief Python language support (Python 3.10+).
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_PYTHON_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_PYTHON_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class PythonLanguageSupport
 * @brief Python 3 language support with modern features.
 *
 * Handles:
 * - Indentation-based folding (no braces)
 * - f-strings, raw strings, byte strings
 * - Type hints and annotations
 * - Decorators @...
 * - Match/case (Python 3.10+)
 * - Walrus operator :=
 */
class PythonLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "python"; }
    [[nodiscard]] QString displayName() const override { return "Python"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-python"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"py", "pyw", "pyi", "pyc", "pyo"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"requirements.txt", "setup.py", "pyproject.toml", "Pipfile"};
    }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    [[nodiscard]] bool usesIndentBasedFolding() const override { return true; }
    [[nodiscard]] QStringList shebangPatterns() const override;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_PYTHON_LANGUAGE_H
