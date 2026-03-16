/**
 * @file html_css_language.cpp
 * @brief Implementation of HTML, CSS, JSON, and XML language support.
 */

#include "html_css_language.h"

namespace NovaPad::Languages {

// =============================================================================
// HTML Language Support
// =============================================================================

QStringList HtmlLanguageSupport::keywords() const
{
    return {
        "!DOCTYPE", "a", "abbr", "address", "area", "article", "aside",
        "audio", "b", "base", "bdi", "bdo", "blockquote", "body", "br",
        "button", "canvas", "caption", "cite", "code", "col", "colgroup",
        "data", "datalist", "dd", "del", "details", "dfn", "dialog", "div",
        "dl", "dt", "em", "embed", "fieldset", "figcaption", "figure",
        "footer", "form", "h1", "h2", "h3", "h4", "h5", "h6", "head",
        "header", "hgroup", "hr", "html", "i", "iframe", "img", "input",
        "ins", "kbd", "label", "legend", "li", "link", "main", "map",
        "mark", "math", "menu", "meta", "meter", "nav", "noscript", "object",
        "ol", "optgroup", "option", "output", "p", "param", "picture",
        "pre", "progress", "q", "rp", "rt", "ruby", "s", "samp", "script",
        "search", "section", "select", "slot", "small", "source", "span",
        "strong", "style", "sub", "summary", "sup", "svg", "table", "tbody",
        "td", "template", "textarea", "tfoot", "th", "thead", "time", "title",
        "tr", "track", "u", "ul", "var", "video", "wbr"
    };
}

std::vector<TokenRule> HtmlLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // DOCTYPE declaration
    QTextCharFormat doctypeFormat;
    doctypeFormat.setForeground(QColor{"#B22222"});
    doctypeFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("doctype",
        QRegularExpression{R"(<!DOCTYPE[^>]*>)"},
        doctypeFormat, false, 100);
    
    // HTML tags
    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor{"#1E90FF"});
    tagFormat.setFontWeight(QFont::Bold);
    QString kw = keywords().join("|");
    rules.emplace_back("tag",
        QRegularExpression{QString{R"(<\??/?(?:%1)\b)"}.arg(kw)},
        tagFormat, false, 95);
    
    // Generic tag names
    rules.emplace_back("generic_tag",
        QRegularExpression{R"(<\??/?[a-zA-Z][a-zA-Z0-9-]*)"},
        tagFormat, false, 94);
    
    // Attributes
    QTextCharFormat attrFormat;
    attrFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("attribute",
        QRegularExpression{R"(\s[a-zA-Z-]+=)"},
        attrFormat, false, 90);
    
    // Attribute values
    QTextCharFormat attrValueFormat;
    attrValueFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("attribute_value",
        QRegularExpression{R"(\"[^"]*\"|'[^']*')"},
        attrValueFormat, false, 85);
    
    // Comments <!-- -->
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("comment",
        QRegularExpression{R"(<!--[\s\S]*?-->)"},
        commentFormat, true, 80);
    
    // Entities &...;
    QTextCharFormat entityFormat;
    entityFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("entity",
        QRegularExpression{R"(&[a-zA-Z][a-zA-Z0-9]*;|&#[0-9]+;|&#x[0-9a-fA-F]+;)"},
        entityFormat, false, 75);
    
    // CDATA sections
    QTextCharFormat cdataFormat;
    cdataFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("cdata",
        QRegularExpression{R"(<!\[CDATA\[[\s\S]*?\]\]>)"},
        cdataFormat, true, 70);
    
    return rules;
}

std::vector<BracePair> HtmlLanguageSupport::bracePairs() const
{
    return {
        BracePair('<', '>', BraceType::XmlTag, true, {}),
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {}),
        BracePair('"', '"', BraceType::TripleQuote, false, "\\"),
        BracePair('\'', '\'', BraceType::TripleQuote, false, "\\")
    };
}

CommentRule HtmlLanguageSupport::commentRule() const
{
    return CommentRule({}, "<!--", "-->", false);
}

IndentationRule HtmlLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;
    rule.smartIndent = true;
    rule.indentAfterBrace = false;
    QStringList tags = keywords();
    tags.removeAll("br");
    tags.removeAll("hr");
    tags.removeAll("img");
    tags.removeAll("input");
    tags.removeAll("meta");
    tags.removeAll("link");
    rule.indentTriggers = tags;
    return rule;
}

// =============================================================================
// CSS Language Support
// =============================================================================

