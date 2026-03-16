/**
 * @file single_instance.h
 * @brief Ensures only one NovaPad process runs per user session.
 *
 * Uses QLocalServer/QLocalSocket (or D-Bus on supported desktops) to detect
 * an already-running instance and forward command-line arguments to it.
 * This mirrors the SingleApplication pattern from the original Notepadqq.
 */

#ifndef NOVAPAD_CORE_SINGLE_INSTANCE_H
#define NOVAPAD_CORE_SINGLE_INSTANCE_H

#include <QApplication>
#include <QLocalServer>

namespace NovaPad::Core {

/**
 * @class SingleInstance
 * @brief QApplication subclass that enforces a single running instance.
 *
 * On startup the application checks whether another NovaPad is already
 * running.  If so, it forwards the current CLI arguments through a local
 * socket and exits.  The running instance receives those arguments via
 * the receivedArguments() signal and opens the requested files.
 */
class SingleInstance : public QApplication
{
    Q_OBJECT

public:
    explicit SingleInstance(int &argc, char **argv);
    ~SingleInstance() override;

    /**
     * @brief Starts the local server so that future instances can connect.
     *
     * Must be called after confirming that no other instance is running.
     */
    void startServer();

    /**
     * @brief Attempts to connect to an existing NovaPad instance.
     *
     * If successful, forwards the current command-line arguments to the
     * running instance and returns true (the caller should then exit).
     *
     * @return true if arguments were sent to another instance.
     */
    [[nodiscard]] bool attachToOtherInstance();

signals:
    /**
     * @brief Emitted when another NovaPad instance sends its arguments.
     * @param workingDirectory  The working directory of the sending instance.
     * @param arguments         The full argument list.
     */
    void receivedArguments(const QString &workingDirectory,
                           const QStringList &arguments);

private:
    /** @brief Derives a socket name unique to the current user. */
    [[nodiscard]] QString socketNameForUser() const;

    /** @brief Serialises and sends CLI args through an open socket. */
    bool sendCommandLineArguments(class QLocalSocket *socket);

    /** @brief Tries to find an already-running instance's socket. */
    [[nodiscard]] QLocalSocket *findRunningInstance();

    /** @brief Slot called when a new connection arrives on the server. */
    void onNewConnection();

    QLocalServer *m_server = nullptr;   ///< Listening server (owned)
};

} // namespace NovaPad::Core

#endif // NOVAPAD_CORE_SINGLE_INSTANCE_H
