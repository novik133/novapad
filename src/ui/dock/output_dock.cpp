/**
 * @file output_dock.cpp
 * @brief Implementation of the output dock widget.
 */

#include "output_dock.h"

#include <QScrollBar>

#include <QVBoxLayout>

namespace NovaPad::UI::Dock {

OutputDock::OutputDock(QWidget *parent)
    : QDockWidget(tr("Output"), parent)
{
    setupUi();
}

void OutputDock::setupUi()
{
    auto *container = new QWidget(this);
    auto *layout    = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    m_outputView = new QPlainTextEdit(container);
    m_outputView->setReadOnly(true);
    m_outputView->setFont(QFont("Monospace", 10));
    m_outputView->setMaximumBlockCount(50000);
    layout->addWidget(m_outputView);

    setWidget(container);
}

void OutputDock::appendOutput(const QString &text)
{
    m_outputView->appendPlainText(text);
    // Auto-scroll to the latest line
    m_outputView->verticalScrollBar()->setValue(
        m_outputView->verticalScrollBar()->maximum());
}

void OutputDock::clearOutput()
{
    m_outputView->clear();
}

} // namespace NovaPad::UI::Dock
