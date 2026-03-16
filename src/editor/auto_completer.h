/**
 * @file auto_completer.h
 * @brief Auto-completion engine for the code editor.
 *
 * Provides context-aware code completion using a combination of:
 *   - Word-based completion (tokens already present in the document)
 *   - Language-keyword completion (from LanguageManager)
 *   - Snippet expansion (user-defined and built-in)
 *
 * The completer attaches to a CodeEditorWidget and shows a popup list
 * as the user types, filtered in real time.
 */

#ifndef NOVAPAD_EDITOR_AUTO_COMPLETER_H
#define NOVAPAD_EDITOR_AUTO_COMPLETER_H

#include <QCompleter>
#include <QObject>
#include <QStringListModel>

namespace NovaPad::Editor {

class CodeEditorWidget; // forward declaration

/**
 * @class AutoCompleter
 * @brief Manages the completion popup and its data model.
 *
 * ## Activation rules
 * - The popup appears after typing 3+ characters of a word.
 * - Pressing Ctrl+Space forces the popup to appear immediately.
 * - Tab or Enter accepts the current selection; Esc dismisses it.
 */
class AutoCompleter : public QObject
{
    Q_OBJECT

public:
    explicit AutoCompleter(CodeEditorWidget *editor, QObject *parent = nullptr);
    ~AutoCompleter() override = default;

    /** @brief Forces the completion popup to show at the current cursor. */
    void triggerCompletion();

    /** @brief Dismisses the popup if visible. */
    void dismiss();

    /** @brief Sets the minimum prefix length required to show the popup. */
    void setMinPrefixLength(int length);

    /** @brief Enables or disables the auto-completer entirely. */
    void setEnabled(bool enabled);

    /** @brief Called by the editor on every key press to update the popup. */
    void onTextChanged();

private slots:
    /** @brief Inserts the chosen completion into the editor. */
    void insertCompletion(const QString &completion);

private:
    /** @brief Rebuilds the word list from the current document content. */
    void rebuildWordList();

    /** @brief Returns the text prefix under the cursor. */
    [[nodiscard]] QString currentPrefix() const;

    CodeEditorWidget   *m_editor          = nullptr;
    QCompleter         *m_completer       = nullptr;
    QStringListModel   *m_model           = nullptr;
    int                 m_minPrefixLength  = 3;
    bool                m_enabled          = true;
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_AUTO_COMPLETER_H
