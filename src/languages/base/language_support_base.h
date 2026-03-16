/**
 * @file language_support_base.h
 * @brief Base interfaces and data structures for comprehensive language support.
 *
 * Provides the foundation for language-specific features including syntax highlighting,
 * brace matching, indentation rules, code folding, and auto-completion triggers.
 * Each language definition inherits from LanguageSupport to provide specialized behavior.
 */

#ifndef NOVAPAD_LANGUAGES_BASE_LANGUAGE_SUPPORT_BASE_H
#define NOVAPAD_LANGUAGES_BASE_LANGUAGE_SUPPORT_BASE_H

#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QTextCharFormat>
#include <vector>

namespace NovaPad::Languages {

/**
 * @enum BraceType
 * @brief Classification of brace delimiters for matching and folding.
 */
enum class BraceType {
    Curly,       ///< { } - Used by C-family, Go, Rust, Swift, PHP
    Round,       ///< ( ) - Universal function calls, expressions
    Square,      ///< [ ] - Arrays, indexing, attributes
    Angle,       ///< < > - Generics, templates, comparisons
    TripleQuote, ///< """ """ or ''' ''' - Python multiline strings
    XmlTag,      ///< <tag></tag> - HTML/XML markup
    Verbatim,    ///< #[ ] or #| |# - Rust raw strings, Lisp blocks
    IndentBased  ///< Python, YAML - no braces, uses indentation
};

/**
 * @struct BracePair
 * @brief Defines a matching pair of opening/closing delimiters.
 */
struct BracePair {
    QChar open;           ///< Opening character (e.g., '{')
    QChar close;          ///< Closing character (e.g., '}')
    BraceType type;       ///< Classification for handling strategy
    bool multiline;       ///< Whether this pair can span multiple lines
    QString escapeChar;   ///< Escape character (e.g., "\" for strings)
    
    constexpr BracePair(QChar o, QChar c, BraceType t, bool ml = true, QString esc = {})
        : open(o), close(c), type(t), multiline(ml), escapeChar(std::move(esc)) {}
};

/**
 * @struct TokenRule
 * @brief A syntax highlighting rule mapping patterns to text formats.
 */
struct TokenRule {
    QString name;                  ///< Rule identifier (e.g., "keyword", "string")
    QRegularExpression pattern;    ///< Regex pattern to match tokens
    QTextCharFormat format;        ///< Visual formatting (color, weight, etc.)
    bool multiline;                ///< Whether rule can span multiple blocks
    int priority;                  ///< Matching priority (higher = first)
    
    TokenRule() = default;
    TokenRule(QString n, QRegularExpression p, QTextCharFormat f, 
              bool ml = false, int prio = 0)
        : name(std::move(n)), pattern(std::move(p)), format(std::move(f)),
          multiline(ml), priority(prio) {}
};

/**
 * @struct CommentRule
 * @brief Defines comment syntax for a language.
 */
struct CommentRule {
    QString lineStart;      ///< Line comment marker (e.g., "slash-slash", "#")
    QString blockStart;     ///< Block comment start (e.g., "slash-star", "<!--")
    QString blockEnd;       ///< Block comment end (e.g., "star-slash", "-->")
    bool nested;            ///< Whether block comments can be nested
    
    CommentRule() = default;
    CommentRule(QString line, QString blkStart = {}, QString blkEnd = {}, bool nest = false)
        : lineStart(std::move(line)), blockStart(std::move(blkStart)),
          blockEnd(std::move(blkEnd)), nested(nest) {}
};

/**
 * @struct IndentationRule
 * @brief Defines indentation behavior for a language.
 */
struct IndentationRule {
    bool useTabs;           ///< Use tabs vs spaces
    int tabSize;            ///< Number of spaces per tab
    bool smartIndent;       ///< Auto-indent based on context
    
    // Brace-based indentation settings
    bool indentAfterBrace;  ///< Indent after opening brace
    bool indentAfterColon;  ///< Indent after colon (Python-style)
    
    // Specific triggers for indentation increase/decrease
    QStringList indentTriggers;   ///< Keywords that trigger indent (e.g., "if", "for")
    QStringList outdentTriggers;  ///< Keywords that trigger outdent (e.g., "return", "break")
    
