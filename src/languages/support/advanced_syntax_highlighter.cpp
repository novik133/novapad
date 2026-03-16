/**
 * @file advanced_syntax_highlighter.cpp
 * @brief Implementation of the advanced syntax highlighter.
 */

#include "advanced_syntax_highlighter.h"

#include <QRegularExpression>
#include <QTextBlock>

namespace NovaPad::Languages {

AdvancedSyntaxHighlighter::AdvancedSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // Initialize with default text format
}

void AdvancedSyntaxHighlighter::setLanguage(const QString &languageId)
{
    if (m_languageId == languageId) return;
    
    m_languageId = languageId;
    m_language = LanguageRegistry::instance().getLanguage(languageId);
    
    // Clear format cache when language changes
    m_formatCache.clear();
    
    // Rehighlight entire document
    rehighlight();
}

void AdvancedSyntaxHighlighter::rehighlightAll()
{
    m_formatCache.clear();
    rehighlight();
}

void AdvancedSyntaxHighlighter::highlightBlock(const QString &text)
{
    if (!m_language) return;
    
    // Handle multi-line constructs first
    handleMultiLineConstructs(text);
    
    // Apply token rules
    applyTokenRules(text);
}

void AdvancedSyntaxHighlighter::applyTokenRules(const QString &text)
{
    if (!m_language) return;
    
    const auto rules = m_language->tokenRules();
    
    for (const auto &rule : rules) {
        // Skip multiline rules - they are handled separately
        if (rule.multiline) continue;
        
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

void AdvancedSyntaxHighlighter::handleMultiLineConstructs(const QString &text)
{
    if (!m_language) return;
    
    const CommentRule commentRule = m_language->commentRule();
    
    int state = previousBlockState();
    if (state == -1) state = StateNormal;
    
    int startIndex = 0;
    
    if (state == StateComment) {
        // Check if block comment ends in this line
        if (!commentRule.blockEnd.isEmpty()) {
            int endIndex = text.indexOf(commentRule.blockEnd);
            if (endIndex >= 0) {
                // Block comment ends here
                setFormat(0, endIndex + commentRule.blockEnd.length(), 
                         QTextCharFormat());  // Comment format from rules
                state = StateNormal;
                startIndex = endIndex + commentRule.blockEnd.length();
            } else {
                // Entire line is in block comment
                setFormat(0, text.length(), QTextCharFormat());
                setCurrentBlockState(StateComment);
                return;
            }
        }
    }
    
    // Apply multi-line token rules
    const auto rules = m_language->tokenRules();
    for (const auto &rule : rules) {
        if (!rule.multiline) continue;
        
        QRegularExpressionMatch match = rule.pattern.match(text);
        if (match.hasMatch()) {
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    
    // Check for block comment start
    if (state == StateNormal && !commentRule.blockStart.isEmpty()) {
        int commentStart = text.indexOf(commentRule.blockStart, startIndex);
        if (commentStart >= 0) {
            int commentEnd = text.indexOf(commentRule.blockEnd, commentStart + commentRule.blockStart.length());
            if (commentEnd >= 0) {
                // Block comment on single line
                QTextCharFormat commentFormat;
                commentFormat.setForeground(QColor{"#808080"});
                commentFormat.setFontItalic(true);
                setFormat(commentStart, commentEnd - commentStart + commentRule.blockEnd.length(), commentFormat);
            } else {
                // Block comment starts but doesn't end
                QTextCharFormat commentFormat;
                commentFormat.setForeground(QColor{"#808080"});
                commentFormat.setFontItalic(true);
                setFormat(commentStart, text.length() - commentStart, commentFormat);
                state = StateComment;
            }
        }
    }
    
    // Handle line comments
    if (state == StateNormal && !commentRule.lineStart.isEmpty()) {
        int lineCommentStart = text.indexOf(commentRule.lineStart);
        if (lineCommentStart >= 0) {
            QTextCharFormat commentFormat;
            commentFormat.setForeground(QColor{"#808080"});
            commentFormat.setFontItalic(true);
            setFormat(lineCommentStart, text.length() - lineCommentStart, commentFormat);
        }
    }
    
    setCurrentBlockState(state);
}

void AdvancedSyntaxHighlighter::updateBlockState(const QString &text, int state)
{
    Q_UNUSED(text)
    setCurrentBlockState(state);
}

} // namespace NovaPad::Languages
