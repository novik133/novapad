/**
 * @file code_editor_widget.cpp
 * @brief Implementation of the native code editor widget.
 *
 * This replaces the QWebView + CodeMirror approach of the original Notepadqq
 * with a pure Qt widget.  All editing, highlighting, and gutter rendering
 * happens in-process without a browser engine, yielding significantly lower
 * memory usage and faster startup.
 */

#include "code_editor_widget.h"
#include "editor/line_number_area.h"
#include "editor/minimap_widget.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCodec>

namespace NovaPad::Editor {

// ---------------------------------------------------------------------------
// Static buffer for pre-allocated editors
// ---------------------------------------------------------------------------
QList<std::shared_ptr<CodeEditorWidget>> CodeEditorWidget::s_editorBuffer;

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

CodeEditorWidget::CodeEditorWidget(QWidget *parent)
    : CodeEditorWidget(Theme{}, parent)
{
}

CodeEditorWidget::CodeEditorWidget(const Theme &theme, QWidget *parent)
    : QPlainTextEdit(parent)
{
    initEditor(theme);
}

CodeEditorWidget::~CodeEditorWidget() = default;

void CodeEditorWidget::initEditor(const Theme & /*theme*/)
{
    // Default codec — UTF-8 without BOM
    m_codec = QTextCodec::codecForName("UTF-8");

    // Line-number gutter (created before setting margins)
    m_lineNumberArea = new LineNumberArea(this);
    m_lineNumberArea->setVisible(true);
    
    // Syntax highlighter (initially empty — set via setLanguage())
    m_highlighter = std::make_unique<SyntaxHighlighter>(document());

    // Bracket matcher
    m_bracketMatcher = std::make_unique<BracketMatcher>(this);

    // Sensible defaults
    setTabStopDistance(fontMetrics().horizontalAdvance(QLatin1Char(' ')) * 4);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setFrameStyle(QFrame::NoFrame);

    connectInternalSignals();
}

void CodeEditorWidget::connectInternalSignals()
{
    // Update gutter width when block count changes
    connect(this, &QPlainTextEdit::blockCountChanged,
            this, [this](int) {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    });

    // Repaint gutter on scroll or cursor move
    connect(this, &QPlainTextEdit::updateRequest,
            this, [this](const QRect &rect, int dy) {
        if (dy) {
            m_lineNumberArea->scroll(0, dy);
        } else {
            m_lineNumberArea->update(0, rect.y(),
                                     m_lineNumberArea->width(), rect.height());
        }
        if (rect.contains(viewport()->rect())) {
            setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
        }
    });

    // Forward cursor-position changes
    connect(this, &QPlainTextEdit::cursorPositionChanged,
            this, [this]() {
        // Update line number area width in case line count changed
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
        m_lineNumberArea->update();
        
        // Highlight the current line
        QList<QTextEdit::ExtraSelection> extraSelections;
        QTextEdit::ExtraSelection selection;
        const QColor lineColor = palette().color(QPalette::AlternateBase);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);

        // Bracket matching highlights
        m_bracketMatcher->highlightMatchingBrackets(extraSelections);

        setExtraSelections(extraSelections);
        emit cursorActivity();
    });

    // Forward content changes
    connect(document(), &QTextDocument::contentsChanged,
            this, &CodeEditorWidget::contentChanged);

    // Clean / dirty tracking
    connect(document(), &QTextDocument::modificationChanged,
            this, [this](bool changed) {
        emit cleanChanged(!changed);
    });

    // Initial viewport margins - must be after all setup
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    m_lineNumberArea->update();
}

// ---------------------------------------------------------------------------
// Buffer management
// ---------------------------------------------------------------------------

std::shared_ptr<CodeEditorWidget> CodeEditorWidget::createBuffered(QWidget *parent)
{
    if (!s_editorBuffer.isEmpty()) {
        auto editor = s_editorBuffer.takeFirst();
        editor->setParent(parent);
        return editor;
    }
    return std::make_shared<CodeEditorWidget>(parent);
}

void CodeEditorWidget::warmBuffer(int count)
{
    for (int i = 0; i < count; ++i) {
        s_editorBuffer.append(std::make_shared<CodeEditorWidget>(nullptr));
    }
}

void CodeEditorWidget::invalidateBuffer()
{
    s_editorBuffer.clear();
}

// ---------------------------------------------------------------------------
// File path
// ---------------------------------------------------------------------------

