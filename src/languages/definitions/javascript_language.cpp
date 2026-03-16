/**
 * @file javascript_language.cpp
 * @brief Implementation of JavaScript/TypeScript language support.
 */

#include "javascript_language.h"

namespace NovaPad::Languages {

QStringList JavaScriptLanguageSupport::keywords() const
{
    return {
        // ECMAScript keywords
        "break", "case", "catch", "class", "const", "continue", "debugger",
        "default", "delete", "do", "else", "export", "extends", "finally",
        "for", "function", "if", "import", "in", "instanceof", "new", "return",
        "super", "switch", "this", "throw", "try", "typeof", "var", "void",
        "while", "with", "yield",
        
        // ES5+ strict mode reserved
        "implements", "interface", "let", "package", "private", "protected",
        "public", "static",
        
        // ES6+
        "async", "await",
        
        // TypeScript specific (treated as keywords)
        "type", "interface", "namespace", "module", "declare", "abstract",
        "readonly", "enum", "infer", "is", "keyof", "unique", "infer",
        "satisfies", "using", "from"
    };
}

QStringList JavaScriptLanguageSupport::builtinTypes() const
{
    return {
        // Primitives
        "string", "number", "boolean", "bigint", "symbol", "undefined", "null",
        "any", "unknown", "never", "void", "object",
        
        // Built-in objects
        "Array", "Date", "Error", "Function", "JSON", "Math", "Number",
        "Object", "RegExp", "String", "Boolean", "Promise", "Map", "Set",
        "WeakMap", "WeakSet", "Proxy", "Reflect", "Symbol", "Intl",
        "ArrayBuffer", "SharedArrayBuffer", "Uint8Array", "Int8Array",
        "Uint16Array", "Int16Array", "Uint32Array", "Int32Array",
        "Float32Array", "Float64Array", "DataView",
        "Generator", "AsyncGenerator", "Iterator", "AsyncIterator",
        "Record", "Partial", "Required", "Readonly", "Pick", "Omit",
        "Exclude", "Extract", "NonNullable", "Parameters", "ReturnType",
        "InstanceType", "ThisParameterType", "OmitThisParameter", "ThisType"
    };
}

QStringList JavaScriptLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "`"};  // Template literals with backticks
}

bool JavaScriptLanguageSupport::isTypeScript(const QString &filePath) const
{
    return filePath.endsWith(".ts") || filePath.endsWith(".tsx");
}

IndentationRule JavaScriptLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;  // JavaScript standard is 2 spaces
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"if", "else", "for", "while", "do", "switch", "case",
                          "try", "catch", "finally", "class", "function", "=>",
                          "async", "await"};
    rule.outdentTriggers = {"return", "break", "continue", "throw"};
    return rule;
}

