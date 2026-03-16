/**
 * @file sql_language.h
 * @brief SQL language support (MySQL, PostgreSQL, SQLite, Standard SQL).
 */

#ifndef NOVAPAD_LANGUAGES_DEFINITIONS_SQL_LANGUAGE_H
#define NOVAPAD_LANGUAGES_DEFINITIONS_SQL_LANGUAGE_H

#include "languages/base/language_support_base.h"

namespace NovaPad::Languages {

/**
 * @class SqlLanguageSupport
 * @brief Comprehensive SQL support for multiple dialects.
 *
 * Supports:
 * - Standard SQL (ANSI SQL)
 * - MySQL/MariaDB extensions
 * - PostgreSQL extensions
 * - SQLite syntax
 * - Window functions
 * - Common Table Expressions (CTEs)
 * - JSON operations
 */
class SqlLanguageSupport final : public LanguageSupport {
public:
    [[nodiscard]] QString id() const override { return "sql"; }
    [[nodiscard]] QString displayName() const override { return "SQL"; }
    [[nodiscard]] QString mimeType() const override { return "text/x-sql"; }
    
    [[nodiscard]] QStringList fileExtensions() const override {
        return {"sql", "ddl", "dml", "mysql", "pgsql", "psql"};
    }
    
    [[nodiscard]] QStringList fileNames() const override { return {}; }
    
    [[nodiscard]] QStringList keywords() const override;
    [[nodiscard]] QStringList builtinTypes() const override;
    [[nodiscard]] std::vector<TokenRule> tokenRules() const override;
    [[nodiscard]] std::vector<BracePair> bracePairs() const override;
    [[nodiscard]] CommentRule commentRule() const override;
    [[nodiscard]] QStringList stringDelimiters() const override;
    [[nodiscard]] IndentationRule indentationRule() const override;
    [[nodiscard]] std::vector<AutoCompleteTrigger> autoCompleteTriggers() const override;
    
    enum class Dialect {
        Standard,
        MySQL,
        PostgreSQL,
        SQLite,
        SQLServer,
        Oracle
    };
    
    void setDialect(Dialect dialect) { m_dialect = dialect; }
    [[nodiscard]] Dialect dialect() const { return m_dialect; }
    
private:
    Dialect m_dialect = Dialect::Standard;
};

} // namespace NovaPad::Languages

#endif // NOVAPAD_LANGUAGES_DEFINITIONS_SQL_LANGUAGE_H