void CodeEditorWidget::setFilePath(const QUrl &path)
{
    const QUrl old = m_filePath;
    m_filePath = path;
    if (old != path) {
        emit fileNameChanged(old, path);
    }
}

QUrl CodeEditorWidget::filePath() const { return m_filePath; }

bool CodeEditorWidget::fileOnDiskChanged() const { return m_fileOnDiskChanged; }
void CodeEditorWidget::setFileOnDiskChanged(bool changed) { m_fileOnDiskChanged = changed; }

// ---------------------------------------------------------------------------
// Content
// ---------------------------------------------------------------------------

void CodeEditorWidget::setContent(const QString &text)
{
    // Block signals to avoid spurious contentChanged during bulk load
    const QSignalBlocker blocker(document());
    setPlainText(text);
    document()->setModified(false);
}

QString CodeEditorWidget::content() const
{
    return toPlainText();
}

// ---------------------------------------------------------------------------
// Clean / dirty
// ---------------------------------------------------------------------------

bool CodeEditorWidget::isClean() const
{
    return !document()->isModified();
}

void CodeEditorWidget::markClean()
{
    document()->setModified(false);
}

void CodeEditorWidget::markDirty()
{
    document()->setModified(true);
}

// ---------------------------------------------------------------------------
// Language / highlighting
// ---------------------------------------------------------------------------

void CodeEditorWidget::setLanguage(const Language *lang)
{
    const Language *old = m_currentLanguage;
    m_currentLanguage = lang;

    if (m_highlighter && lang) {
        m_highlighter->setLanguage(lang);
    }

    // Apply default indentation for this language (unless user overrode it)
    if (!m_customIndentation && lang) {
        // Defaults are applied from LanguageManager's registry
    }

    if (old != lang && lang) {
        emit currentLanguageChanged(lang->id, lang->name);
    }
}

void CodeEditorWidget::setLanguage(const QString &languageId)
{
    setLanguage(LanguageManager::instance().lookupById(languageId));
}

void CodeEditorWidget::setLanguageFromFilePath(const QString &path)
{
    auto &lm = LanguageManager::instance();
    const Language *lang = lm.lookupByExtension(path);
    if (!lang) {
        lang = lm.lookupByFileName(QFileInfo(path).fileName());
    }
    if (lang) {
        setLanguage(lang);
    }
}

void CodeEditorWidget::setLanguageFromFilePath()
{
    if (m_filePath.isValid()) {
        setLanguageFromFilePath(m_filePath.toLocalFile());
    }
}

const Language *CodeEditorWidget::currentLanguage() const { return m_currentLanguage; }

// ---------------------------------------------------------------------------
// Indentation
// ---------------------------------------------------------------------------

void CodeEditorWidget::setCustomIndentation(bool useTabs, int size)
{
    m_customIndentation = true;
    m_indentation.useTabs = useTabs;
    if (size > 0) {
        m_indentation.size = size;
    }
    setTabStopDistance(fontMetrics().horizontalAdvance(QLatin1Char(' ')) * m_indentation.size);
}

void CodeEditorWidget::clearCustomIndentation()
{
    m_customIndentation = false;
}

bool CodeEditorWidget::isUsingCustomIndentation() const { return m_customIndentation; }
IndentationMode CodeEditorWidget::indentationMode() const { return m_indentation; }

std::optional<IndentationMode> CodeEditorWidget::detectDocumentIndentation() const
{
    // Heuristic: scan the first 100 lines for leading whitespace patterns
    int tabLines   = 0;
    int spaceLines = 0;
    int totalSpaces = 0;

    QTextBlock block = document()->begin();
    for (int i = 0; i < 100 && block.isValid(); ++i, block = block.next()) {
        const QString text = block.text();
        if (text.isEmpty()) continue;

        if (text.startsWith(QLatin1Char('\t'))) {
            ++tabLines;
        } else if (text.startsWith(QLatin1Char(' '))) {
            ++spaceLines;
            // Count leading spaces
            int spaces = 0;
            for (const QChar &ch : text) {
                if (ch == QLatin1Char(' ')) ++spaces;
                else break;
            }
            totalSpaces += spaces;
        }
    }

    if (tabLines == 0 && spaceLines == 0) {
        return std::nullopt; // Cannot determine
    }

    IndentationMode mode;
    mode.useTabs = tabLines > spaceLines;
    mode.size    = (spaceLines > 0) ? qMax(2, totalSpaces / spaceLines) : 4;
    return mode;
}

