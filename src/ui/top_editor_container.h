/**
 * @file top_editor_container.h
 * @brief Split-view container managing one or more EditorTabWidgets.
 *
 * Allows the user to split the editor area horizontally or vertically,
 * showing multiple files side by side.  This is the C++23 equivalent
 * of Notepadqq's TopEditorContainer (QSplitter-based).
 */

#ifndef NOVAPAD_UI_TOP_EDITOR_CONTAINER_H
#define NOVAPAD_UI_TOP_EDITOR_CONTAINER_H

#include "editor/code_editor_widget.h"
#include "ui/editor_tab_widget.h"

#include <QSplitter>
#include <QWheelEvent>

#include <functional>
#include <memory>
#include <vector>

namespace NovaPad::UI {

/**
 * @class TopEditorContainer
 * @brief QSplitter subclass that hosts one or more EditorTabWidgets.
 *
 * The container maintains a "current" tab widget — the one that most
 * recently received focus.  Signals from all child tab widgets are
 * aggregated and re-emitted so that MainWindow only needs to connect
 * to a single source.
 */
class TopEditorContainer : public QSplitter
{
    Q_OBJECT

public:
    explicit TopEditorContainer(QWidget *parent = nullptr);

    // -- Tab widget management -----------------------------------------------

    /** @brief Creates and appends a new EditorTabWidget to this container. */
    EditorTabWidget *addTabWidget();

    /** @brief Returns the tab widget at the given splitter index. */
    [[nodiscard]] EditorTabWidget *tabWidget(int index) const;

    /** @brief Returns the tab widget that currently has focus. */
    [[nodiscard]] EditorTabWidget *currentTabWidget() const;

    /**
     * @brief Returns one of the first two tab widgets that is NOT the current one.
     * @param createIfNotExists  If true, creates a second widget if only one exists.
     */
    [[nodiscard]] EditorTabWidget *inactiveTabWidget(bool createIfNotExists);

    /** @brief Returns the tab widget that contains the given editor. */
    [[nodiscard]] EditorTabWidget *tabWidgetFromEditor(
        std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor) const;

    // -- Iteration -----------------------------------------------------------

    /**
     * @brief Executes @p callback for every editor in every tab widget.
     * @param backwardIndexes  If true, iterates in reverse (safe for deletion).
     * @param callback  Returns true to continue, false to break.
     */
    void forEachEditor(
        bool backwardIndexes,
        std::function<bool(int twIdx, int tabIdx,
                           EditorTabWidget *tw,
                           std::shared_ptr<NovaPad::Editor::CodeEditorWidget>)> callback);

    /** @brief Convenience overload — forward iteration. */
    void forEachEditor(
        std::function<bool(int twIdx, int tabIdx,
                           EditorTabWidget *tw,
                           std::shared_ptr<NovaPad::Editor::CodeEditorWidget>)> callback);

    /** @brief Returns all open editors across all tab widgets. */
    [[nodiscard]] std::vector<std::shared_ptr<NovaPad::Editor::CodeEditorWidget>> allEditors() const;

    /** @brief Total number of editors across all tab widgets. */
    [[nodiscard]] int editorCount() const;

    /** @brief Removes @p tw if it has no tabs and is not the last widget. */
    void removeTabWidgetIfEmpty(EditorTabWidget *tw);

    /**
     * @brief Disconnects all signals — call only during shutdown.
     *
     * After this call the container is no longer usable.
     */
    void disconnectAllTabWidgets();

signals:
    void currentTabChanged(EditorTabWidget *tabWidget, int tab);
    void currentTabWidgetChanged(EditorTabWidget *tabWidget);
    void currentEditorChanged(EditorTabWidget *tabWidget, int tab);
    void customTabContextMenuRequested(QPoint point, EditorTabWidget *tabWidget, int tab);
    void tabCloseRequested(EditorTabWidget *tabWidget, int tab);
    void editorAdded(EditorTabWidget *tabWidget, int tab);
    void editorMouseWheel(EditorTabWidget *tabWidget, int tab, QWheelEvent *ev);
    void tabBarDoubleClicked(EditorTabWidget *tabWidget, int tab);

private slots:
    void onCurrentTabChanged(int index);
    void onCurrentTabWidgetChanged();
    void onCustomContextMenuRequested(QPoint point);
    void onTabCloseRequested(int index);
    void onEditorAdded(int tab);

private:
    /** @brief Connects all necessary signals from @p tw to the aggregation layer. */
    void connectTabWidget(EditorTabWidget *tw);

    EditorTabWidget *m_currentTabWidget = nullptr;
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_TOP_EDITOR_CONTAINER_H
