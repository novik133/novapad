/**
 * @file extension.cpp
 * @brief Implementation of the Extension class.
 *
 * Each extension lives in its own directory under the extensions root.
 * The directory must contain a manifest.json with at least:
 *   { "id": "...", "name": "...", "version": "...", "main": "..." }
 */

#include "extension.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>

namespace NovaPad::Extensions {

Extension::Extension(const QString &extensionDir, QObject *parent)
    : QObject(parent)
    , m_directory(extensionDir)
{
}

// ---------------------------------------------------------------------------
// Metadata accessors
// ---------------------------------------------------------------------------

QString Extension::id() const          { return m_id; }
QString Extension::name() const        { return m_name; }
QString Extension::version() const     { return m_version; }
QString Extension::author() const      { return m_author; }
QString Extension::description() const { return m_description; }
QString Extension::directory() const   { return m_directory; }
Extension::State Extension::state() const { return m_state; }

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

bool Extension::load()
{
    if (!parseManifest()) {
        m_state = State::Error;
        emit stateChanged(m_state);
        return false;
    }

    m_state = State::Loaded;
    emit stateChanged(m_state);
    return true;
}

bool Extension::start()
{
    if (m_state != State::Loaded) return false;

    // A full implementation would launch the extension's entry-point script
    // via the ExtensionServer / Node.js runtime.  For now, we mark it as
    // running so the lifecycle is correctly tracked.

    m_state = State::Running;
    emit stateChanged(m_state);
    return true;
}

void Extension::stop()
{
    if (m_state != State::Running) return;

    // Notify the extension runtime to tear down this extension's context
    m_state = State::Loaded;
    emit stateChanged(m_state);
}

// ---------------------------------------------------------------------------
// Manifest parsing
// ---------------------------------------------------------------------------

bool Extension::parseManifest()
{
    const QString manifestPath = QDir(m_directory).filePath("manifest.json");
    QFile file(manifestPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[Extension] Cannot read manifest:" << manifestPath;
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (doc.isNull()) {
        qWarning() << "[Extension] JSON parse error in" << manifestPath
                    << ":" << parseError.errorString();
        return false;
    }

    m_manifest = doc.object();

    // Required fields
    m_id   = m_manifest.value("id").toString();
    m_name = m_manifest.value("name").toString();
    if (m_id.isEmpty() || m_name.isEmpty()) {
        qWarning() << "[Extension] Manifest missing 'id' or 'name' in"
                    << manifestPath;
        return false;
    }

    // Optional fields
    m_version     = m_manifest.value("version").toString("0.0.0");
    m_author      = m_manifest.value("author").toString();
    m_description = m_manifest.value("description").toString();

    return true;
}

} // namespace NovaPad::Extensions
