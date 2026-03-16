/**
 * @file line_number_area.cpp
 * @brief Implementation of the line-number gutter widget.
 */

#include "line_number_area.h"
#include "code_editor_widget.h"

namespace NovaPad::Editor {

LineNumberArea::LineNumberArea(CodeEditorWidget *editor)
    : QWidget(editor)
    , m_editor(editor)
{
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(m_editor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    m_editor->lineNumberAreaPaintEvent(event);
}

} // namespace NovaPad::Editor
