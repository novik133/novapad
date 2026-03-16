/**
 * @file terminal_dock.cpp
 * @brief Implementation of the integrated terminal dock widget.
 */

#include "terminal_dock.h"

#include <QDir>
#include <QVBoxLayout>

namespace NovaPad::UI::Dock {

TerminalDock::TerminalDock(QWidget *parent)
    : QDockWidget(tr("Terminal"), parent)
    , m_workingDir(QDir::currentPath())
{
    setupUi();
}

TerminalDock::~TerminalDock()
{
    if (m_process && m_process->state() != QProcess::NotRunning) {
        m_process->kill();
        m_process->waitForFinished(1000);
    }
}

void TerminalDock::setupUi()
{
    auto *container = new QWidget(this);
    auto *layout    = new QVBoxLayout(container);
    layout->setContentsMargins(2, 2, 2, 2);

    m_outputView = new QPlainTextEdit(container);
    m_outputView->setReadOnly(true);
    m_outputView->setFont(QFont("Monospace", 10));
    m_outputView->setMaximumBlockCount(10000);
    layout->addWidget(m_outputView);

    m_inputLine = new QLineEdit(container);
    m_inputLine->setPlaceholderText(tr("Enter command..."));
    m_inputLine->setFont(QFont("Monospace", 10));
    connect(m_inputLine, &QLineEdit::returnPressed,
            this, &TerminalDock::onCommandEntered);
    layout->addWidget(m_inputLine);

    setWidget(container);

    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &TerminalDock::onProcessOutput);
    connect(m_process, &QProcess::readyReadStandardError,
            this, &TerminalDock::onProcessOutput);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &TerminalDock::onProcessFinished);
}

void TerminalDock::setWorkingDirectory(const QString &dir)
{
    m_workingDir = dir;
}

void TerminalDock::onCommandEntered()
{
    const QString cmd = m_inputLine->text().trimmed();
    if (cmd.isEmpty()) return;

    m_inputLine->clear();
    m_outputView->appendPlainText(QStringLiteral("$ %1").arg(cmd));

    m_process->setWorkingDirectory(m_workingDir);

#ifdef Q_OS_WIN
    m_process->start("cmd.exe", { "/C", cmd });
#else
    m_process->start("/bin/sh", { "-c", cmd });
#endif
}

void TerminalDock::onProcessOutput()
{
    const QByteArray stdOut = m_process->readAllStandardOutput();
    const QByteArray stdErr = m_process->readAllStandardError();

    if (!stdOut.isEmpty()) {
        m_outputView->appendPlainText(QString::fromUtf8(stdOut).trimmed());
    }
    if (!stdErr.isEmpty()) {
        m_outputView->appendPlainText(QString::fromUtf8(stdErr).trimmed());
    }
}

void TerminalDock::onProcessFinished(int exitCode, QProcess::ExitStatus status)
{
    if (status == QProcess::CrashExit) {
        m_outputView->appendPlainText(tr("[Process crashed]"));
    } else if (exitCode != 0) {
        m_outputView->appendPlainText(
            tr("[Process exited with code %1]").arg(exitCode));
    }
}

} // namespace NovaPad::UI::Dock
