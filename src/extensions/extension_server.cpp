/**
 * @file extension_server.cpp
 * @brief Implementation of the extension host process manager.
 */

#include "extension_server.h"
#include "core/globals.h"

#include <QCoreApplication>
#include <QDir>

namespace NovaPad::Extensions {

// ---------------------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------------------

ExtensionServer &ExtensionServer::instance()
{
    static ExtensionServer inst;
    return inst;
}

ExtensionServer::ExtensionServer()
    : QObject(nullptr)
{
}

ExtensionServer::~ExtensionServer()
{
    stop();
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ExtensionServer::start()
{
    if (m_process && m_process->state() != QProcess::NotRunning) {
        return; // Already running
    }

    const QString toolsPath = NovaPad::Core::Globals::extensionToolsPath();
    const QString serverScript = QDir(toolsPath).filePath("server.js");

    if (!QFileInfo::exists(serverScript)) {
        qWarning() << "[ExtensionServer] Server script not found:" << serverScript;
        emit serverError(tr("Extension server script not found."));
        return;
    }

    m_process = new QProcess(this);
    m_process->setWorkingDirectory(toolsPath);

    connect(m_process, &QProcess::started, this, [this]() {
        qInfo() << "[ExtensionServer] Extension host started.";
        emit serverStarted();
    });

    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus status) {
        qInfo() << "[ExtensionServer] Extension host exited with code"
                << exitCode << "status" << status;
        emit serverStopped();
    });

    connect(m_process, &QProcess::errorOccurred,
            this, [this](QProcess::ProcessError error) {
        qWarning() << "[ExtensionServer] Process error:" << error;
        emit serverError(
            tr("Extension server error: %1").arg(m_process->errorString()));
    });

    m_process->start("node", { serverScript });
}

void ExtensionServer::stop()
{
    if (!m_process) return;

    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        if (!m_process->waitForFinished(3000)) {
            m_process->kill();
            m_process->waitForFinished(1000);
        }
    }

    delete m_process;
    m_process = nullptr;
}

bool ExtensionServer::isRunning() const
{
    return m_process && m_process->state() == QProcess::Running;
}

} // namespace NovaPad::Extensions
