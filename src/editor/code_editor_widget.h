/**
 * @file code_editor_widget.h
 * @brief Core code editor widget with syntax highlighting, line numbers, and code folding.
 *
 * This is the heart of NovaPad — a QPlainTextEdit subclass enriched with
 * every feature a professional programmer expects: bracket matching,
 * auto-indentation, multi-cursor support stubs, minimap integration,
 * zoom, font override, and encoding tracking.
 *
 * Unlike the original Notepadqq (which embedded CodeMirror in a QWebView),
 * NovaPad uses a native Qt widget for maximum performance and C++23 integration.
 */

#ifndef NOVAPAD_EDITOR_CODE_EDITOR_WIDGET_H
#define NOVAPAD_EDITOR_CODE_EDITOR_WIDGET_H

#include "editor/bracket_matcher.h"
#include "editor/language_manager.h"
#include "editor/syntax_highlighter.h"

#include <QPlainTextEdit>
#include <QPrinter>
#include <QTextCodec>
#include <QUrl>
#include <QWidget>

#include <memory>
#include <optional>

class EditorTabWidget; // forward-declared for friend access

namespace NovaPad::Editor {

class LineNumberArea;   // defined in line_number_area.h
class MinimapWidget;    // defined in minimap_widget.h

/**
 * @struct CursorPosition
 * @brief Zero-based line/column pair used across the editor subsystem.
 */
struct CursorPosition
{
    int line   = 0;
    int column = 0;

    [[nodiscard]] auto operator<=>(const CursorPosition &) const = default;
};

/**
 * @struct TextSelection
 * @brief Describes a contiguous selection range within the document.
 */
struct TextSelection
{
    CursorPosition from;
    CursorPosition to;
};

/**
 * @struct IndentationMode
 * @brief Captures the current indentation style.
 */
struct IndentationMode
{
    bool useTabs = false;   ///< true → hard tabs; false → spaces
    int  size    = 4;       ///< Number of columns per indent level
};

/**
 * @class CodeEditorWidget
 * @brief A feature-rich plain-text code editor widget.
 *
 * ## Key capabilities
 * - Native syntax highlighting via SyntaxHighlighter
 * - Line-number gutter (LineNumberArea)
 * - Bracket / delimiter matching (BracketMatcher)
 * - Code folding regions (CodeFoldingManager)
 * - Auto-indentation & smart indent
 * - Word wrap toggle
 * - Zoom factor
 * - Encoding & BOM tracking
 * - EOL style tracking (LF / CRLF / CR)
 * - Overwrite / Insert mode toggle
 * - Banner widgets (file-changed notifications, etc.)
 * - Print / export to PDF
 */
class CodeEditorWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    // -- Theme support -------------------------------------------------------
    struct Theme
    {
        QString name;
        QString path;
        Theme(const QString &n = "default", const QString &p = {})
            : name(n), path(p) {}
    };

    // -- Construction --------------------------------------------------------

    explicit CodeEditorWidget(QWidget *parent = nullptr);
    explicit CodeEditorWidget(const Theme &theme, QWidget *parent = nullptr);
    ~CodeEditorWidget() override;

    /**
     * @brief Factory that returns an editor from an internal pre-allocated buffer.
     *
     * Pre-warming the buffer during idle time eliminates the latency spike
     * that would otherwise occur when the user opens a new tab.
     */
    [[nodiscard]] static std::shared_ptr<CodeEditorWidget> createBuffered(QWidget *parent = nullptr);

    /** @brief Pre-allocate editors into the buffer for later use. */
    static void warmBuffer(int count = 1);

    /** @brief Discard all pre-allocated editors (call on shutdown). */
    static void invalidateBuffer();

    // -- File association ----------------------------------------------------

    void       setFilePath(const QUrl &path);
    [[nodiscard]] QUrl filePath() const;

    [[nodiscard]] bool fileOnDiskChanged() const;
    void setFileOnDiskChanged(bool changed);

    // -- Content access ------------------------------------------------------

    /** @brief Replaces the entire document content and resets the undo stack. */
    void setContent(const QString &text);

    /** @brief Returns the full document text. */
    [[nodiscard]] QString content() const;

    // -- Dirty / clean state -------------------------------------------------

    [[nodiscard]] bool isClean() const;
    void markClean();
    void markDirty();

    // -- Language / highlighting ---------------------------------------------

    void setLanguage(const Language *lang);
    void setLanguage(const QString &languageId);
    void setLanguageFromFilePath(const QString &path);
    void setLanguageFromFilePath(); ///< Uses the current filePath()
    [[nodiscard]] const Language *currentLanguage() const;

    // -- Indentation ---------------------------------------------------------

    void setCustomIndentation(bool useTabs, int size = 0);
    void clearCustomIndentation();
    [[nodiscard]] bool isUsingCustomIndentation() const;
    [[nodiscard]] IndentationMode indentationMode() const;

