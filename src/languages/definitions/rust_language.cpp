/**
 * @file rust_language.cpp
 * @brief Implementation of Rust language support.
 */

#include "rust_language.h"

namespace NovaPad::Languages {

QStringList RustLanguageSupport::keywords() const
{
    return {
        "as", "async", "await", "break", "const", "continue", "crate", "dyn",
        "else", "enum", "extern", "false", "fn", "for", "if", "impl", "in",
        "let", "loop", "match", "mod", "move", "mut", "pub", "ref", "return",
        "self", "Self", "static", "struct", "super", "trait", "true", "type",
        "unsafe", "use", "where", "while",
        // Reserved
        "abstract", "become", "box", "do", "final", "macro", "override", "priv",
        "try", "typeof", "unsized", "virtual", "yield"
    };
}

QStringList RustLanguageSupport::builtinTypes() const
{
    return {
        "bool", "char", "i8", "i16", "i32", "i64", "i128", "isize",
        "u8", "u16", "u32", "u64", "u128", "usize",
        "f32", "f64", "str", "String",
        "Vec", "HashMap", "HashSet", "BTreeMap", "BTreeSet",
        "Option", "Result", "Box", "Rc", "Arc", "RefCell", "Cell",
        "Mutex", "RwLock", "AtomicBool", "AtomicI32", "AtomicUsize",
        "Path", "PathBuf", "OsStr", "OsString"
    };
}

std::vector<BracePair> RustLanguageSupport::bracePairs() const
{
    return {
        BracePair('{', '}', BraceType::Curly, true, {}),
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {}),
        BracePair('<', '>', BraceType::Angle, true, {})  // Generics
    };
}

CommentRule RustLanguageSupport::commentRule() const
{
    return CommentRule("//", "/*", "*/", false);
}

QStringList RustLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "r\"", "r#\"", "b\"", "c\""};  // Regular, char, raw, raw with hashes, byte, C-string
}

IndentationRule RustLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"fn", "if", "else", "match", "for", "while", "loop", "impl", "struct", "enum", "trait", "mod"};
    rule.outdentTriggers = {"return", "break", "continue"};
    return rule;
}

std::vector<TokenRule> RustLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // Keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor{"#CC7A00"});  // Dark orange
    keywordFormat.setFontWeight(QFont::Bold);
    QString kw = keywords().join("|");
    rules.emplace_back("keyword",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(kw)},
        keywordFormat, false, 100);
    
    // Lifetimes 'a, 'static
    QTextCharFormat lifetimeFormat;
    lifetimeFormat.setForeground(QColor{"#B8860B"});  // Dark goldenrod
    rules.emplace_back("lifetime",
        QRegularExpression{R"('\w+)"},
        lifetimeFormat, false, 95);
    
    // Macros !macro_name!
    QTextCharFormat macroFormat;
    macroFormat.setForeground(QColor{"#8B4513"});  // Saddle brown
    macroFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("macro",
        QRegularExpression{R"(\b\w+!|\bmacro_rules!)"},
        macroFormat, false, 90);
    
    // Attributes #[...] and #![...]
    QTextCharFormat attrFormat;
    attrFormat.setForeground(QColor{"#808080"});  // Gray
    attrFormat.setFontItalic(true);
    rules.emplace_back("attribute",
        QRegularExpression{R"(#!?\[.*?\])"},
        attrFormat, false, 85);
    
    // Raw strings r"...", r#"..."#
    QTextCharFormat rawStringFormat;
    rawStringFormat.setForeground(QColor{"#228B22"});  // Forest green
    rules.emplace_back("raw_string",
        QRegularExpression{R"(r#*\".*?\"#*)"},
        rawStringFormat, true, 80);
    
    // Byte strings b"...", b'...'
    QTextCharFormat byteStringFormat;
    byteStringFormat.setForeground(QColor{"#6B8E23"});  // Olive drab
    rules.emplace_back("byte_string",
        QRegularExpression{R"(b\"[^\"]*\"|b'[^']*')"},
        byteStringFormat, false, 79);
    
    // Numbers with underscores and type suffixes
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("number",
        QRegularExpression{R"(\b(?:0[xob]?[0-9a-fA-F_]+|[0-9][0-9_]*)(?:\.[0-9_]+)?(?:[eE][+-]?[0-9_]+)?(?:[ui](?:8|16|32|64|128|size)|f(?:32|64))?\b)"},
        numberFormat, false, 70);
    
    // Types
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 75);
    
    // Operators
    QTextCharFormat opFormat;
    opFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("operator",
        QRegularExpression{R"([+\-*/%=<>!&|^~?]+)"},
        opFormat, false, 50);
    
    // Comments
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("line_comment",
        QRegularExpression{R"(//[^\n]*)"},
        commentFormat, false, 40);
    rules.emplace_back("block_comment",
        QRegularExpression{R"(/\*.*?\*/)"},
        commentFormat, true, 40);
    
    return rules;
}

std::vector<AutoCompleteTrigger> RustLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"::", true, 0, {"after_identifier"}},  // Path separator
        {"(", false, 0, {"after_identifier"}},
        {"<", false, 0, {"after_identifier"}},  // Generics
        {"|", true, 0, {"after_brace"}},  // Closures
        {"=>", true, 0, {"after_pattern"}},  // Match arms
        {"&", false, 0, {"after_operator"}},  // Reference
        {"mut ", false, 0, {"after_ampersand", "after_let"}}
    };
}

} // namespace NovaPad::Languages
