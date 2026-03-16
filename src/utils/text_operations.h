/**
 * @file text_operations.h
 * @brief Text manipulation utilities used across the application.
 *
 * Provides string transformations (case conversion, whitespace operations,
 * line sorting, duplicate removal, etc.) that are invoked by the Edit menu
 * and by extensions.
 */

#ifndef NOVAPAD_UTILS_TEXT_OPERATIONS_H
#define NOVAPAD_UTILS_TEXT_OPERATIONS_H

#include <QString>
#include <QStringList>

namespace NovaPad::Utils {

/**
 * @class TextOperations
 * @brief Static utility functions for common text transformations.
 */
class TextOperations final
{
    Q_DISABLE_COPY_MOVE(TextOperations)
    TextOperations() = delete;

public:
    // -- Case transformations ------------------------------------------------
    [[nodiscard]] static QString toUpperCase(const QString &text);
    [[nodiscard]] static QString toLowerCase(const QString &text);
    [[nodiscard]] static QString toTitleCase(const QString &text);
    [[nodiscard]] static QString invertCase(const QString &text);

    // -- Whitespace operations -----------------------------------------------
    [[nodiscard]] static QString trimTrailing(const QString &text);
    [[nodiscard]] static QString trimLeading(const QString &text);
    [[nodiscard]] static QString trimBoth(const QString &text);
    [[nodiscard]] static QString tabsToSpaces(const QString &text, int tabSize = 4);
    [[nodiscard]] static QString spacesToTabs(const QString &text, int tabSize = 4);
    [[nodiscard]] static QString spacesToTabsLeading(const QString &text, int tabSize = 4);
    [[nodiscard]] static QString eolToSpace(const QString &text);
    [[nodiscard]] static QString removeBlankLines(const QString &text);

    // -- Line operations -----------------------------------------------------
    [[nodiscard]] static QString sortLinesAscending(const QString &text);
    [[nodiscard]] static QString sortLinesDescending(const QString &text);
    [[nodiscard]] static QString removeDuplicateLines(const QString &text);
    [[nodiscard]] static QString reverseLines(const QString &text);
    [[nodiscard]] static QString joinLines(const QString &text, const QString &separator = " ");

    // -- Encoding helpers ----------------------------------------------------
    [[nodiscard]] static QString urlEncode(const QString &text);
    [[nodiscard]] static QString urlDecode(const QString &text);
    [[nodiscard]] static QString base64Encode(const QString &text);
    [[nodiscard]] static QString base64Decode(const QString &text);
};

} // namespace NovaPad::Utils

#endif // NOVAPAD_UTILS_TEXT_OPERATIONS_H
