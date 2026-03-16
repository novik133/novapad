/**
 * @file extension.h
 * @brief Represents a single loaded NovaPad extension/plugin.
 *
 * Each extension is a directory containing a manifest.json and one or more
 * script files.  The Extension class encapsulates the metadata and provides
 * lifecycle management (load, start, stop, unload).
 */

#ifndef NOVAPAD_EXTENSIONS_EXTENSION_H
#define NOVAPAD_EXTENSIONS_EXTENSION_H

#include <QJsonObject>
#include <QObject>
#include <QString>

namespace NovaPad::Extensions {

/**
 * @class Extension
 * @brief Metadata and lifecycle wrapper for a single extension.
 */
class Extension : public QObject
{
    Q_OBJECT

public:
    /** @brief Extension lifecycle states. */
    enum class State {
        Unloaded,   ///< Manifest parsed but not activated
        Loaded,     ///< Dependencies resolved, ready to start
        Running,    ///< Actively executing
        Error       ///< Failed to load or start
    };

    explicit Extension(const QString &extensionDir, QObject *parent = nullptr);
    ~Extension() override = default;

    // -- Metadata accessors --------------------------------------------------

    [[nodiscard]] QString id() const;
    [[nodiscard]] QString name() const;
    [[nodiscard]] QString version() const;
    [[nodiscard]] QString author() const;
    [[nodiscard]] QString description() const;
    [[nodiscard]] QString directory() const;
    [[nodiscard]] State   state() const;

    // -- Lifecycle -----------------------------------------------------------

    /** @brief Parses manifest.json and validates required fields. */
    bool load();

    /** @brief Activates the extension (runs its entry point). */
    bool start();

    /** @brief Deactivates the extension gracefully. */
    void stop();

signals:
    void stateChanged(State newState);

private:
    /** @brief Reads and parses the manifest.json file. */
    bool parseManifest();

    QString     m_directory;
    QJsonObject m_manifest;
    State       m_state = State::Unloaded;

    // Cached manifest fields
    QString m_id;
    QString m_name;
    QString m_version;
    QString m_author;
    QString m_description;
};

} // namespace NovaPad::Extensions

#endif // NOVAPAD_EXTENSIONS_EXTENSION_H
