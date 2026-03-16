/**
 * @file line_number_area.h
 * @brief Line-number gutter widget drawn alongside the code editor.
 *
 * This is a lightweight companion widget that delegates all painting to
 * CodeEditorWidget::lineNumberAreaPaintEvent().  It exists as a separate
 * QWidget so that Qt's layout system can manage its geometry automatically.
 */

#ifndef NOVAPAD_EDITOR_LINE_NUMBER_AREA_H
#define NOVAPAD_EDITOR_LINE_NUMBER_AREA_H

#include <QWidget>

namespace NovaPad::Editor {

class CodeEditorWidget; // forward declaration

/**
 * @class LineNumberArea
 * @brief Thin widget that renders line numbers in the editor gutter.
 *
 * All rendering logic lives in CodeEditorWidget — this class merely
 * forwards sizeHint() and paintEvent() calls.
 */
class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    explicit LineNumberArea(CodeEditorWidget *editor);

    /** @brief Returns the preferred width based on the editor's digit count. */
    [[nodiscard]] QSize sizeHint() const override;

protected:
    /** @brief Delegates painting to CodeEditorWidget::lineNumberAreaPaintEvent(). */
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditorWidget *m_editor; ///< Owning editor (not owned — parent manages lifetime)
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_LINE_NUMBER_AREA_H
