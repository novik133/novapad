/**
 * @file single_instance.cpp
 * @brief Implementation of the single-instance enforcement mechanism.
 *
 * Communication protocol (via QLocalSocket):
 *   1. The new instance connects and writes a JSON array:
 *      ["<working_directory>", "<arg1>", "<arg2>", ...]
 *   2. The running instance reads the array, emits receivedArguments(),
 *      and closes the connection.
 *
 * The socket name is derived from the application name plus the current
 * user's UID so that separate user sessions do not interfere.
 */

#include "single_instance.h"

#include <QDataStream>
#include <QDir>
#include <QLocalSocket>
#include <QJsonArray>
#include <QJsonDocument>

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

namespace NovaPad::Core {

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

SingleInstance::SingleInstance(int &argc, char **argv)
    : QApplication(argc, argv)
{
}

SingleInstance::~SingleInstance()
{
    if (m_server) {
        m_server->close();
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void SingleInstance::startServer()
{
    m_server = new QLocalServer(this);

    // Remove any stale socket file left by a crashed process
    QLocalServer::removeServer(socketNameForUser());

    if (!m_server->listen(socketNameForUser())) {
        qWarning() << "[SingleInstance] Failed to start local server:"
                    << m_server->errorString();
        return;
    }

    connect(m_server, &QLocalServer::newConnection,
            this, &SingleInstance::onNewConnection);
}

bool SingleInstance::attachToOtherInstance()
{
    QLocalSocket *socket = findRunningInstance();
    if (!socket) {
        return false;
    }

    const bool sent = sendCommandLineArguments(socket);
    socket->disconnectFromServer();
    delete socket;
    return sent;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

QString SingleInstance::socketNameForUser() const
{
    QString name = QCoreApplication::applicationName() + QStringLiteral("-socket");

#ifdef Q_OS_UNIX
    // Append UID so different users' instances don't collide
    name += QStringLiteral("-") + QString::number(getuid());
#endif

    return name;
}

QLocalSocket *SingleInstance::findRunningInstance()
{
    auto *socket = new QLocalSocket();
    socket->connectToServer(socketNameForUser());

    // Wait briefly for connection — if it succeeds, another instance is alive
    if (socket->waitForConnected(500)) {
        return socket;
    }

    delete socket;
    return nullptr;
}

bool SingleInstance::sendCommandLineArguments(QLocalSocket *socket)
{
    // Encode working directory + arguments as a JSON array
    QJsonArray payload;
    payload.append(QDir::currentPath());
    for (const QString &arg : QCoreApplication::arguments()) {
        payload.append(arg);
    }

    const QByteArray data = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    // Write a length-prefixed message
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_6_0);
    stream << data;

    return socket->waitForBytesWritten(3000);
}

void SingleInstance::onNewConnection()
{
    QLocalSocket *socket = m_server->nextPendingConnection();
    if (!socket) {
        return;
    }

    // Wait for the payload to arrive
    if (!socket->waitForReadyRead(3000)) {
        delete socket;
        return;
    }

    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_6_0);
    QByteArray data;
    stream >> data;

    const QJsonArray payload = QJsonDocument::fromJson(data).array();
    if (payload.size() >= 1) {
        const QString workingDir = payload.at(0).toString();
        QStringList args;
        for (int i = 1; i < payload.size(); ++i) {
            args.append(payload.at(i).toString());
        }
        emit receivedArguments(workingDir, args);
    }

    socket->disconnectFromServer();
    delete socket;
}

} // namespace NovaPad::Core
