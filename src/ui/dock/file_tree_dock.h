/**
 * @file file_tree_dock.h
 * @brief File-system tree dock widget for browsing project directories.
 *
 * Displays a tree view of the filesystem rooted at a user-selected directory.
 * Double-clicking a file opens it in the editor.  Supports basic operations
 * like refresh, rename, and delete via context menu.
 */

#ifndef NOVAPAD_UI_DOCK_FILE_TREE_DOCK_H
#define NOVAPAD_UI_DOCK_FILE_TREE_DOCK_H

#include <QDockWidget>
#include <QFileSystemModel>
#include <QTreeView>

namespace NovaPad::UI::Dock {

/**
 * @class FileTreeDock
 * @brief Dock widget showing a navigable tree of files and directories.
 */
class FileTreeDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit FileTreeDock(QWidget *parent = nullptr);
    ~FileTreeDock() override = default;

    /** @brief Sets the root directory displayed in the tree. */
    void setRootPath(const QString &path);

    /** @brief Returns the current root path. */
    [[nodiscard]] QString rootPath() const;

signals:
    /** @brief Emitted when the user double-clicks a file. */
    void fileActivated(const QString &filePath);

private slots:
    void onDoubleClicked(const QModelIndex &index);
    void onCustomContextMenu(const QPoint &pos);

private:
    void setupUi();

    QFileSystemModel *m_model    = nullptr;
    QTreeView        *m_treeView = nullptr;
};

} // namespace NovaPad::UI::Dock

#endif // NOVAPAD_UI_DOCK_FILE_TREE_DOCK_H