QStringList CssLanguageSupport::keywords() const
{
    return {
        // At-rules
        "@charset", "@import", "@namespace", "@media", "@supports", "@keyframes",
        "@font-face", "@page", "@document", "@viewport", "@counter-style",
        "@property", "@layer", "@container",
        // SCSS specific
        "@mixin", "@include", "@function", "@return", "@extend", "@if", "@else",
        "@for", "@each", "@while", "@debug", "@warn", "@error", "@content",
        "@at-root", "@use", "@forward"
    };
}

std::vector<TokenRule> CssLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // At-rules @...
    QTextCharFormat atRuleFormat;
    atRuleFormat.setForeground(QColor{"#B22222"});
    atRuleFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("atrule",
        QRegularExpression{R"(@[a-zA-Z-]+)"},
        atRuleFormat, false, 100);
    
    // CSS selectors
    QTextCharFormat selectorFormat;
    selectorFormat.setForeground(QColor{"#1E90FF"});
    rules.emplace_back("selector",
        QRegularExpression{R"((?:\.|#)[a-zA-Z][a-zA-Z0-9_-]*|\[.*\])"},
        selectorFormat, false, 95);
    
    // Tag selectors
    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor{"#9370DB"});
    rules.emplace_back("tag",
        QRegularExpression{R"(\b(?:html|body|div|span|h[1-6]|p|a|img|ul|ol|li|table|tr|td|th|form|input|button|script|style|link|meta|head|header|footer|nav|section|article|aside|main|figure|figcaption|canvas|video|audio|source|iframe|object|embed|param)\b)"},
        tagFormat, false, 93);
    
    // Properties
    QTextCharFormat propFormat;
    propFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("property",
        QRegularExpression{R"(\b(?:color|background|border|margin|padding|display|position|float|clear|width|height|font|text|line|vertical|overflow|visibility|opacity|z-index|cursor|content|transform|transition|animation|flex|grid|gap|justify|align|box-shadow|text-shadow|border-radius)\w*\s*:)"},
        propFormat, false, 90);
    
    // Values with units
    QTextCharFormat valueFormat;
    valueFormat.setForeground(QColor{"#2E8B57"});
    rules.emplace_back("value",
        QRegularExpression{R"(\b(?:[0-9]+(?:\.[0-9]+)?(?:px|em|rem|%|vh|vw|vmin|vmax|pt|pc|cm|mm|in|ex|ch|deg|rad|turn|s|ms|Hz|kHz|dpi|dpcm|dppx)?|#[0-9a-fA-F]{3,8}|rgba?\([^)]*\)|hsla?\([^)]*\)|var\([^)]*\))\b)"},
        valueFormat, false, 85);
    
    // Pseudo-classes and pseudo-elements
    QTextCharFormat pseudoFormat;
    pseudoFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("pseudo",
        QRegularExpression{R"((?::?:)[a-zA-Z-]+(?:\([^)]*\))?)"},
        pseudoFormat, false, 80);
    
    // SCSS variables $var
    QTextCharFormat scssVarFormat;
    scssVarFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("scss_var",
        QRegularExpression{R"(\$[a-zA-Z][a-zA-Z0-9_-]*)"},
        scssVarFormat, false, 78);
    
    // CSS variables --var
    QTextCharFormat cssVarFormat;
    cssVarFormat.setForeground(QColor{"#FF6347"});
    rules.emplace_back("css_var",
        QRegularExpression{R"(--[a-zA-Z][a-zA-Z0-9-]*)"},
        cssVarFormat, false, 77);
    
    // Functions
    QTextCharFormat funcFormat;
    funcFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("function",
        QRegularExpression{R"(\b(?:calc|min|max|clamp|var|env|url|rgb|rgba|hsl|hsla|linear-gradient|radial-gradient|conic-gradient|repeating-linear-gradient|repeating-radial-gradient|repeating-conic-gradient|cubic-bezier|steps|matrix|translate|rotate|scale|skew|perspective)\s*\()"},
        funcFormat, false, 75);
    
    // Comments /* */
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("comment",
        QRegularExpression{R"(/\*[\s\S]*?\*/)"},
        commentFormat, true, 70);
    
    // Important !important
    QTextCharFormat importantFormat;
    importantFormat.setForeground(QColor{"#FF0000"});
    importantFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("important",
        QRegularExpression{R"(!important)"},
        importantFormat, false, 65);
    
    return rules;
}

std::vector<BracePair> CssLanguageSupport::bracePairs() const
{
    return {
        BracePair('{', '}', BraceType::Curly, true, {}),
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {})
    };
}

