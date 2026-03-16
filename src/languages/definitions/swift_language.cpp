/**
 * @file swift_language.cpp
 * @brief Implementation of Swift language support.
 */

#include "swift_language.h"

namespace NovaPad::Languages {

QStringList SwiftLanguageSupport::keywords() const
{
    return {
        "associatedtype", "class", "deinit", "enum", "extension", "fileprivate",
        "func", "import", "init", "inout", "internal", "let", "open", "operator",
        "private", "protocol", "public", "rethrows", "static", "struct", "subscript",
        "typealias", "var", "break", "case", "continue", "default", "defer",
        "do", "else", "fallthrough", "for", "guard", "if", "in", "repeat",
        "return", "switch", "where", "while", "as", "Any", "catch", "false",
        "is", "nil", "self", "Self", "super", "throw", "throws", "true", "try",
        // Contextual keywords
        "associativity", "convenience", "didSet", "dynamic", "final", "get",
        "indirect", "infix", "lazy", "left", "mutating", "none", "nonmutating",
        "optional", "override", "postfix", "precedence", "prefix", "Protocol",
        "required", "right", "set", "some", "Type", "unowned", "weak", "willSet",
        // Swift 5.5+
        "async", "await", "actor", "isolated", "nonisolated", "sendable",
        "yield", "discard", "consume", "borrow", "copy"
    };
}

QStringList SwiftLanguageSupport::builtinTypes() const
{
    return {
        "Int", "Int8", "Int16", "Int32", "Int64",
        "UInt", "UInt8", "UInt16", "UInt32", "UInt64",
        "Float", "Double", "Float80", "Bool", "Character",
        "String", "Substring", "UnicodeScalar", "Unicode.UTF8",
        "Unicode.UTF16", "Unicode.UTF32", "Unicode.Scalar",
        "Optional", "ImplicitlyUnwrappedOptional", "Array", "Set", "Dictionary",
        "Range", "ClosedRange", "PartialRangeFrom", "PartialRangeThrough",
        "PartialRangeUpTo", "CountableRange", "CountableClosedRange",
        "Any", "AnyObject", "AnyClass", "AnyHashable",
        "Error", "CustomStringConvertible", "CustomDebugStringConvertible",
        "Equatable", "Comparable", "Hashable", "Codable", "Encodable", "Decodable",
        "Sendable", "Actor", "MainActor", "GlobalActor",
        "Sequence", "IteratorProtocol", "Collection", "BidirectionalCollection",
        "RandomAccessCollection", "MutableCollection", "RangeReplaceableCollection"
    };
}

QStringList SwiftLanguageSupport::stringDelimiters() const
{
    return {"\"", "\"\"\"", "'"};
}

IndentationRule SwiftLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 4;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"func", "class", "struct", "enum", "extension", "protocol",
                          "if", "else", "guard", "for", "while", "do", "catch",
                          "switch", "case"};
    rule.outdentTriggers = {"return", "break", "continue", "fallthrough", "throw"};
    return rule;
}

std::vector<TokenRule> SwiftLanguageSupport::tokenRules() const
{
    auto rules = buildCommonRules();
    
    // Swift-specific keywords
    QTextCharFormat swiftFormat;
    swiftFormat.setForeground(QColor{"#F05138"});  // Swift orange
    rules.emplace_back("swift",
        QRegularExpression{R"(\b(?:let|var|func|class|struct|enum|protocol|extension|guard|defer|async|await|actor|some|any)\b)"},
        swiftFormat, false, 95);
    
    // Multi-line strings """..."""
    QTextCharFormat multilineFormat;
    multilineFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("multiline_string",
        QRegularExpression{R"("""[\s\S]*?""")"},
        multilineFormat, true, 90);
    
    // String interpolation \(expr)
    QTextCharFormat interpFormat;
    interpFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("interpolation",
        QRegularExpression{R"(\\\([^)]*\))"},
        interpFormat, false, 88);
    
    // Optional types ? and !
    QTextCharFormat optionalFormat;
    optionalFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("optional",
        QRegularExpression{R"(\?|!)"},
        optionalFormat, false, 85);
    
    // Property wrappers @...
    QTextCharFormat wrapperFormat;
    wrapperFormat.setForeground(QColor{"#9932CC"});
    wrapperFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("property_wrapper",
        QRegularExpression{R"(@\w+)"},
        wrapperFormat, false, 80);
    
    // Result builders @ViewBuilder
    QTextCharFormat builderFormat;
    builderFormat.setForeground(QColor{"#FF69B4"});
    rules.emplace_back("result_builder",
        QRegularExpression{R"(@[A-Z][a-zA-Z]*Builder)"},
        builderFormat, false, 78);
    
    // Swift types
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        typeFormat, false, 75);
    
    // Function declarations
    QTextCharFormat funcFormat;
    funcFormat.setForeground(QColor{"#800080"});
    rules.emplace_back("function",
        QRegularExpression{R"(\bfunc\s+(\w+))"},
        funcFormat, false, 72);
    
    // Generic constraints
    QTextCharFormat genericFormat;
    genericFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("generic",
        QRegularExpression{R"(<[^>]*>)"},
        genericFormat, false, 70);
    
    // Access control
    QTextCharFormat accessFormat;
    accessFormat.setForeground(QColor{"#A9A9A9"});
    rules.emplace_back("access",
        QRegularExpression{R"(\b(?:open|public|internal|fileprivate|private)\b)"},
        accessFormat, false, 68);
    
    // Attributes @available, @objc, etc.
    QTextCharFormat attrFormat;
    attrFormat.setForeground(QColor{"#808080"});
    attrFormat.setFontItalic(true);
    rules.emplace_back("attribute",
        QRegularExpression{R"(@[a-zA-Z][a-zA-Z0-9]*(?:\([^)]*\))?)"},
        attrFormat, false, 65);
    
    // Raw strings #"..."#
    QTextCharFormat rawFormat;
    rawFormat.setForeground(QColor{"#20B2AA"});
    rules.emplace_back("raw_string",
        QRegularExpression{R"(#+\"[^"]*\"#+)"},
        rawFormat, false, 63);
    
    // Range operators ... and ..<
    QTextCharFormat rangeFormat;
    rangeFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("range",
        QRegularExpression{R"(\.\.\.|\.\.<)"},
        rangeFormat, false, 60);
    
    // Nil coalescing ??
    QTextCharFormat nilFormat;
    nilFormat.setForeground(QColor{"#9370DB"});
    rules.emplace_back("nil_coalescing",
        QRegularExpression{R"(\?\?)"},
        nilFormat, false, 58);
    
    // Keywords with self and super
    QTextCharFormat selfFormat;
    selfFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("self",
        QRegularExpression{R"(\b(?:self|Self|super)\b)"},
        selfFormat, false, 55);
    
    return rules;
}

std::vector<AutoCompleteTrigger> SwiftLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"?.", true, 0, {"after_identifier"}},
        {"??", true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {"{", false, 0, {"after_arrow"}},
        {"<", false, 0, {"after_identifier"}},
        {"\(", true, 0, {"in_string"}},  // String interpolation
        {"as", false, 0, {"after_identifier"}},
        {"is", false, 0, {"after_identifier"}}
    };
}

} // namespace NovaPad::Languages
