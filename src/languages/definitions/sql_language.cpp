/**
 * @file sql_language.cpp
 * @brief Implementation of SQL language support.
 */

#include "sql_language.h"

namespace NovaPad::Languages {

QStringList SqlLanguageSupport::keywords() const
{
    return {
        // Standard SQL keywords
        "SELECT", "FROM", "WHERE", "INSERT", "UPDATE", "DELETE", "CREATE",
        "DROP", "ALTER", "TABLE", "INDEX", "VIEW", "TRIGGER", "PROCEDURE",
        "FUNCTION", "DATABASE", "SCHEMA", "COLUMN", "CONSTRAINT", "PRIMARY",
        "FOREIGN", "KEY", "REFERENCES", "UNIQUE", "NOT", "NULL", "DEFAULT",
        "AUTO_INCREMENT", "IDENTITY", "CHECK", "AND", "OR", "XOR", "IN",
        "BETWEEN", "LIKE", "IS", "EXISTS", "CASE", "WHEN", "THEN", "ELSE",
        "END", "IF", "ELSEIF", "WHILE", "FOR", "LOOP", "REPEAT", "UNTIL",
        "CONTINUE", "BREAK", "RETURN", "BEGIN", "COMMIT", "ROLLBACK",
        "TRANSACTION", "SAVEPOINT", "JOIN", "INNER", "OUTER", "LEFT", "RIGHT",
        "FULL", "CROSS", "NATURAL", "ON", "USING", "GROUP", "BY", "ORDER",
        "ASC", "DESC", "HAVING", "LIMIT", "OFFSET", "TOP", "DISTINCT", "ALL",
        "AS", "WITH", "RECURSIVE", "UNION", "INTERSECT", "EXCEPT", "MINUS",
        "VALUES", "INTO", "SET", "DECLARE", "CURSOR", "OPEN", "CLOSE", "FETCH",
        "EXECUTE", "PREPARE", "DEALLOCATE", "GRANT", "REVOKE", "PRIVILEGES",
        "TO", "USER", "ROLE", "CASCADE", "RESTRICT", "COLLATE", "CHARSET",
        
        // MySQL specific
        "SHOW", "DESCRIBE", "EXPLAIN", "OPTIMIZE", "ANALYZE", "REPAIR",
        "LOCK", "UNLOCK", "TABLES", "FLUSH", "RESET", "PURGE", "BINLOG",
        "MASTER", "SLAVE", "REPLICATION", "EVENT", "SCHEDULER",
        
        // PostgreSQL specific
        "RETURNING", "WINDOW", "OVER", "PARTITION", "RANGE", "ROWS",
        "PRECEDING", "FOLLOWING", "UNBOUNDED", "CURRENT", "ROW", "LATERAL",
        "VARIADIC", "RETURNS", "SETOF", "PERFORM", "RAISE", "NOTICE",
        "EXCEPTION", "PERFORM", "QUERY", "EXECUTE", "FORMAT",
        
        // SQLite specific
        "VACUUM", "ATTACH", "DETACH", "PRAGMA", "REINDEX",
        
        // Window functions
        "ROW_NUMBER", "RANK", "DENSE_RANK", "CUME_DIST", "PERCENT_RANK",
        "LAG", "LEAD", "FIRST_VALUE", "LAST_VALUE", "NTH_VALUE",
        "NTILE", "SUM", "AVG", "COUNT", "MIN", "MAX", "OVER",
        
        // JSON functions (modern SQL)
        "JSON", "JSONB", "JSON_ARRAY", "JSON_OBJECT", "JSON_ARRAYAGG",
        "JSON_OBJECTAGG", "JSON_EXTRACT", "JSON_UNQUOTE", "JSON_QUOTE",
        "JSON_MERGE", "JSON_MERGE_PRESERVE", "JSON_MERGE_PATCH",
        "JSON_TYPE", "JSON_VALID", "JSON_LENGTH", "JSON_DEPTH", "JSON_KEYS",
        "JSON_REMOVE", "JSON_REPLACE", "JSON_SET", "JSON_INSERT",
        "JSON_CONTAINS", "JSON_CONTAINS_PATH", "JSON_SEARCH",
        "JSON_PRETTY", "JSON_STORAGE_SIZE"
    };
}

QStringList SqlLanguageSupport::builtinTypes() const
{
    return {
        // Numeric types
        "INT", "INTEGER", "TINYINT", "SMALLINT", "MEDIUMINT", "BIGINT",
        "DECIMAL", "NUMERIC", "FLOAT", "DOUBLE", "REAL", "DEC", "FIXED",
        "BIT", "BOOLEAN", "BOOL", "SERIAL", "BIGSERIAL", "SMALLSERIAL",
        "MONEY",
        
        // String types
        "CHAR", "VARCHAR", "TINYTEXT", "TEXT", "MEDIUMTEXT", "LONGTEXT",
        "BINARY", "VARBINARY", "TINYBLOB", "BLOB", "MEDIUMBLOB", "LONGBLOB",
        "ENUM", "SET", "CITEXT",
        
        // Date and time types
        "DATE", "TIME", "DATETIME", "TIMESTAMP", "YEAR", "INTERVAL",
        "TIMESTAMPTZ", "TIMETZ",
        
        // Binary/JSON types
        "JSON", "JSONB", "XML", "UUID", "BYTEA",
        
        // Spatial types (MySQL/PostGIS)
        "GEOMETRY", "POINT", "LINESTRING", "POLYGON", "MULTIPOINT",
        "MULTILINESTRING", "MULTIPOLYGON", "GEOMETRYCOLLECTION",
        
        // PostgreSQL specific
        "ARRAY", "HSTORE", "INET", "CIDR", "MACADDR", "TSVECTOR", "TSQUERY",
        
        // Type modifiers
        "UNSIGNED", "ZEROFILL", "CHARACTER", "VARYING", "NATIONAL", "NCHAR",
        "NVARCHAR"
    };
}

QStringList SqlLanguageSupport::stringDelimiters() const
{
    return {"\"", "'", "`"};  // MySQL backticks for identifiers
}

std::vector<BracePair> SqlLanguageSupport::bracePairs() const
{
    return {
        BracePair('(', ')', BraceType::Round, true, {}),
        BracePair('[', ']', BraceType::Square, true, {})
    };
}

CommentRule SqlLanguageSupport::commentRule() const
{
    return CommentRule("--", "/*", "*/", false);
}

IndentationRule SqlLanguageSupport::indentationRule() const
{
    IndentationRule rule;
    rule.useTabs = false;
    rule.tabSize = 2;
    rule.smartIndent = true;
    rule.indentAfterBrace = true;
    rule.indentTriggers = {"SELECT", "FROM", "WHERE", "JOIN", "GROUP", "ORDER",
                          "HAVING", "INSERT", "UPDATE", "DELETE", "CREATE", "BEGIN",
                          "IF", "CASE", "FOR", "WHILE", "LOOP", "WITH"};
    return rule;
}

std::vector<TokenRule> SqlLanguageSupport::tokenRules() const
{
    std::vector<TokenRule> rules;
    
    // SQL keywords (case-insensitive)
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor{"#0000FF"});
    keywordFormat.setFontWeight(QFont::Bold);
    QString kw = keywords().join("|");
    rules.emplace_back("keyword",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(kw), QRegularExpression::CaseInsensitiveOption},
        keywordFormat, false, 100);
    
    // Data types
    QTextCharFormat typeFormat;
    typeFormat.setForeground(QColor{"#2E8B57"});
    QString types = builtinTypes().join("|");
    rules.emplace_back("type",
        QRegularExpression{QString{R"(\b(?:%1)\b)"}.arg(types), QRegularExpression::CaseInsensitiveOption},
        typeFormat, false, 95);
    
    // Functions
    QTextCharFormat funcFormat;
    funcFormat.setForeground(QColor{"#800080"});
    rules.emplace_back("function",
        QRegularExpression{R"(\b[A-Z][A-Z0-9_]*\s*\()"},
        funcFormat, false, 90);
    
    // String literals
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor{"#008000"});
    rules.emplace_back("string",
        QRegularExpression{R"("(?:[^"]|"")*"|'(?:[^']|'')*')"},
        stringFormat, false, 85);
    
    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor{"#FF6600"});
    rules.emplace_back("number",
        QRegularExpression{R"(\b(?:\d+\.?\d*|\.\d+)(?:[eE][+-]?\d+)?\b)"},
        numberFormat, false, 80);
    
    // Identifiers with backticks (MySQL)
    QTextCharFormat backtickFormat;
    backtickFormat.setForeground(QColor{"#D2691E"});
    rules.emplace_back("backtick_id",
        QRegularExpression{R"(`[^`]+`)"},
        backtickFormat, false, 75);
    
    // Schema qualified identifiers (schema.table)
    QTextCharFormat qualifiedFormat;
    qualifiedFormat.setForeground(QColor{"#4169E1"});
    rules.emplace_back("qualified",
        QRegularExpression{R"(\w+\.\w+)"},
        qualifiedFormat, false, 73);
    
    // Parameters :param, @param, $param
    QTextCharFormat paramFormat;
    paramFormat.setForeground(QColor{"#FF1493"});
    rules.emplace_back("parameter",
        QRegularExpression{R"((?:[:@$])\w+)"},
        paramFormat, false, 70);
    
    // Operators
    QTextCharFormat opFormat;
    opFormat.setForeground(QColor{"#FF00FF"});
    rules.emplace_back("operator",
        QRegularExpression{R"([=<>!]+|LIKE|IN|BETWEEN|IS|NOT|AND|OR|XOR)"},
        opFormat, false, 65);
    
    // Comments -- and /* */
    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor{"#808080"});
    commentFormat.setFontItalic(true);
    rules.emplace_back("line_comment",
        QRegularExpression{R"(--[^\n]*)"},
        commentFormat, false, 50);
    rules.emplace_back("block_comment",
        QRegularExpression{R"(/\*[\s\S]*?\*/)"},
        commentFormat, true, 50);
    
    // PostgreSQL dollar-quoted strings $tag$...$tag$
    QTextCharFormat dollarFormat;
    dollarFormat.setForeground(QColor{"#20B2AA"});
    rules.emplace_back("dollar_string",
        QRegularExpression{R"(\$\w*\$[\s\S]*?\$\w*\$)"},
        dollarFormat, true, 88);
    
    // Window functions OVER (PARTITION BY...)
    QTextCharFormat windowFormat;
    windowFormat.setForeground(QColor{"#9932CC"});
    rules.emplace_back("window",
        QRegularExpression{R"(\bOVER\s*\([^)]*\))"},
        windowFormat, false, 60);
    
    // CTEs WITH ... AS
    QTextCharFormat cteFormat;
    cteFormat.setForeground(QColor{"#FF69B4"});
    rules.emplace_back("cte",
        QRegularExpression{R"(\bWITH\s+(?:RECURSIVE\s+)?\w+\s+AS)"},
        cteFormat, false, 58);
    
    return rules;
}

std::vector<AutoCompleteTrigger> SqlLanguageSupport::autoCompleteTriggers() const
{
    return {
        {".", true, 0, {"after_identifier"}},
        {"(", false, 0, {"after_identifier"}},
        {"AS", false, 0, {"after_expression"}},
        {"ON", false, 0, {"after_join"}},
        {"VALUES", false, 0, {"after_insert"}},
        {",", false, 0, {"after_column"}}
    };
}

} // namespace NovaPad::Languages
