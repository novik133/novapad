/**
 * @file platform_utils.h
 * @brief Platform-specific utility functions.
 *
 * Encapsulates OS-specific operations like opening files in the default
 * file manager, detecting the default terminal emulator, and querying
 * system fonts.
 */

#ifndef NOVAPAD_UTILS_PLATFORM_UTILS_H
#define NOVAPAD_UTILS_PLATFORM_UTILS_H

#include <QString>

namespace NovaPad::Utils {

/**
 * @class PlatformUtils
 * @brief Static helpers for platform-specific operations.
 */
class PlatformUtils final
{
    Q_DISABLE_COPY_MOVE(PlatformUtils)
    PlatformUtils() = delete;

public:
    /** @brief Opens the OS file manager and selects the given file. */
    static void showInFileManager(const QString &filePath);

    /** @brief Opens a URL in the user's default web browser. */
    static void openUrl(const QString &url);

    /** @brief Returns the path to the default terminal emulator. */
    [[nodiscard]] static QString defaultTerminal();

    /** @brief Returns the best monospace font available on this platform. */
    [[nodiscard]] static QString defaultMonospaceFont();

    /** @brief Returns true if the desktop is using a dark colour scheme. */
    [[nodiscard]] static bool isDarkTheme();

    /** @brief Returns the OS name and version as a human-readable string. */
    [[nodiscard]] static QString osDescription();
};

} // namespace NovaPad::Utils

#endif // NOVAPAD_UTILS_PLATFORM_UTILS_H
