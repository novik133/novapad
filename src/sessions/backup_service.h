/**
 * @file backup_service.h
 * @brief Automatic backup and crash-recovery service.
 *
 * Periodically saves the state of all open editors to a crash-recovery
 * directory.  On next launch, if an improper shutdown is detected (the
 * backup sentinel file still exists), the service offers to restore the
 * previous session.  Mirrors Notepadqq's BackupService.
 */

#ifndef NOVAPAD_SESSIONS_BACKUP_SERVICE_H
#define NOVAPAD_SESSIONS_BACKUP_SERVICE_H

#include <QString>

namespace NovaPad::Sessions {

/**
 * @class BackupService
 * @brief Static service managing periodic autosave and crash recovery.
 */
class BackupService final
{
    Q_DISABLE_COPY_MOVE(BackupService)
    BackupService() = delete;

public:
    /**
     * @brief Starts the autosave timer with the given interval.
     * @param intervalSec  Seconds between autosave cycles (0 to disable).
     */
    static void enableAutosave(int intervalSec);

    /** @brief Stops the autosave timer. */
    static void disableAutosave();

    /**
     * @brief Checks whether the previous NovaPad session was shut down
     *        improperly (i.e. the sentinel file still exists).
     */
    [[nodiscard]] static bool detectImproperShutdown();

    /**
     * @brief Attempts to restore the last autosaved session.
     * @return true if a window was successfully restored.
     */
    static bool restoreFromBackup();

    /**
     * @brief Performs a single autosave pass — called by the timer.
     */
    static void performAutosave();

    /**
     * @brief Clears backup data and removes the sentinel file.
     *
     * Should be called during a normal shutdown sequence.
     */
    static void clearBackupData();

    /**
     * @brief Returns the directory where backup files are stored.
     */
    [[nodiscard]] static QString backupDirectory();

private:
    /** @brief Creates the sentinel file that marks an active session. */
    static void createSentinelFile();

    /** @brief Removes the sentinel file (normal shutdown). */
    static void removeSentinelFile();

    /** @brief Returns the full path to the sentinel file. */
    [[nodiscard]] static QString sentinelFilePath();

    /** @brief Returns the full path to the autosave session file. */
    [[nodiscard]] static QString autosaveSessionPath();
};

} // namespace NovaPad::Sessions

#endif // NOVAPAD_SESSIONS_BACKUP_SERVICE_H