void CodeEditorWidget::setSmartIndent(bool enabled) { m_smartIndent = enabled; }

// ---------------------------------------------------------------------------
// Encoding / EOL
// ---------------------------------------------------------------------------

QTextCodec *CodeEditorWidget::codec() const { return m_codec; }
void CodeEditorWidget::setCodec(QTextCodec *codec) { m_codec = codec; }

bool CodeEditorWidget::hasBom() const { return m_bom; }
void CodeEditorWidget::setBom(bool bom) { m_bom = bom; }

QString CodeEditorWidget::eolSequence() const { return m_eolSequence; }
void CodeEditorWidget::setEolSequence(const QString &seq) { m_eolSequence = seq; }

// ---------------------------------------------------------------------------
// Visual settings
// ---------------------------------------------------------------------------

qreal CodeEditorWidget::zoomFactor() const { return m_zoomFactor; }

void CodeEditorWidget::setZoomFactor(qreal factor)
{
    m_zoomFactor = qBound(0.25, factor, 5.0);
    QFont f = font();
    f.setPointSizeF(f.pointSizeF() * m_zoomFactor);
    setFont(f);
}

void CodeEditorWidget::setEditorFont(const QString &family, int size, double /*lineHeight*/)
{
    QFont f = font();
    if (!family.isEmpty()) {
        f.setFamily(family);
    }
    if (size > 0) {
        f.setPointSize(size);
    }
    setFont(f);
}

void CodeEditorWidget::setLineNumbersVisible(bool visible)
{
    m_lineNumberArea->setVisible(visible);
    setViewportMargins(visible ? lineNumberAreaWidth() : 0, 0, 0, 0);
}

void CodeEditorWidget::setLineWrap(bool wrap)
{
    setLineWrapMode(wrap ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
}

void CodeEditorWidget::setShowWhitespace(bool show)
{
    QTextOption opt = document()->defaultTextOption();
    auto flags = opt.flags();
    if (show) {
        flags |= QTextOption::ShowTabsAndSpaces;
    } else {
        flags &= ~QTextOption::ShowTabsAndSpaces;
    }
    opt.setFlags(flags);
    document()->setDefaultTextOption(opt);
}

void CodeEditorWidget::setShowEndOfLine(bool show)
{
    QTextOption opt = document()->defaultTextOption();
    auto flags = opt.flags();
    if (show) {
        flags |= QTextOption::ShowLineAndParagraphSeparators;
    } else {
        flags &= ~QTextOption::ShowLineAndParagraphSeparators;
    }
    opt.setFlags(flags);
    document()->setDefaultTextOption(opt);
}

void CodeEditorWidget::setShowTabs(bool show)
{
    // Handled by setShowWhitespace — kept for API compat
    setShowWhitespace(show);
}

void CodeEditorWidget::setOverwriteMode(bool overwrite)
{
    QPlainTextEdit::setOverwriteMode(overwrite);
}

// ---------------------------------------------------------------------------
// Theme
// ---------------------------------------------------------------------------

void CodeEditorWidget::setTheme(const Theme & /*theme*/)
{
    // TODO: Apply colour scheme from theme file to palette + highlighter
}

QList<CodeEditorWidget::Theme> CodeEditorWidget::availableThemes() const
{
    // TODO: Enumerate installed theme files
    return { Theme("default") };
}

CodeEditorWidget::Theme CodeEditorWidget::themeFromName(const QString &name)
{
    return Theme(name);
}

// ---------------------------------------------------------------------------
// Cursor
// ---------------------------------------------------------------------------

CursorPosition CodeEditorWidget::cursorPos() const
{
    const QTextCursor tc = textCursor();
    return { tc.blockNumber(), tc.positionInBlock() };
}

void CodeEditorWidget::setCursorPos(int line, int column)
{
    const QTextBlock block = document()->findBlockByNumber(line);
    if (!block.isValid()) return;

    QTextCursor tc(block);
    tc.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,
                    qMin(column, block.length() - 1));
    setTextCursor(tc);
    centerCursor();
}

void CodeEditorWidget::setCursorPos(const CursorPosition &pos)
{
    setCursorPos(pos.line, pos.column);
}

QPair<int, int> CodeEditorWidget::scrollPosition() const
{
    return { horizontalScrollBar()->value(), verticalScrollBar()->value() };
}

