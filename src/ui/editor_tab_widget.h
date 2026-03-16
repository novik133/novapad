/**
 * @file editor_tab_widget.h
 * @brief Tab widget hosting multiple CodeEditorWidget instances.
 *
 * Each tab contains one editor.  The widget supports tab reordering via
 * drag-and-drop, tab transfer between EditorTabWidgets (for split views),
 * and per-tab saved/unsaved indicators.
 *
 * This is the C++23 / Qt 6.10 equivalent of Notepadqq's EditorTabWidget.
 */

#ifndef NOVAPAD_UI_EDITOR_TAB_WIDGET_H
#define NOVAPAD_UI_EDITOR_TAB_WIDGET_H

#include "editor/code_editor_widget.h"

#include <QHash>
#include <QTabWidget>
#include <QWheelEvent>

#include <memory>

namespace NovaPad::UI {

/**
 * @class EditorTabWidget
 * @brief QTabWidget subclass specialised for hosting code editors.
 *
 * Manages a collection of CodeEditorWidgets, each identified by its tab
 * index.  Shared pointers ensure that an editor's lifetime is tied to
 * the tab widget, while still allowing external code to hold weak
 * references when needed.
 */
class EditorTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit EditorTabWidget(QWidget *parent = nullptr);
    ~EditorTabWidget() override;

    // -- Tab management ------------------------------------------------------

    /**
     * @brief Creates a new editor tab with the given title.
     * @param setFocus  If true, the new tab receives focus immediately.
     * @param title     Tab title (e.g. "New 1" or a file name).
     * @return The index of the new tab.
     */
    int addEditorTab(bool setFocus, const QString &title);

    /**
     * @brief Moves an editor tab from @p source into this widget.
     * @param setFocus      Give focus to the transferred tab.
     * @param source        The originating EditorTabWidget.
     * @param tabIndex      Index of the tab in @p source.
     * @return Index of the new tab in this widget.
     */
    int transferEditorTab(bool setFocus, EditorTabWidget *source, int tabIndex);

    /** @brief Searches for an open editor with the given URL. Returns -1 if not found. */
    [[nodiscard]] int findEditorByUrl(const QUrl &url) const;

    // -- Editor access -------------------------------------------------------

    /** @brief Returns the editor at the given tab index. */
    [[nodiscard]] std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor(int index) const;

    /** @brief Returns the currently active editor. */
    [[nodiscard]] std::shared_ptr<NovaPad::Editor::CodeEditorWidget> currentEditor() const;

    // -- Tab text helpers ----------------------------------------------------

    /** @brief Returns the tab title for the given index. */
    [[nodiscard]] QString tabText(int index) const;

    /** @brief Sets the tab title for the given index. */
    void setTabText(int index, const QString &text);

    /** @brief Generates a suitable tab title from a URL. */
    [[nodiscard]] QString generateTabTitle(const QUrl &url) const;

    // -- Zoom ----------------------------------------------------------------

    [[nodiscard]] qreal zoomFactor() const;
    void setZoomFactor(qreal factor);

    // -- Saved/unsaved icon --------------------------------------------------

    /** @brief Updates the tab icon to reflect saved/unsaved state. */
    void setSavedIcon(int index, bool saved);

    // -- Former-tab tracking -------------------------------------------------

    /** @brief Returns the index of the previously active tab. */
    [[nodiscard]] int formerTabIndex() const;

    // -- Cleanup -------------------------------------------------------------

    /** @brief Deletes this widget if it has no remaining tabs. */
    void deleteIfEmpty();

signals:
    void gotFocus();
    void editorAdded(int index);
    void editorMouseWheel(int tab, QWheelEvent *ev);

protected:
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void tabRemoved(int index) override;

private slots:
    void onCleanChanged(bool isClean);
    void onEditorMouseWheel(QWheelEvent *ev);
    void onFileNameChanged(const QUrl &oldName, const QUrl &newName);
    void onCurrentTabChanged(int index);

private:
    /** @brief Internal: inserts or transfers a tab. */
    int rawAddEditorTab(bool setFocus, const QString &title,
                        EditorTabWidget *source, int sourceTabIndex);

    /** @brief Wires editor signals for the tab at the given index. */
    void connectEditorSignals(NovaPad::Editor::CodeEditorWidget *editor);
    void disconnectEditorSignals(NovaPad::Editor::CodeEditorWidget *editor);

    // -- Per-tab editor storage (shared ownership) ---------------------------
    QHash<NovaPad::Editor::CodeEditorWidget *,
          std::shared_ptr<NovaPad::Editor::CodeEditorWidget>> m_editors;

    qreal m_zoomFactor       = 1.0;
    int   m_formerTabIndex   = 0;
    int   m_mostRecentIndex  = 0;
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_EDITOR_TAB_WIDGET_H