CommentRule CssLanguageSupport::commentRule() const
{
    return CommentRule({}, "/*", "*/", false);
}

IndentationRule CssLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    return rule;
}

// =============================================================================
// JSON Language Support
// =============================================================================

std::vector<TokenRule> JsonLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // Keys
    QTextCharFormat keyFormat;
    keyFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("key",
        QRegularExpression{R"("(?:\\.|[^"\\])*"\s*:)"},
        keyFormat, false, 100);
    
    // Strings
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("string",
        QRegularExpression{R"("(?:\\.|[^"\\])*")"},
        stringFormat, false, 95);
    
    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("number",
        QRegularExpression{R"(\-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?)"},
        numberFormat, false, 90);
    
    // Constants
    QTextCharFormat constFormat;
    constFormat.setForeground(QColor{"#1E90FF"});
    constFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("constant",
        QRegularExpression{R"(\b(?:true|false|null)\b)"},
        constFormat, false, 85);
    
    // Comments (JSON5/JSONC)
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("line_comment",
        QRegularExpression{R"(//[^\n]*)"},
        commentFormat, false, 40);
    rules.emplace_back("block_comment",
        QRegularExpression{R"(/\*[\s\S]*?\*/)"},
        commentFormat, true, 40);
    
    return rules;
}

std::vector<BracePair> JsonLanguageSupport::bracePairs() const
{
    return {
        BracePair('{', '}', BraceType::Curly, true, {}),
        BracePair('[', ']', BraceType::Square, true, {})
    };
}

// =============================================================================
// XML Language Support
// =============================================================================

std::vector<TokenRule> XmlLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // XML declaration
    QTextCharFormat xmlDeclFormat;
    xmlDeclFormat.setForeground(QColor{"#B22222"});
    xmlDeclFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("xml_decl",
        QRegularExpression{R"(<\?xml[^?]*\?>)"},
        xmlDeclFormat, false, 100);
    
    // Processing instructions
    QTextCharFormat piFormat;
    piFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("pi",
        QRegularExpression{R"(<\?[a-zA-Z][^?]*\?>)"},
        piFormat, false, 95);
    
    // Tags
    QTextCharFormat tagFormat;
    tagFormat.setForeground(QColor{"#1E90FF"});
    tagFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("tag",
        QRegularExpression{R"(<\??/?[a-zA-Z][a-zA-Z0-9:-]*)"},
        tagFormat, false, 90);
    
    // Attributes
    QTextCharFormat attrFormat;
    attrFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("attribute",
        QRegularExpression{R"(\s[a-zA-Z:][a-zA-Z0-9:-]*=)"},
        attrFormat, false, 85);
    
    // Attribute values
    QTextCharFormat attrValueFormat;
    attrValueFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("attr_value",
        QRegularExpression{R"("[^"]*"|'[^']*')"},
        attrValueFormat, false, 80);
    
    // Comments
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("comment",
        QRegularExpression{R"(<!--[\s\S]*?-->)"},
        commentFormat, true, 75);
    
    // CDATA
    QTextCharFormat cdataFormat;
    cdataFormat.setForeground(QColor{"#2E8B57"});
    rules.emplace_back("cdata",
        QRegularExpression{R"(<!\[CDATA\[[\s\S]*?\]\]>)"},
        cdataFormat, true, 70);
    
    // DOCTYPE
    QTextCharFormat doctypeFormat;
    doctypeFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("doctype",
        QRegularExpression{R"(<!DOCTYPE[^>]*>)"},
        doctypeFormat, false, 65);
    
    // Entities
    QTextCharFormat entityFormat;
    entityFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("entity",
        QRegularExpression{R"(&[a-zA-Z][a-zA-Z0-9]*;|&#[0-9]+;|&#x[0-9a-fA-F]+;)"},
        entityFormat, false, 60);
    
    // Namespaces
    QTextCharFormat nsFormat;
    nsFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("namespace",
        QRegularExpression{R"(\bxmlns(?::\w+)?=)"},
        nsFormat, false, 55);
    
    return rules;
}

std::vector<BracePair> XmlLanguageSupport::bracePairs() const
{
    return {
        BracePair('<', '>', BraceType::XmlTag, true, {}),
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {})
    };
}

CommentRule XmlLanguageSupport::commentRule() const
{
    return CommentRule({}, "<!--", "-->", false);
}

IndentationRule JsonLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    return rule;
}

IndentationRule XmlLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;
    rule.smartIndent = true;
    rule.indentAfterBrace = false;
    return rule;
}

} // namespace NovaPad::Languages
