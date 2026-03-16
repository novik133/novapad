/**
 * @file backup_service.cpp
 * @brief Implementation of the autosave and crash-recovery service.
 *
 * The service uses a sentinel file to detect improper shutdowns.  The file
 * is created when autosave is enabled and removed during normal exit.  If
 * the file is still present on the next launch, the previous session is
 * assumed to have crashed and can be restored from the autosave XML.
 */

#include "backup_service.h"
#include "sessions/session_manager.h"
#include "core/globals.h"
#include "ui/main_window.h"

#include <QDir>
#include <QFile>
#include <QTimer>

namespace NovaPad::Sessions {

// Static timer — lazily created on first enableAutosave() call
static QTimer *s_autosaveTimer = nullptr;

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void BackupService::enableAutosave(int intervalSec)
{
    if (intervalSec <= 0) {
        disableAutosave();
        return;
    }

    createSentinelFile();

    if (!s_autosaveTimer) {
        s_autosaveTimer = new QTimer();
        s_autosaveTimer->setTimerType(Qt::VeryCoarseTimer);
        QObject::connect(s_autosaveTimer, &QTimer::timeout,
                         &BackupService::performAutosave);
    }

    s_autosaveTimer->start(intervalSec * 1000);
}

void BackupService::disableAutosave()
{
    if (s_autosaveTimer) {
        s_autosaveTimer->stop();
    }
}

bool BackupService::detectImproperShutdown()
{
    return QFile::exists(sentinelFilePath());
}

bool BackupService::restoreFromBackup()
{
    const QString sessionPath = autosaveSessionPath();
    if (!QFile::exists(sessionPath)) {
        return false;
    }

    // Create a new main window and restore the session into it
    auto *win = new NovaPad::UI::MainWindow(QStringList(), nullptr);
    bool ok = SessionManager::loadSession(
        win->docEngine(), win->topEditorContainer(), sessionPath);

    if (ok) {
        win->show();
    } else {
        delete win;
    }

    return ok;
}

void BackupService::performAutosave()
{
    // Iterate all open main windows and save their state
    const auto windows = NovaPad::UI::MainWindow::instances();
    if (windows.isEmpty()) return;

    // For simplicity, save the first window's session (a production version
    // would iterate all windows and merge them into one session file)
    auto *win = windows.first();
    if (win) {
        SessionManager::saveSession(
            win->docEngine(), win->topEditorContainer(), autosaveSessionPath());
    }
}

void BackupService::clearBackupData()
{
    disableAutosave();
    removeSentinelFile();

    // Remove the autosave session file
    QFile::remove(autosaveSessionPath());
}

QString BackupService::backupDirectory()
{
    const QString dir = NovaPad::Core::Globals::appDataPath("backup");
    QDir().mkpath(dir);
    return dir;
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void BackupService::createSentinelFile()
{
    QFile sentinel(sentinelFilePath());
    sentinel.open(QIODevice::WriteOnly);
    sentinel.write("active");
    sentinel.close();
}

void BackupService::removeSentinelFile()
{
    QFile::remove(sentinelFilePath());
}

QString BackupService::sentinelFilePath()
{
    return backupDirectory() + "/session.lock";
}

QString BackupService::autosaveSessionPath()
{
    return backupDirectory() + "/autosave.nps";
}

} // namespace NovaPad::Sessions
