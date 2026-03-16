/**
 * @file extension_api.h
 * @brief C++ API surface exposed to extensions via the extension server.
 *
 * Provides a structured interface that extensions can call to interact with
 * the NovaPad editor — opening files, manipulating text, reading settings,
 * registering commands, etc.
 */

#ifndef NOVAPAD_EXTENSIONS_EXTENSION_API_H
#define NOVAPAD_EXTENSIONS_EXTENSION_API_H

#include <QJsonObject>
#include <QObject>
#include <QString>

namespace NovaPad::Extensions {

/**
 * @class ExtensionApi
 * @brief Singleton that dispatches API calls from extensions to NovaPad internals.
 *
 * Extensions send JSON-RPC-style messages to the ExtensionServer, which
 * routes them here.  Each method validates parameters, performs the
 * requested action, and returns a JSON response.
 */
class ExtensionApi : public QObject
{
    Q_OBJECT

public:
    /** @brief Returns the singleton instance. */
    [[nodiscard]] static ExtensionApi &instance();

    ExtensionApi(const ExtensionApi &) = delete;
    ExtensionApi &operator=(const ExtensionApi &) = delete;

    /**
     * @brief Dispatches a JSON-RPC request to the appropriate handler.
     * @param request  JSON object with "method" and "params" fields.
     * @return JSON response object (with "result" or "error").
     */
    [[nodiscard]] QJsonObject dispatch(const QJsonObject &request);

private:
    ExtensionApi();

    // -- Handler methods -----------------------------------------------------
    QJsonObject handleOpenFile(const QJsonObject &params);
    QJsonObject handleGetActiveEditorContent(const QJsonObject &params);
    QJsonObject handleSetActiveEditorContent(const QJsonObject &params);
    QJsonObject handleGetCursorPosition(const QJsonObject &params);
    QJsonObject handleSetCursorPosition(const QJsonObject &params);
    QJsonObject handleInsertText(const QJsonObject &params);
    QJsonObject handleShowMessage(const QJsonObject &params);
    QJsonObject handleRegisterCommand(const QJsonObject &params);
    QJsonObject handleGetSetting(const QJsonObject &params);
    QJsonObject handleSetSetting(const QJsonObject &params);
};

} // namespace NovaPad::Extensions

#endif // NOVAPAD_EXTENSIONS_EXTENSION_API_H
