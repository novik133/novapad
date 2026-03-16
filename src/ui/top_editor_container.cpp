/**
 * @file top_editor_container.cpp
 * @brief Implementation of the split-view editor container.
 *
 * The container uses a QSplitter layout so that users can drag the divider
 * between two tab widgets to resize them.  Focus tracking ensures that
 * keyboard shortcuts and menu actions always target the correct editor.
 */

#include "top_editor_container.h"

#include <QTabBar>

namespace NovaPad::UI {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TopEditorContainer::TopEditorContainer(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
{
    setChildrenCollapsible(false);
}

// ---------------------------------------------------------------------------
// Tab widget management
// ---------------------------------------------------------------------------

EditorTabWidget *TopEditorContainer::addTabWidget()
{
    auto *tw = new EditorTabWidget(this);
    addWidget(tw);
    connectTabWidget(tw);

    if (!m_currentTabWidget) {
        m_currentTabWidget = tw;
    }

    return tw;
}

EditorTabWidget *TopEditorContainer::tabWidget(int index) const
{
    if (index < 0 || index >= count()) return nullptr;
    return qobject_cast<EditorTabWidget *>(widget(index));
}

EditorTabWidget *TopEditorContainer::currentTabWidget() const
{
    return m_currentTabWidget;
}

EditorTabWidget *TopEditorContainer::inactiveTabWidget(bool createIfNotExists)
{
    // Return the first tab widget that is NOT the current one
    for (int i = 0; i < count(); ++i) {
        auto *tw = tabWidget(i);
        if (tw && tw != m_currentTabWidget) {
            return tw;
        }
    }

    // Only one tab widget exists — create a second if requested
    if (createIfNotExists) {
        return addTabWidget();
    }
    return nullptr;
}

EditorTabWidget *TopEditorContainer::tabWidgetFromEditor(
    std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor) const
{
    if (!editor) return nullptr;

    for (int i = 0; i < count(); ++i) {
        auto *tw = tabWidget(i);
        if (!tw) continue;
        for (int t = 0; t < tw->count(); ++t) {
            if (tw->editor(t) == editor) {
                return tw;
            }
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// Iteration
// ---------------------------------------------------------------------------

void TopEditorContainer::forEachEditor(
    bool backwardIndexes,
    std::function<bool(int, int, EditorTabWidget *,
                       std::shared_ptr<NovaPad::Editor::CodeEditorWidget>)> callback)
{
    if (backwardIndexes) {
        for (int i = count() - 1; i >= 0; --i) {
            auto *tw = tabWidget(i);
            if (!tw) continue;
            for (int t = tw->count() - 1; t >= 0; --t) {
                if (!callback(i, t, tw, tw->editor(t))) return;
            }
        }
    } else {
        for (int i = 0; i < count(); ++i) {
            auto *tw = tabWidget(i);
            if (!tw) continue;
            for (int t = 0; t < tw->count(); ++t) {
                if (!callback(i, t, tw, tw->editor(t))) return;
            }
        }
    }
}

void TopEditorContainer::forEachEditor(
    std::function<bool(int, int, EditorTabWidget *,
                       std::shared_ptr<NovaPad::Editor::CodeEditorWidget>)> callback)
{
    forEachEditor(false, std::move(callback));
}

std::vector<std::shared_ptr<NovaPad::Editor::CodeEditorWidget>>
TopEditorContainer::allEditors() const
{
    std::vector<std::shared_ptr<NovaPad::Editor::CodeEditorWidget>> result;
    for (int i = 0; i < count(); ++i) {
        auto *tw = tabWidget(i);
        if (!tw) continue;
        for (int t = 0; t < tw->count(); ++t) {
            auto ed = tw->editor(t);
            if (ed) result.push_back(std::move(ed));
        }
    }
    return result;
}

int TopEditorContainer::editorCount() const
{
    int total = 0;
    for (int i = 0; i < count(); ++i) {
        auto *tw = tabWidget(i);
        if (tw) total += tw->count();
    }
    return total;
}

void TopEditorContainer::removeTabWidgetIfEmpty(EditorTabWidget *tw)
{
    if (!tw || tw->count() > 0) return;
    if (count() <= 1) return; // Keep at least one tab widget

    if (m_currentTabWidget == tw) {
        // Switch focus to the remaining tab widget
        m_currentTabWidget = tabWidget(0) == tw ? tabWidget(1) : tabWidget(0);
    }

    tw->deleteLater();
}

void TopEditorContainer::disconnectAllTabWidgets()
{
    for (int i = 0; i < count(); ++i) {
        auto *tw = tabWidget(i);
        if (tw) tw->disconnect();
    }
}

// ---------------------------------------------------------------------------
// Signal aggregation — connects child tab widget signals to container signals
// ---------------------------------------------------------------------------

void TopEditorContainer::connectTabWidget(EditorTabWidget *tw)
{
    connect(tw, &QTabWidget::currentChanged,
            this, &TopEditorContainer::onCurrentTabChanged);
    connect(tw, &EditorTabWidget::gotFocus,
            this, &TopEditorContainer::onCurrentTabWidgetChanged);
    connect(tw, &EditorTabWidget::editorAdded,
            this, &TopEditorContainer::onEditorAdded);
    connect(tw, &QTabWidget::tabCloseRequested,
            this, &TopEditorContainer::onTabCloseRequested);

    tw->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tw, &QWidget::customContextMenuRequested,
            this, &TopEditorContainer::onCustomContextMenuRequested);
}

// ---------------------------------------------------------------------------
// Private slots
// ---------------------------------------------------------------------------

void TopEditorContainer::onCurrentTabChanged(int index)
{
    auto *tw = qobject_cast<EditorTabWidget *>(sender());
    if (tw && tw == m_currentTabWidget) {
        emit currentTabChanged(tw, index);
        emit currentEditorChanged(tw, index);
    }
}

void TopEditorContainer::onCurrentTabWidgetChanged()
{
    auto *tw = qobject_cast<EditorTabWidget *>(sender());
    if (tw && tw != m_currentTabWidget) {
        m_currentTabWidget = tw;
        emit currentTabWidgetChanged(tw);
        emit currentEditorChanged(tw, tw->currentIndex());
    }
}

void TopEditorContainer::onCustomContextMenuRequested(QPoint point)
{
    auto *tw = qobject_cast<EditorTabWidget *>(sender());
    if (!tw) return;
    // tabBar() is protected in QTabWidget, use the point directly since customContextMenuRequested
    // is triggered on the tab widget itself and we can use the tabBar's geometry
    int tab = -1;
    if (QTabBar *tb = tw->findChild<QTabBar*>()) {
        tab = tb->tabAt(tb->mapFromGlobal(tw->mapToGlobal(point)));
    }
    if (tab >= 0) {
        emit customTabContextMenuRequested(point, tw, tab);
    }
}

void TopEditorContainer::onTabCloseRequested(int index)
{
    auto *tw = qobject_cast<EditorTabWidget *>(sender());
    if (tw) emit tabCloseRequested(tw, index);
}

void TopEditorContainer::onEditorAdded(int tab)
{
    auto *tw = qobject_cast<EditorTabWidget *>(sender());
    if (tw) emit editorAdded(tw, tab);
}

} // namespace NovaPad::UI