void CodeEditorWidget::setScrollPosition(int left, int top)
{
    horizontalScrollBar()->setValue(left);
    verticalScrollBar()->setValue(top);
}

// ---------------------------------------------------------------------------
// Selections
// ---------------------------------------------------------------------------

QList<TextSelection> CodeEditorWidget::selections() const
{
    const QTextCursor tc = textCursor();
    if (!tc.hasSelection()) return {};

    TextSelection sel;
    sel.from.line   = document()->findBlock(tc.selectionStart()).blockNumber();
    sel.from.column = tc.selectionStart() - document()->findBlock(tc.selectionStart()).position();
    sel.to.line     = document()->findBlock(tc.selectionEnd()).blockNumber();
    sel.to.column   = tc.selectionEnd() - document()->findBlock(tc.selectionEnd()).position();
    return { sel };
}

QStringList CodeEditorWidget::selectedTexts() const
{
    const QTextCursor tc = textCursor();
    if (!tc.hasSelection()) return {};
    return { tc.selectedText() };
}

void CodeEditorWidget::setSelectionsText(const QStringList &texts)
{
    if (texts.isEmpty()) return;
    QTextCursor tc = textCursor();
    tc.insertText(texts.first());
    setTextCursor(tc);
}

void CodeEditorWidget::setSelection(int fromLine, int fromCol, int toLine, int toCol)
{
    QTextCursor tc(document());
    const QTextBlock fromBlock = document()->findBlockByNumber(fromLine);
    tc.setPosition(fromBlock.position() + fromCol);
    const QTextBlock toBlock = document()->findBlockByNumber(toLine);
    tc.setPosition(toBlock.position() + toCol, QTextCursor::KeepAnchor);
    setTextCursor(tc);
}

// ---------------------------------------------------------------------------
// Document info
// ---------------------------------------------------------------------------

int CodeEditorWidget::lineCount() const
{
    return document()->blockCount();
}

QString CodeEditorWidget::currentWord() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

// ---------------------------------------------------------------------------
// Banners
// ---------------------------------------------------------------------------

void CodeEditorWidget::insertBanner(QWidget *banner)
{
    if (!banner) return;
    banner->setParent(this);
    banner->show();
    // Position above the viewport
    banner->move(0, 0);
    setViewportMargins(lineNumberAreaWidth(), banner->height(), 0, 0);
}

void CodeEditorWidget::removeBanner(QWidget *banner)
{
    if (!banner) return;
    banner->hide();
    banner->setParent(nullptr);
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    emit bannerRemoved(banner);
}

void CodeEditorWidget::removeBanner(const QString &objectName)
{
    for (auto *child : findChildren<QWidget *>(objectName)) {
        removeBanner(child);
    }
}

// ---------------------------------------------------------------------------
// Printing
// ---------------------------------------------------------------------------

void CodeEditorWidget::print(QPrinter *printer)
{
    document()->print(printer);
}

QByteArray CodeEditorWidget::printToPdf(const QPageLayout & /*layout*/) const
{
    // TODO: Implement PDF export using QPdfWriter
    return {};
}

// ---------------------------------------------------------------------------
// Line-number area helpers
// ---------------------------------------------------------------------------

int CodeEditorWidget::lineNumberAreaWidth() const
{
    if (m_lineNumberArea && !m_lineNumberArea->isVisible()) {
        return 0;
    }

    int digits = 1;
    int maxBlock = qMax(1, blockCount());
    while (maxBlock >= 10) {
        maxBlock /= 10;
        ++digits;
    }
    digits = qMax(digits, 3); // minimum 3 digits wide

    const int space = 8 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditorWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), palette().color(QPalette::Window));

    QTextBlock block = firstVisibleBlock();
    int blockNumber  = block.blockNumber();
    int top          = qRound(blockBoundingGeometry(block)
                                  .translated(contentOffset()).top());
    int bottom       = top + qRound(blockBoundingRect(block).height());

    const int currentLine = textCursor().blockNumber();
    const QColor normalColor  = palette().color(QPalette::PlaceholderText);
    const QColor activeColor  = palette().color(QPalette::Text);

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const QString number = QString::number(blockNumber + 1);
            painter.setPen(blockNumber == currentLine ? activeColor : normalColor);
            painter.drawText(0, top,
                             m_lineNumberArea->width() - 4,
                             fontMetrics().height(),
                             Qt::AlignRight | Qt::AlignVCenter,
                             number);
        }

        block  = block.next();
        top    = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

