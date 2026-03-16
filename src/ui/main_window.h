/**
 * @file main_window.h
 * @brief Primary application window for NovaPad.
 *
 * Owns the menu bar, toolbar, status bar, tab containers, search dialogs,
 * and dock widgets.  Multiple MainWindow instances can coexist (one per
 * "New Window" action).  A static registry tracks all live instances so
 * that inter-window operations (move tab to other window, etc.) work.
 *
 * This is the C++23 / Qt 6.10 equivalent of Notepadqq's MainWindow.
 */

#ifndef NOVAPAD_UI_MAIN_WINDOW_H
#define NOVAPAD_UI_MAIN_WINDOW_H

#include "editor/code_editor_widget.h"
#include "search/advanced_search_dock.h"
#include "search/search_replace_dialog.h"
#include "settings/novapad_settings.h"
#include "ui/editor_tab_widget.h"
#include "ui/top_editor_container.h"

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPrinter>
#include <QPushButton>
#include <QToolBar>

#include <functional>
#include <memory>

// Forward declarations for dock widgets
namespace NovaPad::UI::Dock {
class FileTreeDock;
class TerminalDock;
class OutputDock;
}

// Forward declaration for doc engine
namespace NovaPad::Utils { class DocEngine; }

namespace NovaPad::UI {

/**
 * @class MainWindow
 * @brief The top-level application window containing menus, toolbars, editors, and docks.
 *
 * ## Feature summary
 * - Multi-tab editing with split view
 * - Full menu bar mirroring all Notepadqq actions
 * - Configurable toolbar
 * - Status bar with cursor info, encoding, EOL, language
 * - Search / Replace dialog and Advanced Search dock
 * - File tree, integrated terminal, and output dock panels
 * - Drag-and-drop file opening
 * - Session save / restore
 * - Extension menu integration
 * - Print / PDF export
 * - Run external commands
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // -- Tab close result codes (mirrors original Notepadqq) -----------------
    enum TabCloseResult {
        TabClosed_Saved,         ///< Tab closed, file was saved
        TabClosed_NotSaved,      ///< Tab closed, file was NOT saved
        TabClosed_AlreadySaved,  ///< Tab closed, file was already clean
        TabClosed_Canceled       ///< User canceled the close
    };

    enum SaveChangesReason {
        SaveReason_TabClosing,
        SaveReason_Generic
    };

    // -- Construction --------------------------------------------------------

    explicit MainWindow(const QString &workingDirectory,
                        const QStringList &arguments,
                        QWidget *parent = nullptr);
    explicit MainWindow(const QStringList &arguments,
                        QWidget *parent = nullptr);
    ~MainWindow() override;

    // -- Static instance registry --------------------------------------------

    [[nodiscard]] static QList<MainWindow *> instances();
    [[nodiscard]] static MainWindow *lastActiveInstance();

    // -- Accessors -----------------------------------------------------------

    [[nodiscard]] TopEditorContainer *topEditorContainer() const;
    [[nodiscard]] NovaPad::Utils::DocEngine *docEngine() const;
    [[nodiscard]] std::shared_ptr<NovaPad::Editor::CodeEditorWidget> currentEditor();

    // -- Public operations ---------------------------------------------------

    void openCommandLineUrls(const QString &workingDir, const QStringList &args);
    void removeTabWidgetIfEmpty(EditorTabWidget *tabWidget);

    // -- Extension menu support ----------------------------------------------

    QAction *addExtensionMenuItem(const QString &extensionId, const QString &text);
    void showExtensionsMenu(bool show);

    // -- Toolbar management --------------------------------------------------

    [[nodiscard]] QString defaultToolBarString() const;
    [[nodiscard]] QToolBar *toolBar() const;
    [[nodiscard]] QList<QAction *> allActions() const;
    [[nodiscard]] QList<const QMenu *> allMenus() const;
    void loadToolBar();

    // -- Run menu ------------------------------------------------------------

    void generateRunMenu();

public slots:
    void refreshEditorUiInfo(std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor);
    void refreshCursorInfo();

signals:
    void windowClosed();

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    // -- File menu -----------------------------------------------------------
    void onActionNew();
    void onActionOpen();
    void onActionOpenFolder();
    void onActionSave();
    void onActionSaveAs();
    void onActionSaveCopyAs();
    void onActionSaveAll();
    void onActionClose();
    void onActionCloseAll();
    void onActionCloseAllButCurrent();
    void onActionCloseLeft();
    void onActionCloseRight();
    void onActionReloadFromDisk();
    void onActionRename();
    void onActionPrint();
    void onActionPrintNow();
    void onActionExit();

    // -- Edit menu -----------------------------------------------------------
    void onActionUndo();
    void onActionRedo();
    void onActionCut();
    void onActionCopy();
    void onActionPaste();
    void onActionDelete();
    void onActionSelectAll();
    void onActionBeginEndSelect();
    void onActionUpperCase();
    void onActionLowerCase();
    void onActionDeleteLine();
    void onActionDuplicateLine();
    void onActionMoveLineUp();
    void onActionMoveLineDown();
    void onActionTrimTrailingSpace();
    void onActionTrimLeadingSpace();
    void onActionTrimBothSpace();
    void onActionEolToSpace();
    void onActionTabToSpace();
    void onActionSpaceToTabAll();
    void onActionSpaceToTabLeading();

    // -- Search menu ---------------------------------------------------------
    void onActionSearch();
    void onActionReplace();
    void onActionFindNext();
    void onActionFindPrevious();
    void onActionFindInFiles();
    void onActionGoToLine();

    // -- View menu -----------------------------------------------------------
    void onActionWordWrap(bool on);
    void onActionFullScreen(bool on);
    void onActionShowEndOfLine(bool on);
    void onActionShowAllCharacters(bool on);
    void onActionShowSpaces(bool on);
    void onActionShowTabs(bool on);
    void onActionShowMenuBar(bool on);
    void onActionShowToolBar(bool on);
    void onActionZoomIn();
    void onActionZoomOut();
    void onActionRestoreZoom();
    void onActionSmartIndent(bool on);
    void onActionMathRendering(bool on);
    void onActionToggleFormerTab();

    // -- Encoding menu -------------------------------------------------------
    void onActionUtf8();
    void onActionUtf8NoBom();
    void onActionUtf16BE();
    void onActionUtf16LE();
    void onActionInterpretAsUtf8();
    void onActionInterpretAsUtf8NoBom();
    void onActionInterpretAsUtf16BE();
    void onActionInterpretAsUtf16LE();
    void onActionConvertEncoding();
    void onActionReloadInterpretedAs();

    // -- EOL format ----------------------------------------------------------
    void onActionUnixFormat();
    void onActionWindowsFormat();
    void onActionMacFormat();

    // -- Language menu -------------------------------------------------------
    void setCurrentEditorLanguage(const QString &language);
    void onActionPlainText();
    void onActionIndentDefault();
    void onActionIndentCustom();

    // -- Theme handling ------------------------------------------------------
    void onThemeChanged(const QString &themeId);

    // -- Window menu ---------------------------------------------------------
    void onActionNewWindow();
    void onActionOpenInNewWindow();
    void onActionMoveToNewWindow();
    void onActionMoveToOtherView();

    // -- Session menu --------------------------------------------------------
    void onActionLoadSession();
    void onActionSaveSession();

    // -- Extensions menu -----------------------------------------------------
    void onActionInstallExtension();

    // -- Clipboard path actions ----------------------------------------------
    void onActionCopyFullPath();
    void onActionCopyFileName();
    void onActionCopyDirectory();

    // -- Recent files --------------------------------------------------------
    void onActionEmptyRecentFiles();
    void onActionOpenAllRecent();

    // -- About ---------------------------------------------------------------
    void onActionAboutNovaPad();
    void onActionAboutQt();

    // -- Run menu ------------------------------------------------------------
    void onRunCommand();
    void onModifyRunCommands();

    // -- Internal signals from child widgets ----------------------------------
    void onTabCloseRequested(EditorTabWidget *tabWidget, int tab);
    void onCurrentEditorChanged(EditorTabWidget *tabWidget, int tab);
    void onEditorAdded(EditorTabWidget *tabWidget, int tab);
    void onTabContextMenu(QPoint point, EditorTabWidget *tabWidget, int tab);
    void onTabBarDoubleClicked(EditorTabWidget *tabWidget, int tab);
    void onEditorMouseWheel(EditorTabWidget *tabWidget, int tab, QWheelEvent *ev);
    void onEditorUrlsDropped(const QList<QUrl> &urls);
    void onFileOnDiskChanged(EditorTabWidget *tabWidget, int tab, bool removed);
    void onDocumentSaved(EditorTabWidget *tabWidget, int tab);
    void onDocumentReloaded(EditorTabWidget *tabWidget, int tab);
    void onDocumentLoaded(EditorTabWidget *tabWidget, int tab,
                          bool wasAlreadyOpened, bool updateRecentDocs);
    void onBannerRemoved(QWidget *banner);
    void onSearchDockItemInteracted(/*const DocResult &doc, const MatchResult *result, int type*/);

