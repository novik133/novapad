/**
 * @file perl_language.cpp
 * @brief Implementation of Perl language support.
 */

#include "perl_language.h"

namespace NovaPad::Languages {

QStringList PerlLanguageSupport::keywords() const
{
    return {
        "continue", "else", "elsif", "for", "foreach", "if", "last", "my",
        "next", "no", "our", "package", "return", "sub", "unless", "until",
        "use", "while", "given", "when", "default", "say", "state",
        // Special literals
        "__DATA__", "__END__", "__FILE__", "__LINE__", "__PACKAGE__",
        // Builtin functions (contextual keywords)
        "print", "printf", "sprintf", "chomp", "chop", "chr", "crypt",
        "hex", "index", "lc", "lcfirst", "length", "oct", "ord", "pack",
        "q", "qq", "qx", "qw", "qr", "quotemeta", "reverse", "rindex",
        "split", "sprintf", "substr", "tr", "y", "uc", "ucfirst",
        "pos", "study"
    };
}

QStringList PerlLanguageSupport::builtinTypes() const
{
    return {
        "$", "@", "%", "&", "*",  // Sigils
        "scalar", "array", "hash", "code", "glob", "ref"
    };
}

QStringList PerlLanguageSupport::shebangPatterns() const
{
    return {
        "#!/usr/bin/env perl",
        "#!/usr/bin/perl",
        "#!/usr/local/bin/perl"
    };
}

std::vector<BracePair> PerlLanguageSupport::bracePairs() const
{
    return {
        BracePair('{', '}', BraceType::Curly, true, {}),
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {}),
        BracePair('<', '>', BraceType::Angle, true, {})
    };
}

CommentRule PerlLanguageSupport::commentRule() const
{
    return CommentRule("#", {}, {}, false);
}

QStringList PerlLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "`", "q{", "qq{", "qx{", "qw{", "qr{", "m{", "s{"};
}

IndentationRule PerlLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"sub", "if", "elsif", "else", "unless", "for", "foreach",
                          "while", "until", "given", "when", "default", "package"};
    rule.outdentTriggers = {"return", "last", "next", "redo"};
    return rule;
}

std::vector<TokenRule> PerlLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // Keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor{"#0000FF"});
    keywordFormat.setFontWeight(QFont::Bold);
    QString kw = keywords().join("|");
    rules.emplace_back("keyword",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(kw)},
        keywordFormat, false, 100);
    
    // Variables with sigils
    QTextCharFormat varFormat;
    varFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("scalar",
        QRegularExpression{R"(\$\w+)"},
        varFormat, false, 95);
    rules.emplace_back("array",
        QRegularExpression{R"(@\w+)"},
        varFormat, false, 94);
    rules.emplace_back("hash",
        QRegularExpression{R"(%\w+)"},
        varFormat, false, 93);
    
    // Special variables ($_, $@, $!, etc.)
    QTextCharFormat specialFormat;
    specialFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("special_var",
        QRegularExpression{R"(\$[_!@/?]|\$\d+|\$[&`'+])"},
        specialFormat, false, 92);
    
    // Quoting constructs q{}, qq{}, qw{}, etc.
    QTextCharFormat quoteFormat;
    quoteFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("quote",
        QRegularExpression{R"(\b(?:q|qq|qx|qw|qr|m|s|tr|y)\s*[^a-zA-Z0-9])"},
        quoteFormat, false, 90);
    
    // Regular expressions /.../ and m/.../
    QTextCharFormat regexFormat;
    regexFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("regex",
        QRegularExpression{R"(\b(?:m|s|tr|y)\s*/[^/]*/[a-z]*|/[^/]*/[a-z]*)"},
        regexFormat, false, 88);
    
    // POD documentation
    QTextCharFormat podFormat;
    podFormat.setForeground(QColor{"#808080"});
    podFormat.setFontItalic(true);
    rules.emplace_back("pod",
        QRegularExpression{R"(^=[a-zA-Z][a-zA-Z0-9_]*.*?^=cut)"},
        podFormat, true, 85);
    
    // Here-doc <<EOF
    QTextCharFormat heredocFormat;
    heredocFormat.setForeground(QColor{"#20B2AA"});
    rules.emplace_back("heredoc",
        QRegularExpression{R"(<<['"]?\w+['"]?[\s\S]*?^\w+;)"},
        heredocFormat, true, 83);
    
    // Strings
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor{"#008000"});
    rules.emplace_back("string_double",
        QRegularExpression{R"("(?:[^"\\]|\\.)*")"},
        stringFormat, false, 80);
    rules.emplace_back("string_single",
        QRegularExpression{R"('(?:[^'\\]|\\.)*')"},
        stringFormat, false, 79);
    
    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("number",
        QRegularExpression{R"(\b(?:0[xX][0-9a-fA-F_]+|0[oO]?[0-7_]+|0[bB][01_]+|[0-9_]+(?:\.[0-9_]+)?(?:[eE][+-]?[0-9_]+)?)\b)"},
        numberFormat, false, 75);
    
    // Subroutine declarations
    QTextCharFormat subFormat;
    subFormat.setForeground(QColor{"#800080"});
    rules.emplace_back("sub",
        QRegularExpression{R"(\bsub\s+(\w+))"},
        subFormat, false, 72);
    
    // Package declarations
    QTextCharFormat packageFormat;
    packageFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("package",
        QRegularExpression{R"(\bpackage\s+(\w+(?:::\w+)*)\b)"},
        packageFormat, false, 70);
    
    // Use/require statements
    QTextCharFormat useFormat;
    useFormat.setForeground(QColor{"#A52A2A"});
    rules.emplace_back("use",
        QRegularExpression{R"(\b(?:use|require|no)\s+(\w+(?:::\w+)*)\b)"},
        useFormat, false, 68);
    
    // Comments #...
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("comment",
        QRegularExpression{R"(#[^\n]*)"},
        commentFormat, false, 40);
    
    return rules;
}

} // namespace NovaPad::Languages
