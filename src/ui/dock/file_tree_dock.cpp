/**
 * @file file_tree_dock.cpp
 * @brief Implementation of the file-system tree dock widget.
 */

#include "file_tree_dock.h"

#include <QDir>
#include <QHeaderView>
#include <QMenu>
#include <QVBoxLayout>

namespace NovaPad::UI::Dock {

FileTreeDock::FileTreeDock(QWidget *parent)
    : QDockWidget(tr("File Browser"), parent)
{
    setupUi();
}

void FileTreeDock::setupUi()
{
    auto *container = new QWidget(this);
    auto *layout    = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    m_model = new QFileSystemModel(this);
    m_model->setRootPath(QDir::homePath());
    m_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

    m_treeView = new QTreeView(container);
    m_treeView->setModel(m_model);
    m_treeView->setRootIndex(m_model->index(QDir::homePath()));
    m_treeView->setAnimated(true);
    m_treeView->setSortingEnabled(true);
    m_treeView->sortByColumn(0, Qt::AscendingOrder);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Hide columns other than Name by default
    m_treeView->header()->hideSection(1); // Size
    m_treeView->header()->hideSection(2); // Type
    m_treeView->header()->hideSection(3); // Modified

    connect(m_treeView, &QTreeView::doubleClicked,
            this, &FileTreeDock::onDoubleClicked);
    connect(m_treeView, &QWidget::customContextMenuRequested,
            this, &FileTreeDock::onCustomContextMenu);

    layout->addWidget(m_treeView);
    setWidget(container);
}

void FileTreeDock::setRootPath(const QString &path)
{
    m_model->setRootPath(path);
    m_treeView->setRootIndex(m_model->index(path));
}

QString FileTreeDock::rootPath() const
{
    return m_model->rootPath();
}

void FileTreeDock::onDoubleClicked(const QModelIndex &index)
{
    if (!m_model->isDir(index)) {
        emit fileActivated(m_model->filePath(index));
    }
}

void FileTreeDock::onCustomContextMenu(const QPoint &pos)
{
    QModelIndex index = m_treeView->indexAt(pos);
    if (!index.isValid()) return;

    QMenu menu(this);
    menu.addAction(tr("Open"), this, [this, index]() {
        if (!m_model->isDir(index)) {
            emit fileActivated(m_model->filePath(index));
        }
    });
    menu.addAction(tr("Refresh"), this, [this]() {
        m_model->setRootPath(m_model->rootPath());
    });
    menu.exec(m_treeView->viewport()->mapToGlobal(pos));
}

} // namespace NovaPad::UI::Dock
