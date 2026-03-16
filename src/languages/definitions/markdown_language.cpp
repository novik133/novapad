/**
 * @file markdown_language.cpp
 * @brief Implementation of Markdown language support.
 */

#include "markdown_language.h"

namespace NovaPad::Languages {

std::vector<TokenRule> MarkdownLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // Headers (# to ######)
    QTextCharFormat h1Format;
    h1Format.setForeground(QColor{"#DC143C"});  // Crimson
    h1Format.setFontPointSize(16);
    h1Format.setFontWeight(QFont::Bold);
    rules.emplace_back("h1",
        QRegularExpression{R"(^#[ \t]+.*$)"},
        h1Format, false, 100);
    
    QTextCharFormat h2Format;
    h2Format.setForeground(QColor{"#4169E1"});  // Royal blue
    h2Format.setFontPointSize(14);
    h2Format.setFontWeight(QFont::Bold);
    rules.emplace_back("h2",
        QRegularExpression{R"(^##[ \t]+.*$)"},
        h2Format, false, 98);
    
    QTextCharFormat h3Format;
    h3Format.setForeground(QColor{"#2E8B57"});  // Sea green
    h3Format.setFontPointSize(12);
    h3Format.setFontWeight(QFont::Bold);
    rules.emplace_back("h3",
        QRegularExpression{R"(^###[ \t]+.*$)"},
        h3Format, false, 96);
    
    QTextCharFormat h4Format;
    h4Format.setForeground(QColor{"#9932CC"});  // Dark orchid
    h4Format.setFontWeight(QFont::Bold);
    rules.emplace_back("h4",
        QRegularExpression{R"(^####[ \t]+.*$)"},
        h4Format, false, 94);
    
    QTextCharFormat h5Format;
    h5Format.setForeground(QColor{"#FF8C00"});  // Dark orange
    h5Format.setFontWeight(QFont::Bold);
    rules.emplace_back("h5",
        QRegularExpression{R"(^#####[ \t]+.*$)"},
        h5Format, false, 92);
    
    QTextCharFormat h6Format;
    h6Format.setForeground(QColor{"#A9A9A9"});  // Dark gray
    h6Format.setFontWeight(QFont::Bold);
    rules.emplace_back("h6",
        QRegularExpression{R"(^######[ \t]+.*$)"},
        h6Format, false, 90);
    
    // Fenced code blocks ```...```
    QTextCharFormat codeBlockFormat;
    codeBlockFormat.setForeground(QColor{"#808080"});
    codeBlockFormat.setBackground(QColor{"#F5F5F5"});
    rules.emplace_back("fenced_code",
        QRegularExpression{R"(^```[\s\S]*?^```)"},
        codeBlockFormat, true, 88);
    
    // Inline code `...`
    QTextCharFormat inlineCodeFormat;
    inlineCodeFormat.setForeground(QColor{"#B22222"});
    inlineCodeFormat.setBackground(QColor{"#F5F5F5"});
    rules.emplace_back("inline_code",
        QRegularExpression{R"(`[^`]+`)"},
        inlineCodeFormat, false, 86);
    
    // Bold **...** or __...__
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("bold",
        QRegularExpression{R"(\*\*[\s\S]*?\*\*|__[\s\S]*?__)"},
        boldFormat, false, 85);
    
    // Italic *...* or _..._
    QTextCharFormat italicFormat;
    italicFormat.setFontItalic(true);
    rules.emplace_back("italic",
        QRegularExpression{R"(\*[\s\S]*?\*|_[\s\S]*?_)"},
        italicFormat, false, 84);
    
    // Strikethrough ~~...~~
    QTextCharFormat strikeFormat;
    strikeFormat.setFontStrikeOut(true);
    rules.emplace_back("strikethrough",
        QRegularExpression{R"(~~[^~]+~~)"},
        strikeFormat, false, 82);
    
    // Links [text](url) and [text][ref]
    QTextCharFormat linkFormat;
    linkFormat.setForeground(QColor{"#1E90FF"});
    linkFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    rules.emplace_back("link",
        QRegularExpression{R"(\[[^\]]+\](?:\([^)]+\)|\[[^\]]*\]))"},
        linkFormat, false, 80);
    
    // Images ![alt](url)
    QTextCharFormat imageFormat;
    imageFormat.setForeground(QColor{"#9370DB"});
    imageFormat.setFontItalic(true);
    rules.emplace_back("image",
        QRegularExpression{R"(!\[[^\]]*\]\([^)]+\))"},
        imageFormat, false, 78);
    
    // Blockquotes >...
    QTextCharFormat quoteFormat;
    quoteFormat.setForeground(QColor{"#6A5ACD"});  // Slate blue
    quoteFormat.setBackground(QColor{"#F0F8FF"});
    rules.emplace_back("blockquote",
        QRegularExpression{R"(^>[ \t>]*.*$)"},
        quoteFormat, false, 76);
    
    // Lists (-, *, +, 1.)
    QTextCharFormat listFormat;
    listFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("list",
        QRegularExpression{R"(^\s*(?:[-*+]|\d+\.)[ \t])"},
        listFormat, false, 74);
    
    // Task lists - [x] or - [ ]
    QTextCharFormat taskFormat;
    taskFormat.setForeground(QColor{"#FF6347"});
    rules.emplace_back("task",
        QRegularExpression{R"(^\s*[-*+][ \t]+\[[xX ]\])"},
        taskFormat, false, 73);
    
    // Tables |...|
    QTextCharFormat tableFormat;
    tableFormat.setForeground(QColor{"#4682B4"});
    rules.emplace_back("table",
        QRegularExpression{R"(^\|[^\n]*\|)"},
        tableFormat, false, 72);
    
    // Table separator |---|---|
    QTextCharFormat tableSepFormat;
    tableSepFormat.setForeground(QColor{"#A9A9A9"});
    rules.emplace_back("table_sep",
        QRegularExpression{R"(^\|[-:| \t]+\|)"},
        tableSepFormat, false, 71);
    
    // Horizontal rule --- or *** or ___
    QTextCharFormat hrFormat;
    hrFormat.setForeground(QColor{"#D3D3D3"});
    rules.emplace_back("hr",
        QRegularExpression{R"(^[-*_]{3,}\s*$)"},
        hrFormat, false, 70);
    
    // Footnotes [^1] or [^note]
    QTextCharFormat footnoteFormat;
    footnoteFormat.setForeground(QColor{"#DAA520"});  // Goldenrod
    footnoteFormat.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    rules.emplace_back("footnote",
        QRegularExpression{R"(\[\^[\w-]+\])"},
        footnoteFormat, false, 68);
    
    // Emojis :emoji_name:
    QTextCharFormat emojiFormat;
    emojiFormat.setFontPointSize(14);
    rules.emplace_back("emoji",
        QRegularExpression{R"(:[a-z_]+:)"},
        emojiFormat, false, 66);
    
    // HTML tags in Markdown
    QTextCharFormat htmlFormat;
    htmlFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("html",
        QRegularExpression{R"(<[a-zA-Z][^>]*>)"},
        htmlFormat, false, 65);
    
    return rules;
}

CommentRule MarkdownLanguageSupport::commentRule() const
{
    // Markdown doesn't have standard comments, but we can use HTML comments
    return CommentRule({}, "<!--", "-->", false);
}

IndentationRule MarkdownLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;
    rule.smartIndent = true;
    rule.indentAfterColon = false;
    rule.indentTriggers = {"-", "*", "+", "1."};
    return rule;
}

} // namespace NovaPad::Languages
