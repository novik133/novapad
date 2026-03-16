/**
 * @file minimap_widget.h
 * @brief A zoomed-out document overview (minimap) rendered alongside the editor.
 *
 * The minimap gives programmers a bird's-eye view of the entire file,
 * similar to the feature found in VS Code and Sublime Text.  Clicking or
 * dragging within the minimap scrolls the main editor to the corresponding
 * position.
 */

#ifndef NOVAPAD_EDITOR_MINIMAP_WIDGET_H
#define NOVAPAD_EDITOR_MINIMAP_WIDGET_H

#include <QWidget>

namespace NovaPad::Editor {

class CodeEditorWidget; // forward declaration

/**
 * @class MinimapWidget
 * @brief Renders a scaled-down preview of the document text.
 *
 * The minimap is intentionally kept simple: it paints each character as a
 * single-pixel-wide dot using the syntax-highlighting colours, and overlays
 * a translucent viewport indicator showing the currently visible region.
 */
class MinimapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MinimapWidget(CodeEditorWidget *editor, QWidget *parent = nullptr);
    ~MinimapWidget() override = default;

    /** @brief Preferred width — typically 80–120 px. */
    [[nodiscard]] QSize sizeHint() const override;

    /** @brief Enable or disable the minimap at runtime. */
    void setEnabled(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    /** @brief Scrolls the editor so that the given minimap Y coordinate is centred. */
    void scrollEditorToY(int y);

    CodeEditorWidget *m_editor = nullptr;
    bool              m_active = true;
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_MINIMAP_WIDGET_H
