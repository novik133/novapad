/**
 * @file c_language.cpp
 * @brief Implementation of C language support.
 */

#include "c_language.h"

namespace NovaPad::Languages {

QStringList CLanguageSupport::keywords() const
{
    return {
        // C89 keywords
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while",
        
        // C99 keywords
        "inline", "restrict", "_Bool", "_Complex", "_Imaginary",
        
        // C11 keywords
        "_Alignas", "_Alignof", "_Atomic", "_Generic", "_Noreturn",
        "_Static_assert", "_Thread_local",
        
        // C23 keywords
        "alignas", "alignof", "bool", "constexpr", "false", "nullptr",
        "static_assert", "thread_local", "true", "typeof", "typeof_unqual"
    };
}

QStringList CLanguageSupport::builtinTypes() const
{
    return {
        "void", "char", "short", "int", "long", "float", "double",
        "signed", "unsigned", "_Bool", "bool", "_Complex", "_Imaginary",
        "size_t", "ptrdiff_t", "intptr_t", "uintptr_t", "int8_t", "int16_t",
        "int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
        "nullptr_t"
    };
}

std::vector<TokenRule> CLanguageSupport::tokenRules() const
{
    auto rules = buildCommonRules();
    
    // C99/C11 generic selection highlighting
    QTextCharFormat genericFormat;
    genericFormat.setForeground(QColor{"#8B4513"});  // Saddle brown
    genericFormat.setFontWeight(QFont::Bold);
    rules.emplace_back("generic",
        QRegularExpression{R"(\b(?:_Generic|generic)\b)"},
        genericFormat, false, 85);
    
    // Standard library function highlighting
    QTextCharFormat stdlibFormat;
    stdlibFormat.setForeground(QColor{"#4682B4"});  // Steel blue
    rules.emplace_back("stdlib",
        QRegularExpression{R"(\b(?:printf|scanf|malloc|calloc|realloc|free|fopen|fclose|fread|fwrite|sprintf|snprintf|memcpy|memmove|memset|memcmp|strlen|strcpy|strncpy|strcat|strncat|strcmp|strncmp|strchr|strstr|atoi|atol|atof|abs|labs|div|ldiv|rand|srand|exit|atexit|qsort|bsearch|getenv|system)\b)"},
        stdlibFormat, false, 60);
    
    return rules;
}

std::vector<AutoCompleteTrigger> CLanguageSupport::autoCompleteTriggers() const
{
    return {
        {"->", true, 0, {"after_identifier"}},
        {"." , false, 0, {"after_identifier"}},  // Less common in C
        {"(", false, 0, {"after_identifier"}}
    };
}

} // namespace NovaPad::Languages
