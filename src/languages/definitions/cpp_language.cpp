/**
 * @file cpp_language.cpp
 * @brief Implementation of C++ language support.
 */

#include "cpp_language.h"

namespace NovaPad::Languages {

QStringList CppLanguageSupport::keywords() const
{
    return {
        // C++98/03 keywords
        "asm", "auto", "break", "case", "catch", "class", "const", "const_cast",
        "continue", "default", "delete", "do", "double", "dynamic_cast", "else",
        "enum", "explicit", "export", "extern", "false", "float", "for", "friend",
        "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
        "operator", "private", "protected", "public", "register",
        "reinterpret_cast", "return", "short", "signed", "sizeof", "static",
        "static_cast", "struct", "switch", "template", "this", "throw", "true",
        "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
        "virtual", "void", "volatile", "wchar_t", "while",
        
        // C++11 keywords
        "alignas", "alignof", "char16_t", "char32_t", "constexpr", "decltype",
        "noexcept", "nullptr", "static_assert", "thread_local",
        
        // C++20 keywords
        "concept", "consteval", "constinit", "co_await", "co_return", "co_yield",
        "requires", "export"  // export is also a module keyword
    };
}

QStringList CppLanguageSupport::builtinTypes() const
{
    return {
        "bool", "char", "char8_t", "char16_t", "char32_t", "double", "float",
        "int", "long", "short", "signed", "unsigned", "void", "wchar_t",
        "size_t", "ptrdiff_t", "nullptr_t", "max_align_t", "byte"
    };
}

QStringList CppLanguageSupport::standardLibraryTypes() const
{
    return {
        // Containers
        "array", "vector", "deque", "list", "forward_list", "set", "multiset",
        "map", "multimap", "unordered_set", "unordered_multiset", "unordered_map",
        "unordered_multimap", "stack", "queue", "priority_queue",
        
        // Strings
        "string", "u16string", "u32string", "wstring", "string_view",
        "u8string", "u8string_view",
        
        // I/O
        "istream", "ostream", "iostream", "ifstream", "ofstream", "fstream",
        "istringstream", "ostringstream", "stringstream",
        
        // Smart pointers
        "unique_ptr", "shared_ptr", "weak_ptr", "auto_ptr", // auto_ptr deprecated
        
        // Threading
        "thread", "mutex", "recursive_mutex", "timed_mutex", "shared_mutex",
        "lock_guard", "unique_lock", "shared_lock", "condition_variable",
        
        // Other
        "optional", "variant", "any", "expected", "span", "format",
        "chrono::", "filesystem::"
    };
}

QStringList CppLanguageSupport::standardLibraryFunctions() const
{
    return {
        "std::move", "std::forward", "std::swap", "std::min", "std::max",
        "std::begin", "std::end", "std::size", "std::data", "std::empty",
        "std::make_unique", "std::make_shared", "std::static_pointer_cast",
        "std::dynamic_pointer_cast", "std::const_pointer_cast",
        "std::sort", "std::find", "std::binary_search", "std::lower_bound",
        "std::upper_bound", "std::equal_range"
    };
}

QStringList CppLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "R\""};  // Include raw string prefix
}

std::vector<TokenRule> CppLanguageSupport::tokenRules() const
{
    // Start with common C-family rules
    auto rules = buildCommonRules();
    
    // Add C++-specific template rules
    addTemplateRules(rules);
    
    // Add modern C++ rules
    addModernCppRules(rules);
    
    // Add raw string literal rules
    addRawStringRules(rules);
    
    // Add attribute rules
    addAttributeRules(rules);
    
    // Add user-defined literal rules
    addUserDefinedLiteralRules(rules);
    
    return rules;
}

void CppLanguageSupport::addTemplateRules(std::vector<TokenRule> &rules) const
{
    QTextCharFormat templateFormat;
    templateFormat.setForeground(QColor{"#8B008B"});  // Dark magenta
    templateFormat.setFontWeight(QFont::Bold);
    
    // Template keywords
    rules.emplace_back("template_keyword",
        QRegularExpression{R"(\b(?:template|typename|concept|requires)\b)"},
        templateFormat, false, 95);
}

void CppLanguageSupport::addModernCppRules(std::vector<TokenRule> &rules) const
{
    QTextCharFormat modernFormat;
    modernFormat.setForeground(QColor{"#9932CC"});  // Dark orchid
    modernFormat.setFontWeight(QFont::Bold);
    
    // C++11/14/17/20 specific keywords
    rules.emplace_back("modern_cpp",
        QRegularExpression{R"(\b(?:constexpr|consteval|constinit|decltype|noexcept|nullptr|static_assert|thread_local|co_await|co_return|co_yield|alignas|alignof|char8_t|char16_t|char32_t)\b)"},
        modernFormat, false, 92);
    
    // Module keywords
    QTextCharFormat moduleFormat;
    moduleFormat.setForeground(QColor{"#FF1493"});  // Deep pink
    rules.emplace_back("module",
        QRegularExpression{R"(\b(?:import|module|export)\b)"},
        moduleFormat, false, 93);
}

void CppLanguageSupport::addRawStringRules(std::vector<TokenRule> &rules) const
{
    QTextCharFormat rawStringFormat;
    rawStringFormat.setForeground(QColor{"#008000"});  // Green
    
    // R"delimiter(content)delimiter" - multiline raw strings
    rules.emplace_back("raw_string",
        QRegularExpression{R"(R\"([^\(\)\\ ]{0,16})\(.*?\)\1\")"},
        rawStringFormat, true, 85);
}

void CppLanguageSupport::addAttributeRules(std::vector<TokenRule> &rules) const
{
    QTextCharFormat attributeFormat;
    attributeFormat.setForeground(QColor{"#808080"});  // Gray
    attributeFormat.setFontItalic(true);
    
    // C++11/17/20 attributes: [[nodiscard]], [[likely]], etc.
    rules.emplace_back("attribute",
        QRegularExpression{R"(\[\[.*?\]\])"},
        attributeFormat, false, 75);
}

void CppLanguageSupport::addUserDefinedLiteralRules(std::vector<TokenRule> &rules) const
{
    QTextCharFormat udlFormat;
    udlFormat.setForeground(QColor{"#D2691E"});  // Chocolate
    
    // User-defined literals: 42_km, "hello"s, 3.14_rad
    rules.emplace_back("user_defined_literal",
        QRegularExpression{R"(\b(?:[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?[a-zA-Z_][a-zA-Z0-9_]*|\".*?\"[a-zA-Z_][a-zA-Z0-9_]*)\b)"},
        udlFormat, false, 65);
}

std::vector<AutoCompleteTrigger> CppLanguageSupport::autoCompleteTriggers() const
{
    return {
        // Member access
        {"." , true, 0, {"after_identifier"}},
        // Pointer member access
        {"->", true, 0, {"after_identifier"}},
        // Scope resolution
        {"::", true, 0, {"after_identifier", "after_operator"}},
        // Template arguments
        {"<", false, 0, {"after_identifier", "after_keyword"}},
        // Function arguments
        {"(", false, 0, {"after_identifier"}}
    };
}

} // namespace NovaPad::Languages
