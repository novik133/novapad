/**
 * @file extension_loader.h
 * @brief Discovers and loads extensions from the extensions directory.
 *
 * Scans the extensions directory for subdirectories containing a valid
 * manifest.json, instantiates Extension objects for each, and starts
 * them via the ExtensionServer runtime.  Mirrors Notepadqq's ExtensionsLoader.
 */

#ifndef NOVAPAD_EXTENSIONS_EXTENSION_LOADER_H
#define NOVAPAD_EXTENSIONS_EXTENSION_LOADER_H

#include "extensions/extension.h"

#include <QList>
#include <QString>

namespace NovaPad::Extensions {

/**
 * @class ExtensionLoader
 * @brief Static service that discovers, loads, and manages all extensions.
 */
class ExtensionLoader final
{
    Q_DISABLE_COPY_MOVE(ExtensionLoader)
    ExtensionLoader() = delete;

public:
    /**
     * @brief Checks whether the extension runtime (e.g. Node.js) is available.
     * @return true if the runtime binary was found.
     */
    [[nodiscard]] static bool isRuntimePresent();

    /**
     * @brief Starts the extension server process.
     *
     * Must be called before loadAll() so that extensions have a runtime
     * to connect to.
     */
    static void startServer();

    /**
     * @brief Discovers and loads all extensions in the given directory.
     * @param extensionsDir  Root directory containing extension subdirectories.
     */
    static void loadAll(const QString &extensionsDir);

    /**
     * @brief Stops and unloads all currently running extensions.
     */
    static void unloadAll();

    /**
     * @brief Returns the list of all loaded extensions.
     */
    [[nodiscard]] static const QList<Extension *> &extensions();

    /**
     * @brief Finds a loaded extension by its unique id.
     * @return Pointer to the Extension, or nullptr if not found.
     */
    [[nodiscard]] static Extension *findById(const QString &id);

private:
    static QList<Extension *> s_extensions;
};

} // namespace NovaPad::Extensions

#endif // NOVAPAD_EXTENSIONS_EXTENSION_LOADER_H
