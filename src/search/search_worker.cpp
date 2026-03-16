/**
 * @file search_worker.cpp
 * @brief Implementation of the multi-file search worker.
 *
 * Recursively enumerates files matching the glob filter, reads each one
 * line by line, and collects matches.  For large projects a production
 * implementation would use QtConcurrent::mappedReduced; the synchronous
 * version here is sufficient for moderate-sized codebases.
 */

#include "search_worker.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

namespace NovaPad::Search {

QVector<SearchMatch> SearchWorker::search(const Options &opts)
{
    QVector<SearchMatch> results;

    if (opts.searchTerm.isEmpty() || opts.directory.isEmpty()) {
        return results;
    }

    // Build the regex or prepare for plain-text matching
    QRegularExpression regex;
    if (opts.regex) {
        auto flags = opts.caseSensitive
                         ? QRegularExpression::NoPatternOption
                         : QRegularExpression::CaseInsensitiveOption;
        regex.setPattern(opts.searchTerm);
        regex.setPatternOptions(flags);
        if (!regex.isValid()) return results;
    }

    // Parse the filter into QStringList for QDirIterator name filters
    QStringList nameFilters = opts.filter.split(';', Qt::SkipEmptyParts);
    if (nameFilters.isEmpty()) nameFilters << "*.*";

    // Recursively iterate matching files
    QDirIterator it(opts.directory, nameFilters,
                    QDir::Files | QDir::Readable,
                    QDirIterator::Subdirectories);

    while (it.hasNext()) {
        const QString filePath = it.next();

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) continue;

        QTextStream stream(&file);
        int lineNumber = 0;

        while (!stream.atEnd()) {
            const QString line = stream.readLine();
            ++lineNumber;

            bool matched = false;
            int  col     = 0;

            if (opts.regex) {
                auto match = regex.match(line);
                if (match.hasMatch()) {
                    matched = true;
                    col = static_cast<int>(match.capturedStart());
                }
            } else {
                Qt::CaseSensitivity cs = opts.caseSensitive
                                             ? Qt::CaseSensitive
                                             : Qt::CaseInsensitive;
                int idx = line.indexOf(opts.searchTerm, 0, cs);
                if (idx >= 0) {
                    // Whole-word check
                    if (opts.wholeWord) {
                        auto isWordChar = [](QChar c) {
                            return c.isLetterOrNumber() || c == QLatin1Char('_');
                        };
                        bool leftOk  = (idx == 0) || !isWordChar(line[idx - 1]);
                        int  end     = idx + opts.searchTerm.length();
                        bool rightOk = (end >= line.length()) || !isWordChar(line[end]);
                        if (!leftOk || !rightOk) continue;
                    }
                    matched = true;
                    col = idx;
                }
            }

            if (matched) {
                results.append({
                    filePath,
                    lineNumber,
                    col,
                    line.trimmed()
                });
            }
        }
    }

    return results;
}

} // namespace NovaPad::Search