    /** @brief Heuristically detects the indentation used in the document. */
    [[nodiscard]] std::optional<IndentationMode> detectDocumentIndentation() const;

    void setSmartIndent(bool enabled);

    // -- Encoding / EOL ------------------------------------------------------

    [[nodiscard]] QTextCodec *codec() const;
    void setCodec(QTextCodec *codec);

    [[nodiscard]] bool hasBom() const;
    void setBom(bool bom);

    [[nodiscard]] QString eolSequence() const;
    void setEolSequence(const QString &seq);

    // -- Visual settings -----------------------------------------------------

    [[nodiscard]] qreal zoomFactor() const;
    void setZoomFactor(qreal factor);

    void setEditorFont(const QString &family, int size, double lineHeight);
    void setLineNumbersVisible(bool visible);
    void setLineWrap(bool wrap);
    void setShowWhitespace(bool show);
    void setShowEndOfLine(bool show);
    void setShowTabs(bool show);
    void setOverwriteMode(bool overwrite);

    // -- Theme ---------------------------------------------------------------

    void setTheme(const Theme &theme);
    [[nodiscard]] QList<Theme> availableThemes() const;
    static Theme themeFromName(const QString &name);

    // -- Cursor helpers ------------------------------------------------------

    [[nodiscard]] CursorPosition cursorPos() const;
    void setCursorPos(int line, int column);
    void setCursorPos(const CursorPosition &pos);

    [[nodiscard]] QPair<int, int> scrollPosition() const;
    void setScrollPosition(int left, int top);

    // -- Selection -----------------------------------------------------------

    [[nodiscard]] QList<TextSelection> selections() const;
    [[nodiscard]] QStringList selectedTexts() const;
    void setSelectionsText(const QStringList &texts);
    void setSelection(int fromLine, int fromCol, int toLine, int toCol);

    // -- Document info -------------------------------------------------------

    [[nodiscard]] int lineCount() const;
    [[nodiscard]] QString currentWord() const;

    // -- Banner management ---------------------------------------------------

    void insertBanner(QWidget *banner);
    void removeBanner(QWidget *banner);
    void removeBanner(const QString &objectName);

    // -- Printing ------------------------------------------------------------

    void print(QPrinter *printer);
    [[nodiscard]] QByteArray printToPdf(const QPageLayout &layout = {}) const;

    // -- Loading flag (used during async file load) --------------------------

    bool isLoading = false;

signals:
    void contentChanged();
    void cursorActivity();
    void cleanChanged(bool isClean);
    void fileNameChanged(const QUrl &oldName, const QUrl &newName);
    void currentLanguageChanged(const QString &id, const QString &name);
    void bannerRemoved(QWidget *banner);
    void mouseWheelUsed(QWheelEvent *event);
    void urlsDropped(const QList<QUrl> &urls);

protected:
    // -- Event overrides -----------------------------------------------------
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    friend class ::EditorTabWidget;
    friend class LineNumberArea;

    // -- Initialisation helpers ----------------------------------------------
    void initEditor(const Theme &theme);
    void connectInternalSignals();

    // -- Gutter painting (called by LineNumberArea) --------------------------
    int  lineNumberAreaWidth() const;
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    // -- Auto-indent logic ---------------------------------------------------
    void handleAutoIndent(QKeyEvent *event);
    void handleTabKey(QKeyEvent *event);

    // -- Internal state ------------------------------------------------------
    QUrl                          m_filePath;
    QString                       m_tabName;
    bool                          m_fileOnDiskChanged  = false;
    QString                       m_eolSequence        = QStringLiteral("\n");
    QTextCodec                   *m_codec              = nullptr; // set in ctor
    bool                          m_bom                = false;
    bool                          m_customIndentation  = false;
    IndentationMode               m_indentation        = { false, 4 };
    bool                          m_smartIndent        = true;
    qreal                         m_zoomFactor         = 1.0;
    const Language               *m_currentLanguage    = nullptr;

    // -- Child widgets -------------------------------------------------------
    LineNumberArea               *m_lineNumberArea     = nullptr;
    MinimapWidget                *m_minimap            = nullptr;
    std::unique_ptr<SyntaxHighlighter>  m_highlighter;
    std::unique_ptr<BracketMatcher>     m_bracketMatcher;

    // -- Pre-allocation buffer -----------------------------------------------
    static QList<std::shared_ptr<CodeEditorWidget>> s_editorBuffer;

    // -- Tab name management (workaround for KDE title mangling) -------------
    [[nodiscard]] QString tabName() const;
    void setTabName(const QString &name);
};

} // namespace NovaPad::Editor

#endif // NOVAPAD_EDITOR_CODE_EDITOR_WIDGET_H
