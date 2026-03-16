/**
 * @file text_operations.cpp
 * @brief Implementation of text manipulation utilities.
 */

#include "text_operations.h"

#include <QSet>
#include <QUrl>

namespace NovaPad::Utils {

// ---------------------------------------------------------------------------
// Case transformations
// ---------------------------------------------------------------------------

QString TextOperations::toUpperCase(const QString &text) { return text.toUpper(); }
QString TextOperations::toLowerCase(const QString &text) { return text.toLower(); }

QString TextOperations::toTitleCase(const QString &text)
{
    QString result = text;
    bool prevSpace = true;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i].isSpace()) {
            prevSpace = true;
        } else if (prevSpace) {
            result[i] = result[i].toUpper();
            prevSpace = false;
        } else {
            result[i] = result[i].toLower();
        }
    }
    return result;
}

QString TextOperations::invertCase(const QString &text)
{
    QString result = text;
    for (int i = 0; i < result.size(); ++i) {
        if (result[i].isUpper())      result[i] = result[i].toLower();
        else if (result[i].isLower()) result[i] = result[i].toUpper();
    }
    return result;
}

// ---------------------------------------------------------------------------
// Whitespace operations
// ---------------------------------------------------------------------------

QString TextOperations::trimTrailing(const QString &text)
{
    QStringList lines = text.split('\n');
    for (auto &line : lines) {
        while (line.endsWith(' ') || line.endsWith('\t'))
            line.chop(1);
    }
    return lines.join('\n');
}

QString TextOperations::trimLeading(const QString &text)
{
    QStringList lines = text.split('\n');
    for (auto &line : lines) {
        while (line.startsWith(' ') || line.startsWith('\t'))
            line.remove(0, 1);
    }
    return lines.join('\n');
}

QString TextOperations::trimBoth(const QString &text)
{
    return trimLeading(trimTrailing(text));
}

QString TextOperations::tabsToSpaces(const QString &text, int tabSize)
{
    QString result = text;
    result.replace('\t', QString(tabSize, ' '));
    return result;
}

QString TextOperations::spacesToTabs(const QString &text, int tabSize)
{
    QString result = text;
    result.replace(QString(tabSize, ' '), QStringLiteral("\t"));
    return result;
}

QString TextOperations::spacesToTabsLeading(const QString &text, int tabSize)
{
    QStringList lines = text.split('\n');
    for (auto &line : lines) {
        int spaces = 0;
        for (const QChar &ch : std::as_const(line)) {
            if (ch == ' ') ++spaces;
            else break;
        }
        int tabs = spaces / tabSize;
        int rem  = spaces % tabSize;
        line = QString(tabs, '\t') + QString(rem, ' ') + line.mid(spaces);
    }
    return lines.join('\n');
}

QString TextOperations::eolToSpace(const QString &text)
{
    QString result = text;
    result.replace('\n', ' ');
    return result;
}

QString TextOperations::removeBlankLines(const QString &text)
{
    QStringList lines = text.split('\n');
    QStringList filtered;
    for (const auto &line : lines) {
        if (!line.trimmed().isEmpty()) {
            filtered.append(line);
        }
    }
    return filtered.join('\n');
}

// ---------------------------------------------------------------------------
// Line operations
// ---------------------------------------------------------------------------

QString TextOperations::sortLinesAscending(const QString &text)
{
    QStringList lines = text.split('\n');
    lines.sort(Qt::CaseInsensitive);
    return lines.join('\n');
}

QString TextOperations::sortLinesDescending(const QString &text)
{
    QStringList lines = text.split('\n');
    lines.sort(Qt::CaseInsensitive);
    std::reverse(lines.begin(), lines.end());
    return lines.join('\n');
}

QString TextOperations::removeDuplicateLines(const QString &text)
{
    QStringList lines = text.split('\n');
    QSet<QString> seen;
    QStringList unique;
    for (const auto &line : lines) {
        if (!seen.contains(line)) {
            seen.insert(line);
            unique.append(line);
        }
    }
    return unique.join('\n');
}

QString TextOperations::reverseLines(const QString &text)
{
    QStringList lines = text.split('\n');
    std::reverse(lines.begin(), lines.end());
    return lines.join('\n');
}

QString TextOperations::joinLines(const QString &text, const QString &separator)
{
    QStringList lines = text.split('\n');
    return lines.join(separator);
}

// ---------------------------------------------------------------------------
// Encoding helpers
// ---------------------------------------------------------------------------

QString TextOperations::urlEncode(const QString &text)
{
    return QUrl::toPercentEncoding(text);
}

QString TextOperations::urlDecode(const QString &text)
{
    return QUrl::fromPercentEncoding(text.toUtf8());
}

QString TextOperations::base64Encode(const QString &text)
{
    return QString::fromLatin1(text.toUtf8().toBase64());
}

QString TextOperations::base64Decode(const QString &text)
{
    return QString::fromUtf8(QByteArray::fromBase64(text.toLatin1()));
}

} // namespace NovaPad::Utils
