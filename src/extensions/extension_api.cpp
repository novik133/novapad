/**
 * @file extension_api.cpp
 * @brief Implementation of the extension API dispatcher.
 *
 * Each handler method validates its parameters, performs the requested
 * action on the appropriate NovaPad subsystem, and returns a JSON response.
 * Unknown methods return a standard JSON-RPC "method not found" error.
 */

#include "extension_api.h"
#include "ui/main_window.h"

#include <QJsonArray>
#include <QMessageBox>

namespace NovaPad::Extensions {

// ---------------------------------------------------------------------------
// Singleton
// ---------------------------------------------------------------------------

ExtensionApi &ExtensionApi::instance()
{
    static ExtensionApi inst;
    return inst;
}

ExtensionApi::ExtensionApi()
    : QObject(nullptr)
{
}

// ---------------------------------------------------------------------------
// Dispatcher
// ---------------------------------------------------------------------------

QJsonObject ExtensionApi::dispatch(const QJsonObject &request)
{
    const QString method = request.value("method").toString();
    const QJsonObject params = request.value("params").toObject();

    if (method == "openFile")                  return handleOpenFile(params);
    if (method == "getActiveEditorContent")    return handleGetActiveEditorContent(params);
    if (method == "setActiveEditorContent")    return handleSetActiveEditorContent(params);
    if (method == "getCursorPosition")         return handleGetCursorPosition(params);
    if (method == "setCursorPosition")         return handleSetCursorPosition(params);
    if (method == "insertText")                return handleInsertText(params);
    if (method == "showMessage")               return handleShowMessage(params);
    if (method == "registerCommand")           return handleRegisterCommand(params);
    if (method == "getSetting")                return handleGetSetting(params);
    if (method == "setSetting")                return handleSetSetting(params);

    return { { "error", QStringLiteral("Method not found: %1").arg(method) } };
}

// ---------------------------------------------------------------------------
// Handlers
// ---------------------------------------------------------------------------

QJsonObject ExtensionApi::handleOpenFile(const QJsonObject &params)
{
    const QString filePath = params.value("filePath").toString();
    if (filePath.isEmpty()) {
        return { { "error", "Missing 'filePath' parameter" } };
    }

    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (win) {
        win->openCommandLineUrls(QString(), { filePath });
        return { { "result", true } };
    }
    return { { "error", "No active window" } };
}

QJsonObject ExtensionApi::handleGetActiveEditorContent(const QJsonObject & /*params*/)
{
    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (!win) return { { "error", "No active window" } };

    auto editor = win->currentEditor();
    if (!editor) return { { "error", "No active editor" } };

    return { { "result", editor->content() } };
}

QJsonObject ExtensionApi::handleSetActiveEditorContent(const QJsonObject &params)
{
    const QString content = params.value("content").toString();

    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (!win) return { { "error", "No active window" } };

    auto editor = win->currentEditor();
    if (!editor) return { { "error", "No active editor" } };

    editor->setContent(content);
    return { { "result", true } };
}

QJsonObject ExtensionApi::handleGetCursorPosition(const QJsonObject & /*params*/)
{
    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (!win) return { { "error", "No active window" } };

    auto editor = win->currentEditor();
    if (!editor) return { { "error", "No active editor" } };

    auto pos = editor->cursorPos();
    QJsonObject result;
    result["line"]   = pos.line;
    result["column"] = pos.column;
    return { { "result", result } };
}

QJsonObject ExtensionApi::handleSetCursorPosition(const QJsonObject &params)
{
    const int line   = params.value("line").toInt();
    const int column = params.value("column").toInt();

    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (!win) return { { "error", "No active window" } };

    auto editor = win->currentEditor();
    if (!editor) return { { "error", "No active editor" } };

    editor->setCursorPos(line, column);
    return { { "result", true } };
}

QJsonObject ExtensionApi::handleInsertText(const QJsonObject &params)
{
    const QString text = params.value("text").toString();

    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (!win) return { { "error", "No active window" } };

    auto editor = win->currentEditor();
    if (!editor) return { { "error", "No active editor" } };

    QTextCursor tc = editor->textCursor();
    tc.insertText(text);
    editor->setTextCursor(tc);
    return { { "result", true } };
}

QJsonObject ExtensionApi::handleShowMessage(const QJsonObject &params)
{
    const QString title   = params.value("title").toString("NovaPad Extension");
    const QString message = params.value("message").toString();

    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    QMessageBox::information(win, title, message);
    return { { "result", true } };
}

QJsonObject ExtensionApi::handleRegisterCommand(const QJsonObject &params)
{
    const QString extensionId = params.value("extensionId").toString();
    const QString label       = params.value("label").toString();

    if (extensionId.isEmpty() || label.isEmpty()) {
        return { { "error", "Missing 'extensionId' or 'label'" } };
    }

    auto *win = NovaPad::UI::MainWindow::lastActiveInstance();
    if (win) {
        win->addExtensionMenuItem(extensionId, label);
        return { { "result", true } };
    }
    return { { "error", "No active window" } };
}

QJsonObject ExtensionApi::handleGetSetting(const QJsonObject &params)
{
    Q_UNUSED(params);
    // TODO: read from NovaPadSettings
    return { { "result", QJsonObject() } };
}

QJsonObject ExtensionApi::handleSetSetting(const QJsonObject &params)
{
    Q_UNUSED(params);
    // TODO: write to NovaPadSettings
    return { { "result", true } };
}

} // namespace NovaPad::Extensions
