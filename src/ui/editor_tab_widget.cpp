/**
 * @file editor_tab_widget.cpp
 * @brief Implementation of the editor tab widget.
 *
 * Each tab hosts a CodeEditorWidget via shared pointer.  The tab bar
 * shows a dot icon for unsaved tabs and the file name for saved ones.
 * Tabs can be reordered by dragging, and transferred between tab widgets
 * for split-view editing.
 */

#include "editor_tab_widget.h"

#include <QFileInfo>
#include <QMouseEvent>
#include <QTabBar>

namespace NovaPad::UI {

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

EditorTabWidget::EditorTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    setElideMode(Qt::ElideRight);

    connect(this, &QTabWidget::currentChanged,
            this, &EditorTabWidget::onCurrentTabChanged);
}

EditorTabWidget::~EditorTabWidget()
{
    m_editors.clear();
}

// ---------------------------------------------------------------------------
// Tab management
// ---------------------------------------------------------------------------

int EditorTabWidget::addEditorTab(bool setFocus, const QString &title)
{
    return rawAddEditorTab(setFocus, title, nullptr, -1);
}

int EditorTabWidget::transferEditorTab(bool setFocus, EditorTabWidget *source, int tabIndex)
{
    return rawAddEditorTab(setFocus, {}, source, tabIndex);
}

int EditorTabWidget::rawAddEditorTab(bool setFocus, const QString &title,
                                     EditorTabWidget *source, int sourceTabIndex)
{
    std::shared_ptr<NovaPad::Editor::CodeEditorWidget> ed;

    if (source && sourceTabIndex >= 0) {
        // Transfer an existing editor from another tab widget
        ed = source->editor(sourceTabIndex);
        if (!ed) return -1;

        // Disconnect signals from the source and remove the tab (without deleting)
        source->disconnectEditorSignals(ed.get());
        source->m_editors.remove(ed.get());
        // Block signals while removing to prevent unwanted side effects
        const QSignalBlocker blocker(source);
        source->removeTab(sourceTabIndex);
    } else {
        // Create a brand-new editor using the pre-allocation buffer
        ed = NovaPad::Editor::CodeEditorWidget::createBuffered(this);
    }

    // Register the editor in our ownership map
    m_editors.insert(ed.get(), ed);

    // Add the tab
    const QString tabTitle = title.isEmpty()
                                 ? (ed->filePath().isValid()
                                        ? generateTabTitle(ed->filePath())
                                        : tr("New"))
                                 : title;

    const int idx = QTabWidget::addTab(ed.get(), tabTitle);

    connectEditorSignals(ed.get());
    setSavedIcon(idx, ed->isClean());

    if (setFocus) {
        setCurrentIndex(idx);
        ed->setFocus();
    }

    emit editorAdded(idx);
    return idx;
}