    IndentationRule()
        : useTabs(false), tabSize(4), smartIndent(true),
          indentAfterBrace(true), indentAfterColon(false) {}
};

/**
 * @struct AutoCompleteTrigger
 * @brief Defines auto-completion behavior for specific contexts.
 */
struct AutoCompleteTrigger {
    QString trigger;              ///< Character that triggers completion (e.g., ".", "::")
    bool immediate;               ///< Show suggestions immediately vs delay
    int minChars;                 ///< Minimum characters before triggering
    QStringList contexts;        ///< Valid contexts (e.g., "after_identifier", "after_operator")
};

/**
 * @class LanguageSupport
 * @brief Abstract base class providing language-specific editor features.
 *
 * Each supported language implements this interface to provide:
 * - Syntax highlighting rules
 * - Brace matching pairs
 * - Indentation and folding behavior
 * - Auto-completion triggers
 * - Language-specific validation
 */
class LanguageSupport {
public:
    virtual ~LanguageSupport() = default;
    
    // -- Identity ----------------------------------------------------------
    
    /** @brief Unique language identifier (e.g., "cpp", "python", "rust"). */
    [[nodiscard]] virtual QString id() const = 0;
    
    /** @brief Human-readable language name (e.g., "C++", "Python 3"). */
    [[nodiscard]] virtual QString displayName() const = 0;
    
    /** @brief Primary MIME type for the language. */
    [[nodiscard]] virtual QString mimeType() const = 0;
    
    /** @brief File extensions associated with this language (without dot). */
    [[nodiscard]] virtual QStringList fileExtensions() const = 0;
    
    /** @brief Specific filenames that indicate this language (e.g., "Makefile"). */
    [[nodiscard]] virtual QStringList fileNames() const = 0;
    
    // -- Syntax Highlighting ----------------------------------------------
    
    /** @brief Returns all token rules for syntax highlighting. */
    [[nodiscard]] virtual std::vector<TokenRule> tokenRules() const = 0;
    
    /** @brief Returns comment syntax rules. */
    [[nodiscard]] virtual CommentRule commentRule() const = 0;
    
    /** @brief Returns string literal delimiters (e.g., '"', "'", "`"). */
    [[nodiscard]] virtual QStringList stringDelimiters() const = 0;
    
    // -- Brace Matching & Folding ------------------------------------------
    
    /** @brief Returns all brace pairs for matching and folding. */
    [[nodiscard]] virtual std::vector<BracePair> bracePairs() const = 0;
    
    /** @brief Whether this language uses indentation-based folding (Python-style). */
    [[nodiscard]] virtual bool usesIndentBasedFolding() const { return false; }
    
    /** @brief Validates if a brace pair is correctly matched at given position. */
    [[nodiscard]] virtual bool isValidBraceMatch(const QString &text, int openPos, int closePos) const;
    
    // -- Indentation --------------------------------------------------------
    
    /** @brief Returns indentation rules for the language. */
    [[nodiscard]] virtual IndentationRule indentationRule() const = 0;
    
    /** @brief Calculates the appropriate indentation for a new line. */
    [[nodiscard]] virtual QString calculateIndent(const QString &previousLine, 
                                                   const QString &currentLine,
                                                   int currentIndent) const;
    
    // -- Auto-completion ---------------------------------------------------
    
    /** @brief Returns triggers for context-aware auto-completion. */
    [[nodiscard]] virtual std::vector<AutoCompleteTrigger> autoCompleteTriggers() const = 0;
    
    /** @brief Returns built-in keywords for completion. */
    [[nodiscard]] virtual QStringList keywords() const = 0;
    
    /** @brief Returns built-in types for completion. */
    [[nodiscard]] virtual QStringList builtinTypes() const = 0;
    
    // -- Language-specific features ----------------------------------------
    
    /** @brief Returns shebang patterns for script detection. */
    [[nodiscard]] virtual QStringList shebangPatterns() const { return {}; }
    
    /** @brief Whether this language supports nested block comments. */
    [[nodiscard]] virtual bool supportsNestedComments() const { return false; }
    
    /** @brief Whether this language uses preprocessor directives. */
    [[nodiscard]] virtual bool hasPreprocessor() const { return false; }
    
    /** @brief Returns preprocessor prefix if applicable (e.g., "#"). */
    [[nodiscard]] virtual QString preprocessorPrefix() const { return {}; }
    
    /** @brief Whether this language is case-sensitive. */
    [[nodiscard]] virtual bool isCaseSensitive() const { return true; }
    
    // -- Factory method -----------------------------------------------------
    
    /** @brief Creates language support instance by ID. */
    [[nodiscard]] static std::unique_ptr<LanguageSupport> create(const QString &id);
    
    /** @brief Detects language from file extension or content. */
    [[nodiscard]] static QString detectLanguage(const QString &filePath, 
                                                const QString &content = {});
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_BASE_LANGUAGE_SUPPORT_BASE_H
