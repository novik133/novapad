/**
 * @file session_manager.h
 * @brief Saves and restores editor sessions to/from XML files.
 *
 * A session captures the full state of all open windows, tab widgets,
 * and editors — including file paths, cursor positions, scroll offsets,
 * and unsaved content hashes.  This mirrors Notepadqq's Sessions class.
 */

#ifndef NOVAPAD_SESSIONS_SESSION_MANAGER_H
#define NOVAPAD_SESSIONS_SESSION_MANAGER_H

#include <QString>

namespace NovaPad::Utils { class DocEngine; }
namespace NovaPad::UI    { class TopEditorContainer; }

namespace NovaPad::Sessions {

/**
 * @class SessionManager
 * @brief Static helpers for serialising / deserialising editor sessions.
 */
class SessionManager final
{
    Q_DISABLE_COPY_MOVE(SessionManager)
    SessionManager() = delete;

public:
    /**
     * @brief Saves the current state of all editors to an XML file.
     * @param docEngine   Document engine (provides encoding/BOM info).
     * @param container   Top-level editor container to iterate.
     * @param filePath    Destination XML file path.
     * @return true on success.
     */
    static bool saveSession(NovaPad::Utils::DocEngine *docEngine,
                            NovaPad::UI::TopEditorContainer *container,
                            const QString &filePath);

    /**
     * @brief Restores editors from a previously saved session file.
     * @param docEngine   Document engine used to reload files.
     * @param container   Container into which tabs are created.
     * @param filePath    Source XML session file path.
     * @return true on success.
     */
    static bool loadSession(NovaPad::Utils::DocEngine *docEngine,
                            NovaPad::UI::TopEditorContainer *container,
                            const QString &filePath);
};

} // namespace NovaPad::Sessions

#endif // NOVAPAD_SESSIONS_SESSION_MANAGER_H