private:
    // -- Static registry -----------------------------------------------------
    static QList<MainWindow *> s_instances;

    // -- Initialisation helpers ----------------------------------------------
    void commonInit(const QString &workingDir, const QStringList &args);
    void createActions();
    void createMenus();
    void createStatusBar();
    void connectEditorContainerSignals();
    void setupLanguagesMenu();
    void restoreWindowSettings();
    void loadIcons();
    void updateRecentDocsMenu();
    void configureUserInterface();
    void fixKeyboardShortcuts();

    // -- Tab management helpers ----------------------------------------------
    int  askIfWantToSave(EditorTabWidget *tabWidget, int tab, int reason);
    int  closeTab(EditorTabWidget *tabWidget, int tab,
                  bool remove = true, bool force = false);
    int  save(EditorTabWidget *tabWidget, int tab);
    int  saveAs(EditorTabWidget *tabWidget, int tab, bool copy);
    QUrl getSaveDialogDefaultFileName(EditorTabWidget *tabWidget, int tab);
    bool saveTabsToCache();
    bool finalizeAllTabs();
    void transformSelectedText(std::function<QString(const QString &)> func);
    void checkIndentationMode(std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor);
    void openRecentFileEntry(const QUrl &url);
    QUrl stringToUrl(const QString &fileName, const QString &workingDir = {});
    void toggleOverwrite();
    bool updateSymbols(bool on);

    // -- Member data ---------------------------------------------------------
    TopEditorContainer            *m_topEditorContainer  = nullptr;
    NovaPad::Utils::DocEngine     *m_docEngine           = nullptr;
    QToolBar                      *m_mainToolBar         = nullptr;
    NovaPad::Settings::NovaPadSettings &m_settings;

    // -- Status bar widgets --------------------------------------------------
    QLabel      *m_sbDocInfoLabel    = nullptr;
    QPushButton *m_sbFileFormatBtn   = nullptr;
    QPushButton *m_sbEolFormatBtn    = nullptr;
    QPushButton *m_sbTextFormatBtn   = nullptr;
    QPushButton *m_sbOvertypeBtn     = nullptr;

    // -- Search subsystem ----------------------------------------------------
    NovaPad::Search::SearchReplaceDialog *m_searchReplaceDialog = nullptr;
    NovaPad::Search::AdvancedSearchDock  *m_advSearchDock       = nullptr;

    // -- Dock widgets --------------------------------------------------------
    NovaPad::UI::Dock::FileTreeDock  *m_fileTreeDock  = nullptr;
    NovaPad::UI::Dock::TerminalDock  *m_terminalDock  = nullptr;
    NovaPad::UI::Dock::OutputDock    *m_outputDock    = nullptr;

    // -- Tab context menu ----------------------------------------------------
    QMenu              *m_tabContextMenu = nullptr;
    QList<QAction *>    m_tabContextMenuActions;

    // -- State ---------------------------------------------------------------
    QString   m_workingDirectory;
    bool      m_overwrite           = false;
    QPair<int, int> m_beginSelectPos;
    bool      m_beginSelectPosSet   = false;

    // -- Extension menus -----------------------------------------------------
    QMap<QString, QMenu *> m_extensionMenus;
};

} // namespace NovaPad::UI

#endif // NOVAPAD_UI_MAIN_WINDOW_H
