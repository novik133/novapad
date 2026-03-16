/**
 * @file globals.h
 * @brief Global constants, paths, and utility functions for NovaPad.
 *
 * Centralises all environment queries (data directories, editor paths,
 * extension paths) so that platform-specific logic lives in one place.
 */

#ifndef NOVAPAD_CORE_GLOBALS_H
#define NOVAPAD_CORE_GLOBALS_H

#include <QObject>
#include <QString>
#include <QUrl>

namespace NovaPad::Core {

/**
 * @class Globals
 * @brief Static helper providing application-wide paths and diagnostic output.
 *
 * No instance is ever created — every member is static.  The class is sealed
 * (marked `final`) to prevent accidental subclassing.
 */
class Globals final
{
    Q_DISABLE_COPY_MOVE(Globals)
    Globals() = delete;

public:
    // -- Application metadata ------------------------------------------------

    /** @brief Human-readable version string, e.g. "1.0.0". */
    static constexpr auto kVersion = "1.0.0";

    /** @brief URL where contributors are listed. */
    static constexpr auto kContributorsUrl =
        "https://github.com/novik133/novapad/graphs/contributors";

    /** @brief Project website. */
    static constexpr auto kWebsite = "https://noviktech.com";
    
    /** @brief GitHub repository. */
    static constexpr auto kGitHubUrl = "https://github.com/novik133/novapad";

    // -- Path helpers --------------------------------------------------------

    /**
     * @brief Returns the writable application-data directory.
     * @param fileName  Optional file name to append.
     * @return Full path inside the app-data folder.
     */
    [[nodiscard]] static QString appDataPath(const QString &fileName = {});

    /**
     * @brief Returns the path to the embedded CodeMirror / editor HTML.
     */
    [[nodiscard]] static QString editorPath();

    /**
     * @brief Returns the directory where user-installed extensions reside.
     */
    [[nodiscard]] static QString extensionsPath();

    /**
     * @brief Returns the path to the bundled extension-tool scripts.
     */
    [[nodiscard]] static QString extensionToolsPath();

    /**
     * @brief Extracts a human-readable file name from a QUrl.
     * @param url  The URL to parse.
     * @return The local file name, or the full URL string if non-local.
     */
    [[nodiscard]] static QString fileNameFromUrl(const QUrl &url);

    /**
     * @brief Returns the copyright string including the current year.
     */
    [[nodiscard]] static QString copyrightNotice();

    // -- Diagnostics ---------------------------------------------------------

    /**
     * @brief Prints environment details to stderr for debugging.
     *
     * Includes Qt version, OS, locale, relevant paths, etc.
     */
    static void printEnvironmentInfo();
};

} // namespace NovaPad::Core

// Convenience macros mirroring original Notepadqq helpers
#define printerrln(msg) qWarning() << msg

#endif // NOVAPAD_CORE_GLOBALS_H
