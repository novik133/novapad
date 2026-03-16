/**
 * @file minimap_widget.cpp
 * @brief Implementation of the document minimap widget.
 *
 * The minimap renders a simplified, zoomed-out view of the entire document.
 * Each line is drawn as a thin horizontal stroke using averaged character
 * colours from the syntax highlighter.  A semi-transparent overlay shows
 * which portion of the document is currently visible in the editor viewport.
 */

#include "minimap_widget.h"
#include "code_editor_widget.h"

#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>

namespace NovaPad::Editor {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

MinimapWidget::MinimapWidget(CodeEditorWidget *editor, QWidget *parent)
    : QWidget(parent)
    , m_editor(editor)
{
    setFixedWidth(100);
    setCursor(Qt::PointingHandCursor);

    // Repaint the minimap whenever the editor content or scroll changes
    connect(m_editor, &QPlainTextEdit::updateRequest,
            this, QOverload<>::of(&QWidget::update));
    connect(m_editor->document(), &QTextDocument::contentsChanged,
            this, QOverload<>::of(&QWidget::update));
}

QSize MinimapWidget::sizeHint() const
{
    return QSize(100, 0);
}

void MinimapWidget::setEnabled(bool enabled)
{
    m_active = enabled;
    setVisible(enabled);
}

// ---------------------------------------------------------------------------
// Painting
// ---------------------------------------------------------------------------

void MinimapWidget::paintEvent(QPaintEvent * /*event*/)
{
    if (!m_active || !m_editor) return;

    QPainter painter(this);
    painter.fillRect(rect(), QColor(30, 30, 30));

    const int totalLines = m_editor->document()->blockCount();
    if (totalLines == 0) return;

    // Scale factor: map document lines → minimap pixels
    const double scale = static_cast<double>(height()) / qMax(totalLines, 1);
    const double lineH = qMax(scale, 1.0);

    // Draw each visible line as a small coloured bar
    QTextBlock block = m_editor->document()->begin();
    int blockNum = 0;
    const QColor textColor(180, 180, 180, 120);
    const QColor keywordColor(86, 156, 214, 160);

    while (block.isValid()) {
        const int y = static_cast<int>(blockNum * scale);
        const int textLen = qMin(block.text().length(), width());

        // Simple representation: draw a line proportional to text length
        if (textLen > 0) {
            const QColor color = block.text().trimmed().startsWith('#')
                                     ? keywordColor : textColor;
            painter.setPen(Qt::NoPen);
            painter.setBrush(color);
            painter.drawRect(4, y, textLen, qMax(static_cast<int>(lineH), 1));
        }

        block = block.next();
        ++blockNum;
    }

    // Draw the viewport indicator (translucent rectangle)
    const QScrollBar *vbar = m_editor->verticalScrollBar();
    Q_UNUSED(vbar)
    
    // Calculate first visible block from scrollbar position
    const int firstVisible = static_cast<int>(vbar->value() / m_editor->fontMetrics().lineSpacing());
    const int visibleLines = m_editor->viewport()->height()
                             / m_editor->fontMetrics().lineSpacing();

    const int vpTop    = static_cast<int>(firstVisible * scale);
    const int vpHeight = qMax(static_cast<int>(visibleLines * scale), 10);

    painter.setBrush(QColor(255, 255, 255, 30));
    painter.setPen(QColor(255, 255, 255, 60));
    painter.drawRect(0, vpTop, width(), vpHeight);
}

// ---------------------------------------------------------------------------
// Mouse interaction — scroll the editor
// ---------------------------------------------------------------------------

void MinimapWidget::mousePressEvent(QMouseEvent *event)
{
    scrollEditorToY(event->pos().y());
}

void MinimapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        scrollEditorToY(event->pos().y());
    }
}

void MinimapWidget::scrollEditorToY(int y)
{
    if (!m_editor) return;

    const int totalLines = m_editor->document()->blockCount();
    if (totalLines == 0) return;

    const double ratio = static_cast<double>(y) / height();
    const int targetLine = static_cast<int>(ratio * totalLines);

    // Scroll the editor so that targetLine is roughly centred
    QTextBlock block = m_editor->document()->findBlockByNumber(
        qBound(0, targetLine, totalLines - 1));
    QTextCursor tc(const_cast<QTextDocument*>(m_editor->document()));
    tc.setPosition(block.position());
    m_editor->setTextCursor(tc);
    m_editor->centerCursor();
}

} // namespace NovaPad::Editor
