/**
 * @file extension_loader.cpp
 * @brief Implementation of the extension discovery and loading service.
 */

#include "extension_loader.h"
#include "extensions/extension_server.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QProcess>

namespace NovaPad::Extensions {

QList<Extension *> ExtensionLoader::s_extensions;

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

bool ExtensionLoader::isRuntimePresent()
{
    // Check if Node.js is available on the system PATH
    QProcess proc;
    proc.start("node", { "--version" });
    proc.waitForFinished(3000);
    return proc.exitCode() == 0;
}

void ExtensionLoader::startServer()
{
    ExtensionServer::instance().start();
}

void ExtensionLoader::loadAll(const QString &extensionsDir)
{
    QDir dir(extensionsDir);
    if (!dir.exists()) {
        qWarning() << "[ExtensionLoader] Extensions directory does not exist:"
                    << extensionsDir;
        return;
    }

    // Each subdirectory is a potential extension
    QDirIterator it(extensionsDir, QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        const QString extDir = it.next();
        const QString manifest = QDir(extDir).filePath("manifest.json");

        if (!QFileInfo::exists(manifest)) {
            continue;
        }

        auto *ext = new Extension(extDir);
        if (ext->load()) {
            s_extensions.append(ext);
            qInfo() << "[ExtensionLoader] Loaded extension:"
                    << ext->name() << ext->version();

            // Auto-start the extension if the server is running
            if (ExtensionServer::instance().isRunning()) {
                ext->start();
            }
        } else {
            qWarning() << "[ExtensionLoader] Failed to load extension from:"
                        << extDir;
            delete ext;
        }
    }
}

void ExtensionLoader::unloadAll()
{
    for (auto *ext : s_extensions) {
        ext->stop();
        delete ext;
    }
    s_extensions.clear();

    ExtensionServer::instance().stop();
}

const QList<Extension *> &ExtensionLoader::extensions()
{
    return s_extensions;
}

Extension *ExtensionLoader::findById(const QString &id)
{
    for (auto *ext : s_extensions) {
        if (ext->id() == id) return ext;
    }
    return nullptr;
}

} // namespace NovaPad::Extensions
