/**
 * @file language_manager.h
 * @brief Language registry and lookup service for syntax highlighting.
 *
 * Maintains an in-memory catalogue of programming languages, each described
 * by its name, MIME type, associated file extensions, and file-name patterns.
 * The catalogue is populated once at startup and then queried by the editor
 * whenever a file is opened or the user manually selects a language.
 *
 * This is the C++23 equivalent of Notepadqq's LanguageService.
 */

#ifndef NOVAPAD_EDITOR_LANGUAGE_MANAGER_H
#define NOVAPAD_EDITOR_LANGUAGE_MANAGER_H

#include <QString>
#include <QStringList>
#include <QVector>

namespace NovaPad::Editor {

/**
 * @struct Language
 * @brief Describes a single programming / markup language.
 */
struct Language
{
    QString     id;               ///< Unique identifier (e.g. "cpp", "python")
    QString     name;             ///< Human-readable name ("C++", "Python")
    QString     mime;             ///< Primary MIME type
    QString     mode;             ///< Highlighter mode group (e.g. "clike")
    QStringList fileNames;        ///< Exact file-name matches (e.g. "Makefile")
    QStringList fileExtensions;   ///< Extensions without dot (e.g. "cpp", "h")
    QStringList firstNonBlankLine;///< Shebang / magic-line patterns

    Language() = default;
    Language(QString i, QString n, QString m, QString mo,
                      QStringList fn, QStringList fe, QStringList fnbl)
        : id(std::move(i)), name(std::move(n)), mime(std::move(m)), mode(std::move(mo)),
          fileNames(std::move(fn)), fileExtensions(std::move(fe)), firstNonBlankLine(std::move(fnbl)) {}
    Language(const Language &) = default;
    Language(Language &&) = default;
    Language &operator=(const Language &) = default;
    Language &operator=(Language &&) = default;
};

using LanguageList = QVector<Language>;

/**
 * @class LanguageManager
 * @brief Singleton language catalogue with efficient lookup by id, extension,
 *        file name, or content.
 *
 * Thread-safety: the catalogue is read-only after construction, so concurrent
 * lookups are safe without locking.
 */
class LanguageManager final
{
public:
    /** @brief Returns the singleton instance (created on first call). */
    [[nodiscard]] static LanguageManager &instance();

    // Non-copyable, non-movable singleton
    LanguageManager(const LanguageManager &) = delete;
    LanguageManager &operator=(const LanguageManager &) = delete;

    // -- Lookup methods ------------------------------------------------------

    /** @brief Finds a language by its unique id (e.g. "python"). */
    [[nodiscard]] const Language *lookupById(const QString &id) const;

    /** @brief Finds a language by exact file name (e.g. "Makefile"). */
    [[nodiscard]] const Language *lookupByFileName(const QString &fileName) const;

    /** @brief Finds a language by file extension (e.g. "cpp"). */
    [[nodiscard]] const Language *lookupByExtension(const QString &filePath) const;

    /** @brief Finds a language by inspecting the first non-blank line (shebang). */
    [[nodiscard]] const Language *lookupByContent(const QString &content) const;

    /** @brief Returns the full language catalogue. */
    [[nodiscard]] const LanguageList &languages() const { return m_languages; }

private:
    LanguageManager();

    /** @brief Populates the catalogue with all supported languages. */
    void registerLanguages();

    LanguageList m_languages;
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_LANGUAGE_MANAGER_H
