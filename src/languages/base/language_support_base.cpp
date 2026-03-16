/**
 * @file language_support_base.cpp
 * @brief Implementation of base LanguageSupport functionality.
 */

#include "language_support_base.h"

#include <QFileInfo>

namespace NovaPad::Languages {

bool LanguageSupport::isValidBraceMatch(const QString &text, int openPos, int closePos) const
{
    Q_UNUSED(text)
    Q_UNUSED(openPos)
    Q_UNUSED(closePos)
    // Default implementation accepts all matches; override for language-specific validation
    return true;
}

QString LanguageSupport::calculateIndent(const QString &previousLine,
                                         const QString &currentLine,
                                         int currentIndent) const
{
    Q_UNUSED(previousLine)
    Q_UNUSED(currentLine)
    // Default: maintain current indentation
    return QString(currentIndent, ' ');
}

} // namespace NovaPad::Languages
