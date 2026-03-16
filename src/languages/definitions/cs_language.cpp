/**
 * @file cs_language.cpp
 * @brief Implementation of C# language support.
 */

#include "cs_language.h"

namespace NovaPad::Languages {

QStringList CsLanguageSupport::keywords() const
{
    return {
        "abstract", "as", "base", "bool", "break", "byte", "case", "catch",
        "char", "checked", "class", "const", "continue", "decimal", "default",
        "delegate", "do", "double", "else", "enum", "event", "explicit",
        "extern", "false", "finally", "fixed", "float", "for", "foreach",
        "goto", "if", "implicit", "in", "int", "interface", "internal",
        "is", "lock", "long", "namespace", "new", "null", "object", "operator",
        "out", "override", "params", "private", "protected", "public",
        "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof",
        "stackalloc", "static", "string", "struct", "switch", "this", "throw",
        "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe",
        "ushort", "using", "virtual", "void", "volatile", "while",
        "var", "dynamic", "async", "await", "nameof", "when",
        "record", "init", "and", "or", "not", "nint", "nuint",
        "file", "scoped", "required"
    };
}

QStringList CsLanguageSupport::builtinTypes() const
{
    return {
        "bool", "byte", "sbyte", "char", "decimal", "double", "float",
        "int", "uint", "long", "ulong", "short", "ushort", "object",
        "string", "void", "dynamic", "var", "nint", "nuint"
    };
}

QStringList CsLanguageSupport::standardLibraryTypes() const
{
    return {
        "Console", "String", "Object", "Exception", "DateTime", "TimeSpan",
        "Guid", "Uri", "Version", "Nullable", "Tuple", "ValueTuple",
        "Task", "Thread", "CancellationToken", "List", "Dictionary",
        "HashSet", "Queue", "Stack", "ObservableCollection",
        "IEnumerable", "IEnumerator", "ICollection", "IList", "IDictionary",
        "Action", "Func", "Predicate", "EventHandler", "EventArgs"
    };
}

QStringList CsLanguageSupport::stringDelimiters() const
{
    return {"\"", "@\"", "$\"", "'"};
}

std::vector<TokenRule> CsLanguageSupport::tokenRules() const
{
    auto rules = buildCommonRules();
    
    QTextCharFormat contextualFormat;
    contextualFormat.setForeground(QColor{"#9370DB"});
    rules.emplace_back("contextual",
        QRegularExpression{R"(\b(?:var|dynamic|nameof|async|await|yield|partial|record|init)\b)"},
        contextualFormat, false, 88);
    
    QTextCharFormat linqFormat;
    linqFormat.setForeground(QColor{"#FF69B4"});
    rules.emplace_back("linq",
        QRegularExpression{R"(\b(?:select|from|where|group|into|orderby|join|let|ascending|descending|by|on|equals)\b)"},
        linqFormat, false, 87);
    
    QTextCharFormat verbatimFormat;
    verbatimFormat.setForeground(QColor{"#228B22"});
    rules.emplace_back("verbatim_string",
        QRegularExpression{R"(@\"[^\"]*(?:\"\"[^\"]*)*\")"},
        verbatimFormat, true, 84);
    
    return rules;
}

std::vector<AutoCompleteTrigger> CsLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"." , true, 0, {"after_identifier"}},
        {"?.", true, 0, {"after_identifier"}},
        {"??", true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {"<", false, 0, {"after_identifier"}},
        {"=>", true, 0, {"after_identifier"}}
    };
}

} // namespace NovaPad::Languages