int EditorTabWidget::findEditorByUrl(const QUrl &url) const
{
    for (int i = 0; i < count(); ++i) {
        auto ed = editor(i);
        if (ed && ed->filePath() == url) return i;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Editor access
// ---------------------------------------------------------------------------

std::shared_ptr<NovaPad::Editor::CodeEditorWidget> EditorTabWidget::editor(int index) const
{
    auto *w = qobject_cast<NovaPad::Editor::CodeEditorWidget *>(widget(index));
    if (!w) return nullptr;
    auto it = m_editors.find(w);
    return (it != m_editors.end()) ? it.value() : nullptr;
}

std::shared_ptr<NovaPad::Editor::CodeEditorWidget> EditorTabWidget::currentEditor() const
{
    return editor(currentIndex());
}

// ---------------------------------------------------------------------------
// Tab text helpers
// ---------------------------------------------------------------------------

QString EditorTabWidget::tabText(int index) const
{
    return QTabWidget::tabText(index);
}

void EditorTabWidget::setTabText(int index, const QString &text)
{
    QTabWidget::setTabText(index, text);
}

QString EditorTabWidget::generateTabTitle(const QUrl &url) const
{
    if (url.isLocalFile()) {
        return QFileInfo(url.toLocalFile()).fileName();
    }
    return url.toString();
}

// ---------------------------------------------------------------------------
// Zoom
// ---------------------------------------------------------------------------

qreal EditorTabWidget::zoomFactor() const { return m_zoomFactor; }

void EditorTabWidget::setZoomFactor(qreal factor)
{
    m_zoomFactor = factor;
    for (int i = 0; i < count(); ++i) {
        auto ed = editor(i);
        if (ed) ed->setZoomFactor(factor);
    }
}

// ---------------------------------------------------------------------------
// Saved / unsaved icon
// ---------------------------------------------------------------------------

void EditorTabWidget::setSavedIcon(int index, bool saved)
{
    if (saved) {
        setTabIcon(index, QIcon::fromTheme("document-saved"));
    } else {
        setTabIcon(index, QIcon::fromTheme("document-unsaved"));
    }
}

// ---------------------------------------------------------------------------
// Former-tab tracking
// ---------------------------------------------------------------------------

int EditorTabWidget::formerTabIndex() const { return m_formerTabIndex; }

// ---------------------------------------------------------------------------
// Cleanup
// ---------------------------------------------------------------------------

void EditorTabWidget::deleteIfEmpty()
{
    if (count() == 0) {
        deleteLater();
    }
}

// ---------------------------------------------------------------------------
// Signal wiring
// ---------------------------------------------------------------------------

void EditorTabWidget::connectEditorSignals(NovaPad::Editor::CodeEditorWidget *ed)
{
    connect(ed, &NovaPad::Editor::CodeEditorWidget::cleanChanged,
            this, &EditorTabWidget::onCleanChanged);
    connect(ed, &NovaPad::Editor::CodeEditorWidget::mouseWheelUsed,
            this, &EditorTabWidget::onEditorMouseWheel);
    connect(ed, &NovaPad::Editor::CodeEditorWidget::fileNameChanged,
            this, &EditorTabWidget::onFileNameChanged);
}

void EditorTabWidget::disconnectEditorSignals(NovaPad::Editor::CodeEditorWidget *ed)
{
    disconnect(ed, nullptr, this, nullptr);
}

// ---------------------------------------------------------------------------
// Private slots
// ---------------------------------------------------------------------------

void EditorTabWidget::onCleanChanged(bool isClean)
{
    auto *ed = qobject_cast<NovaPad::Editor::CodeEditorWidget *>(sender());
    if (!ed) return;

    int idx = indexOf(ed);
    if (idx >= 0) {
        setSavedIcon(idx, isClean);
    }
}

void EditorTabWidget::onEditorMouseWheel(QWheelEvent *ev)
{
    auto *ed = qobject_cast<NovaPad::Editor::CodeEditorWidget *>(sender());
    if (!ed) return;

    int idx = indexOf(ed);
    if (idx >= 0) {
        emit editorMouseWheel(idx, ev);
    }
}

void EditorTabWidget::onFileNameChanged(const QUrl & /*oldName*/, const QUrl &newName)
{
    auto *ed = qobject_cast<NovaPad::Editor::CodeEditorWidget *>(sender());
    if (!ed) return;

    int idx = indexOf(ed);
    if (idx >= 0) {
        setTabText(idx, generateTabTitle(newName));
    }
}

void EditorTabWidget::onCurrentTabChanged(int index)
{
    m_formerTabIndex  = m_mostRecentIndex;
    m_mostRecentIndex = index;
}

// ---------------------------------------------------------------------------
// Event overrides
// ---------------------------------------------------------------------------

void EditorTabWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    // Middle-click on a tab → close it
    if (ev->button() == Qt::MiddleButton) {
        int idx = tabBar()->tabAt(ev->pos());
        if (idx >= 0) {
            emit tabCloseRequested(idx);
            return;
        }
    }

    // Clicking anywhere in the widget signals focus gain
    emit gotFocus();
    QTabWidget::mouseReleaseEvent(ev);
}

void EditorTabWidget::tabRemoved(int /*index*/)
{
    // If the last tab was removed, signal that this widget is empty
    if (count() == 0) {
        deleteIfEmpty();
    }
}

} // namespace NovaPad::UI
