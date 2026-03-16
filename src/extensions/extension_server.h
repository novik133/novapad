/**
 * @file extension_server.h
 * @brief Manages the extension runtime process (e.g. Node.js).
 *
 * The ExtensionServer launches a background process that hosts the
 * extension API.  Extensions communicate with NovaPad through a local
 * socket or HTTP endpoint managed by this server.
 */

#ifndef NOVAPAD_EXTENSIONS_EXTENSION_SERVER_H
#define NOVAPAD_EXTENSIONS_EXTENSION_SERVER_H

#include <QObject>
#include <QProcess>

namespace NovaPad::Extensions {

/**
 * @class ExtensionServer
 * @brief Singleton that manages the extension host process.
 */
class ExtensionServer : public QObject
{
    Q_OBJECT

public:
    /** @brief Returns the singleton instance. */
    [[nodiscard]] static ExtensionServer &instance();

    // Non-copyable, non-movable singleton
    ExtensionServer(const ExtensionServer &) = delete;
    ExtensionServer &operator=(const ExtensionServer &) = delete;

    /** @brief Starts the extension host process. */
    void start();

    /** @brief Stops the extension host process. */
    void stop();

    /** @brief Returns true if the host process is currently running. */
    [[nodiscard]] bool isRunning() const;

signals:
    void serverStarted();
    void serverStopped();
    void serverError(const QString &message);

private:
    ExtensionServer();
    ~ExtensionServer() override;

    QProcess *m_process = nullptr;
};

} // namespace NovaPad::Extensions

#endif // NOVAPAD_EXTENSIONS_EXTENSION_SERVER_H