std::vector<TokenRule> JavaScriptLanguageSupport::tokenRules() const
{
    auto rules = buildCommonRules();
    
    // ES6+ keywords
    QTextCharFormat es6Format;
    es6Format.setForeground(QColor{"#FF1493"});  // Deep pink
    rules.emplace_back("es6",
        QRegularExpression{R"(\b(?:async|await|class|const|let|yield|import|export|default|extends|super|get|set|static|typeof|instanceof|in|of)\b)"},
        es6Format, false, 95);
    
    // Template literals `...`
    QTextCharFormat templateFormat;
    templateFormat.setForeground(QColor{"#20B2AA"});
    rules.emplace_back("template",
        QRegularExpression{R"(`(?:[^`\\]|\\[^])*`)"},
        templateFormat, true, 90);
    
    // Template literal interpolation ${...}
    QTextCharFormat interpolationFormat;
    interpolationFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("interpolation",
        QRegularExpression{R"(\$\{[^}]*\})"},
        interpolationFormat, false, 88);
    
    // Arrow functions =>
    QTextCharFormat arrowFormat;
    arrowFormat.setForeground(QColor{"#9370DB"});
    arrowFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("arrow",
        QRegularExpression{R"(=>)"},
        arrowFormat, false, 85);
    
    // TypeScript types
    QTextCharFormat tsTypeFormat;
    tsTypeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("ts_type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types)},
        tsTypeFormat, false, 80);
    
    // Function declarations
    QTextCharFormat funcFormat;
    funcFormat.setForeground(QColor{"#800080"});
    rules.emplace_back("function",
        QRegularExpression{R"(\b(?:function|const|let|var)\s+(\w+)\s*(?:=|:|\())"},
        funcFormat, false, 78);
    
    // JSX/TSX tags
    QTextCharFormat jsxTagFormat;
    jsxTagFormat.setForeground(QColor{"#1E90FF"});
    rules.emplace_back("jsx_tag",
        QRegularExpression{R"(<[a-zA-Z][a-zA-Z0-9]*)"},
        jsxTagFormat, false, 75);
    
    // JSX attributes
    QTextCharFormat jsxAttrFormat;
    jsxAttrFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("jsx_attr",
        QRegularExpression{R"(\b[a-z][a-zA-Z0-9]*=)"},
        jsxAttrFormat, false, 72);
    
    // TypeScript decorators @...
    QTextCharFormat decoratorFormat;
    decoratorFormat.setForeground(QColor{"#A52A2A"});
    decoratorFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("decorator",
        QRegularExpression{R"(@\w+(?:\.\w+)*)"},
        decoratorFormat, false, 70);
    
    // TypeScript interfaces
    QTextCharFormat interfaceFormat;
    interfaceFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("interface",
        QRegularExpression{R"(\binterface\s+(\w+))"},
        interfaceFormat, false, 68);
    
    // Type annotations : type
    QTextCharFormat typeAnnoFormat;
    typeAnnoFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("type_annotation",
        QRegularExpression{R"(:\s*[A-Z][a-zA-Z0-9_<>, ]*)"},
        typeAnnoFormat, false, 65);
    
    // Generics <T>
    QTextCharFormat genericFormat;
    genericFormat.setForeground(QColor{"#DDA0DD"});
    rules.emplace_back("generic",
        QRegularExpression{R"(<[A-Z][a-zA-Z0-9_<>, ]*>)"},
        genericFormat, false, 63);
    
    // Optional chaining ?. and nullish coalescing ??
    QTextCharFormat optionalFormat;
    optionalFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("optional",
        QRegularExpression{R"(\?\.|\?\?)"},
        optionalFormat, false, 60);
    
    // Regular expressions
    QTextCharFormat regexFormat;
    regexFormat.setForeground(QColor{"#FF6347"});
    rules.emplace_back("regex",
        QRegularExpression{R"(/[^/\\]*(?:\\.[^/\\]*)*/[gimsuy]*)"},
        regexFormat, false, 55);
    
    // JSDoc comments /** ... */
    QTextCharFormat jsdocFormat;
    jsdocFormat.setForeground(QColor{"#4682B4"});
    jsdocFormat.setFontItalic(true);
    rules.emplace_back("jsdoc",
        QRegularExpression{R"(/\*\*[\s\S]*?\*/)"},
        jsdocFormat, true, 45);
    
    return rules;
}

std::vector<AutoCompleteTrigger> JavaScriptLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"?.", true, 0, {"after_identifier"}},  // Optional chaining
        {"??", true, 0, {"after_identifier"}},  // Nullish coalescing
        {"(", false, 0, {"after_identifier"}},
        {"`", false, 0, {"after_operator"}},  // Template literal
        {"${", true, 0, {"in_template"}},      // Interpolation
        {":", false, 0, {"after_property"}},  // Type annotation
        {"=>", true, 0, {"after_parameters"}},  // Arrow function
        {"<", false, 0, {"after_identifier"}}  // Generic
    };
}

} // namespace NovaPad::Languages
