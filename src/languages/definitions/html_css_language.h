/**
 * @file html_css_language.h
 * @brief HTML, CSS, and JSON markup language support.
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_HTML_CSS_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_HTML_CSS_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class HtmlLanguageSupport
 * @brief HTML/XHTML language support.
 */
class HtmlLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "html"; }
    [[nodiscard]] QString displayName() const override { return "HTML"; }
    [[nodiscard]] QString mimeType() const override { return "text/html"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"html", "htm", "xhtml", "shtml"};
    }
    
    [[nodiscard]] QStringList fileNames() const override { return {}; }
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override { return {}; }
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override { return {"\"", "'"}; }
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override { return {}; }
};

/**
 * @class CssLanguageSupport
 * @brief CSS/SCSS/LESS language support.
 */
class CssLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "css"; }
    [[nodiscard]] QString displayName() const override { return "CSS/SCSS"; }
    [[nodiscard]] QString mimeType() const override { return "text/css"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"css", "scss", "sass", "less", "styl"};
    }
    
    [[nodiscard]] QStringList fileNames() const override { return {}; }
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override { return {}; }
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override { return {"\"", "'"}; }
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override { return {}; }
};

/**
 * @class JsonLanguageSupport
 * @brief JSON language support.
 */
class JsonLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "json"; }
    [[nodiscard]] QString displayName() const override { return "JSON"; }
    [[nodiscard]] QString mimeType() const override { return "application/json"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"json", "jsonc", "json5"};
    }
    
    [[nodiscard]] QStringList fileNames() const override {
        return {"package.json", "composer.json", "tsconfig.json", "jsconfig.json"};
    }
    
    [[nodiscard]] QStringList keywords() const override { return {}; }
    [[nodiscard]] QStringList builtinTypes() const override { return {}; }
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override { return CommentRule("//", "/*", "*/"); }
    [[nodiscard]] QStringList stringDelimiters() const override { return {"\""}; }
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override { return {}; }
};

/**
 * @class XmlLanguageSupport
 * @brief XML language support.
 */
class XmlLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "xml"; }
    [[nodiscard]] QString displayName() const override { return "XML"; }
    [[nodiscard]] QString mimeType() const override { return "text/xml"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"xml", "xsl", "xslt", "svg", "rss", "atom", "wsdl", "xsd"};
    }
    
    [[nodiscard]] QStringList fileNames() const override { return {}; }
    [[nodiscard]] QStringList keywords() const override { return {}; }
    [[nodiscard]] QStringList builtinTypes() const override { return {}; }
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override { return {"\"", "'"}; }
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override { return {}; }
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_HTML_CSS_LANGUAGE_H