// ---------------------------------------------------------------------------
// Event overrides
// ---------------------------------------------------------------------------

void CodeEditorWidget::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    const QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(
        QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditorWidget::keyPressEvent(QKeyEvent *event)
{
    // Handle Tab / Shift+Tab for indentation
    if (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab) {
        handleTabKey(event);
        return;
    }

    // Handle Enter with auto-indent
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        && m_smartIndent) {
        handleAutoIndent(event);
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
}

void CodeEditorWidget::wheelEvent(QWheelEvent *event)
{
    // Ctrl+Wheel → zoom
    if (event->modifiers() & Qt::ControlModifier) {
        const int delta = event->angleDelta().y();
        if (delta > 0) {
            setZoomFactor(m_zoomFactor + 0.1);
        } else if (delta < 0) {
            setZoomFactor(m_zoomFactor - 0.1);
        }
        event->accept();
        emit mouseWheelUsed(event);
        return;
    }
    QPlainTextEdit::wheelEvent(event);
}

void CodeEditorWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        QPlainTextEdit::dragEnterEvent(event);
    }
}

void CodeEditorWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        emit urlsDropped(event->mimeData()->urls());
        event->acceptProposedAction();
    } else {
        QPlainTextEdit::dropEvent(event);
    }
}

void CodeEditorWidget::paintEvent(QPaintEvent *event)
{
    QPlainTextEdit::paintEvent(event);
}

// ---------------------------------------------------------------------------
// Auto-indent
// ---------------------------------------------------------------------------

void CodeEditorWidget::handleAutoIndent(QKeyEvent * /*event*/)
{
    QTextCursor tc = textCursor();
    const QString currentLineText = tc.block().text();

    // Extract leading whitespace from the current line
    QString indent;
    for (const QChar &ch : currentLineText) {
        if (ch == QLatin1Char(' ') || ch == QLatin1Char('\t')) {
            indent += ch;
        } else {
            break;
        }
    }

    // Insert newline + matching indentation
    tc.insertText(QStringLiteral("\n") + indent);
    setTextCursor(tc);
    ensureCursorVisible();
}

void CodeEditorWidget::handleTabKey(QKeyEvent *event)
{
    QTextCursor tc = textCursor();

    if (event->key() == Qt::Key_Backtab) {
        // Shift+Tab: unindent selected lines
        if (tc.hasSelection()) {
            const int start = document()->findBlock(tc.selectionStart()).blockNumber();
            const int end   = document()->findBlock(tc.selectionEnd()).blockNumber();
            tc.beginEditBlock();
            for (int i = start; i <= end; ++i) {
                QTextBlock block = document()->findBlockByNumber(i);
                const QString text = block.text();
                QTextCursor blockCursor(block);
                if (text.startsWith(QLatin1Char('\t'))) {
                    blockCursor.deleteChar();
                } else {
                    int spaces = 0;
                    for (const QChar &ch : text) {
                        if (ch == QLatin1Char(' ') && spaces < m_indentation.size) {
                            ++spaces;
                        } else {
                            break;
                        }
                    }
                    for (int s = 0; s < spaces; ++s) {
                        blockCursor.deleteChar();
                    }
                }
            }
            tc.endEditBlock();
        }
        return;
    }

    // Tab: indent
    if (tc.hasSelection()) {
        // Indent selected lines
        const int start = document()->findBlock(tc.selectionStart()).blockNumber();
        const int end   = document()->findBlock(tc.selectionEnd()).blockNumber();
        const QString indentStr = m_indentation.useTabs
                                      ? QStringLiteral("\t")
                                      : QString(m_indentation.size, QLatin1Char(' '));
        tc.beginEditBlock();
        for (int i = start; i <= end; ++i) {
            QTextCursor blockCursor(document()->findBlockByNumber(i));
            blockCursor.insertText(indentStr);
        }
        tc.endEditBlock();
    } else {
        // Insert indent at cursor
        if (m_indentation.useTabs) {
            tc.insertText(QStringLiteral("\t"));
        } else {
            tc.insertText(QString(m_indentation.size, QLatin1Char(' ')));
        }
        setTextCursor(tc);
    }
}

// ---------------------------------------------------------------------------
// Tab name (internal)
// ---------------------------------------------------------------------------

QString CodeEditorWidget::tabName() const { return m_tabName; }
void CodeEditorWidget::setTabName(const QString &name) { m_tabName = name; }

} // namespace NovaPad::Editor
