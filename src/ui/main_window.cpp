/**
 * @file main_window.cpp
 * @brief Implementation of the primary NovaPad application window.
 *
 * This is the largest file in the project — it wires together the menu bar,
 * toolbar, status bar, editor containers, search dialogs, dock widgets, and
 * handles all user actions.  It is the C++23 / Qt 6.10 rewrite of
 * Notepadqq's mainwindow.cpp.
 *
 * The class follows the same architecture as the original:
 *   - A static list of MainWindow instances for multi-window support.
 *   - A TopEditorContainer that holds one or more EditorTabWidgets.
 *   - A DocEngine responsible for all file I/O and monitoring.
 *   - A configurable toolbar and comprehensive menu bar.
 */

#include "main_window.h"

#include "core/command_line_parser.h"
#include "core/globals.h"
#include "editor/language_manager.h"
#include "search/advanced_search_dock.h"
#include "search/search_replace_dialog.h"
#include "sessions/backup_service.h"
#include "sessions/session_manager.h"
#include "settings/novapad_settings.h"
#include "themes/theme_manager.h"
#include "ui/dialogs/about_dialog.h"
#include "ui/dialogs/encoding_chooser_dialog.h"
#include "ui/dialogs/go_to_line_dialog.h"
#include "ui/dialogs/indentation_dialog.h"
#include "ui/dialogs/preferences_dialog.h"
#include "ui/dialogs/run_command_dialog.h"
#include "ui/dock/file_tree_dock.h"
#include "ui/dock/output_dock.h"
#include "ui/dock/terminal_dock.h"
#include "ui/icon_provider.h"
#include "ui/top_editor_container.h"
#include "utils/file_utils.h"
#include "utils/text_operations.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QStatusBar>
#include <QTextCodec>

namespace NovaPad::UI {

// ---------------------------------------------------------------------------
// Static instance registry
// ---------------------------------------------------------------------------

QList<MainWindow *> MainWindow::s_instances;

QList<MainWindow *> MainWindow::instances() { return s_instances; }

MainWindow *MainWindow::lastActiveInstance()
{
    return s_instances.isEmpty() ? nullptr : s_instances.last();
}

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

MainWindow::MainWindow(const QString &workingDirectory,
                       const QStringList &arguments,
                       QWidget *parent)
    : QMainWindow(parent)
    , m_settings(NovaPad::Settings::NovaPadSettings::instance())
{
    commonInit(workingDirectory, arguments);
}

MainWindow::MainWindow(const QStringList &arguments, QWidget *parent)
    : QMainWindow(parent)
    , m_settings(NovaPad::Settings::NovaPadSettings::instance())
{
    commonInit(QDir::currentPath(), arguments);
}

MainWindow::~MainWindow()
{
    s_instances.removeAll(this);
}

void MainWindow::commonInit(const QString &workingDir, const QStringList & /*args*/)
{
    s_instances.append(this);
    m_workingDirectory = workingDir;

    setWindowTitle(QStringLiteral("NovaPad"));
    setMinimumSize(800, 600);
    resize(1280, 800);
    setAcceptDrops(true);

    // --- Core subsystems ---
    m_topEditorContainer = new TopEditorContainer(this);
    setCentralWidget(m_topEditorContainer);

    // DocEngine would be created here (manages file I/O and monitoring)
    // m_docEngine = new NovaPad::Utils::DocEngine(m_topEditorContainer, this);

    // --- Build the UI ---
    createActions();
    createMenus();
    loadToolBar();
    createStatusBar();
    connectEditorContainerSignals();
    setupLanguagesMenu();
    restoreWindowSettings();
    configureUserInterface();
    loadIcons();

    // --- Dock widgets ---
    m_fileTreeDock = new NovaPad::UI::Dock::FileTreeDock(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_fileTreeDock);
    m_fileTreeDock->hide(); // Hidden by default

    m_terminalDock = new NovaPad::UI::Dock::TerminalDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_terminalDock);
    m_terminalDock->hide(); // Hidden by default

    m_outputDock = new NovaPad::UI::Dock::OutputDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_outputDock);
    m_outputDock->hide(); // Hidden by default

    // --- Advanced Search Dock ---
    m_advSearchDock = new NovaPad::Search::AdvancedSearchDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, m_advSearchDock->dockWidget());
    m_advSearchDock->dockWidget()->hide();

    // Ensure at least one editor tab is open
    if (m_topEditorContainer->count() == 0) {
        auto *tw = m_topEditorContainer->addTabWidget();
        tw->addEditorTab(true, tr("New 1"));
    }
}

// ---------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------

TopEditorContainer *MainWindow::topEditorContainer() const
{
    return m_topEditorContainer;
}

NovaPad::Utils::DocEngine *MainWindow::docEngine() const
{
    return m_docEngine;
}

std::shared_ptr<NovaPad::Editor::CodeEditorWidget> MainWindow::currentEditor()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return nullptr;
    return tw->currentEditor();
}

// ---------------------------------------------------------------------------
// Action creation
// ---------------------------------------------------------------------------

void MainWindow::createActions()
{
    // Actions are created by createMenus() — Qt Designer-style action objects.
    // This method exists as a hook for any pre-menu action setup.
}

// ---------------------------------------------------------------------------
// Menu creation — mirrors the full Notepadqq menu structure
// ---------------------------------------------------------------------------

void MainWindow::createMenus()
{
    // ===== File Menu ========================================================
    auto *fileMenu = menuBar()->addMenu(tr("&File"));

    auto *actNew = fileMenu->addAction(tr("&New"));
    actNew->setShortcut(QKeySequence::New);
    connect(actNew, &QAction::triggered, this, &MainWindow::onActionNew);
    actNew->setObjectName("actionNew");

    auto *actOpen = fileMenu->addAction(tr("&Open..."));
    actOpen->setShortcut(QKeySequence::Open);
    connect(actOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    actOpen->setObjectName("actionOpen");

    auto *actOpenFolder = fileMenu->addAction(tr("Open Fol&der..."), this,
                                              &MainWindow::onActionOpenFolder);
    actOpenFolder->setObjectName("actionOpenFolder");

    // -- Recent Files submenu ---
    auto *recentMenu = fileMenu->addMenu(tr("Open &Recent"));
    recentMenu->setObjectName("menuRecentFiles");
    recentMenu->addAction(tr("Open All Recent Files"), this, &MainWindow::onActionOpenAllRecent);
    recentMenu->addSeparator();
    recentMenu->addAction(tr("Empty Recent Files List"), this, &MainWindow::onActionEmptyRecentFiles);

    fileMenu->addSeparator();

    auto *reloadAction = fileMenu->addAction(tr("Re&load from Disk"));
    reloadAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_R));
    connect(reloadAction, &QAction::triggered, this, &MainWindow::onActionReloadFromDisk);
    fileMenu->addAction(tr("Rena&me"), this, &MainWindow::onActionRename);

    fileMenu->addSeparator();

    auto *saveAction2 = fileMenu->addAction(tr("&Save"));
    saveAction2->setShortcut(QKeySequence::Save);
    connect(saveAction2, &QAction::triggered, this, &MainWindow::onActionSave);
    auto *saveAsAction = fileMenu->addAction(tr("Save &As..."));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::onActionSaveAs);
    fileMenu->addAction(tr("Save a Cop&y As..."), this, &MainWindow::onActionSaveCopyAs);
    auto *saveAllAction2 = fileMenu->addAction(tr("Sav&e All"));
    saveAllAction2->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAllAction2, &QAction::triggered, this, &MainWindow::onActionSaveAll);

    fileMenu->addSeparator();

    auto *closeAction2 = fileMenu->addAction(tr("&Close"));
    closeAction2->setShortcut(QKeySequence::Close);
    connect(closeAction2, &QAction::triggered, this, &MainWindow::onActionClose);
    fileMenu->addAction(tr("Cl&ose All"), this, &MainWindow::onActionCloseAll);
    fileMenu->addAction(tr("Close All BUT Current"), this, &MainWindow::onActionCloseAllButCurrent);
    fileMenu->addAction(tr("Close Left Tabs"), this, &MainWindow::onActionCloseLeft);
    fileMenu->addAction(tr("Close Right Tabs"), this, &MainWindow::onActionCloseRight);

    fileMenu->addSeparator();

    auto *printAction = fileMenu->addAction(tr("&Print..."));
    printAction->setShortcut(QKeySequence::Print);
    connect(printAction, &QAction::triggered, this, &MainWindow::onActionPrint);
    fileMenu->addAction(tr("Print Now"), this, &MainWindow::onActionPrintNow);

    fileMenu->addSeparator();

    // -- Session submenu ---
    auto *sessionMenu = fileMenu->addMenu(tr("S&essions"));
    sessionMenu->addAction(tr("&Load Session..."), this, &MainWindow::onActionLoadSession);
    sessionMenu->addAction(tr("&Save Session..."), this, &MainWindow::onActionSaveSession);

    fileMenu->addSeparator();

    auto *exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence(Qt::ALT | Qt::Key_F4));
    connect(exitAction, &QAction::triggered, this, &MainWindow::onActionExit);

    // ===== Edit Menu ========================================================
    auto *editMenu = menuBar()->addMenu(tr("&Edit"));

    auto *undoAction = editMenu->addAction(tr("&Undo"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onActionUndo);
    auto *redoAction = editMenu->addAction(tr("&Redo"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onActionRedo);

    editMenu->addSeparator();

    auto *cutAction2 = editMenu->addAction(tr("Cu&t"));
    cutAction2->setShortcut(QKeySequence::Cut);
    connect(cutAction2, &QAction::triggered, this, &MainWindow::onActionCut);
    auto *copyAction2 = editMenu->addAction(tr("&Copy"));
    copyAction2->setShortcut(QKeySequence::Copy);
    connect(copyAction2, &QAction::triggered, this, &MainWindow::onActionCopy);
    auto *pasteAction2 = editMenu->addAction(tr("&Paste"));
    pasteAction2->setShortcut(QKeySequence::Paste);
    connect(pasteAction2, &QAction::triggered, this, &MainWindow::onActionPaste);
    auto *deleteAction = editMenu->addAction(tr("&Delete"));
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onActionDelete);
    auto *selectAllAction = editMenu->addAction(tr("Select &All"));
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, this, &MainWindow::onActionSelectAll);

    editMenu->addSeparator();

    editMenu->addAction(tr("Begin/End Select"), this, &MainWindow::onActionBeginEndSelect);
    auto *upperAction = editMenu->addAction(tr("&UPPERCASE"));
    upperAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_U));
    connect(upperAction, &QAction::triggered, this, &MainWindow::onActionUpperCase);
    auto *lowerAction = editMenu->addAction(tr("&lowercase"));
    lowerAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U));
    connect(lowerAction, &QAction::triggered, this, &MainWindow::onActionLowerCase);

    editMenu->addSeparator();

    // -- Line operations submenu ---
    auto *lineOpsMenu = editMenu->addMenu(tr("Line &Operations"));
    auto *delLineAction = lineOpsMenu->addAction(tr("Delete Line"));
    delLineAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_K));
    connect(delLineAction, &QAction::triggered, this, &MainWindow::onActionDeleteLine);
    auto *dupLineAction = lineOpsMenu->addAction(tr("Duplicate Line"));
    dupLineAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(dupLineAction, &QAction::triggered, this, &MainWindow::onActionDuplicateLine);
    auto *moveUpAction = lineOpsMenu->addAction(tr("Move Line Up"));
    moveUpAction->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Up));
    connect(moveUpAction, &QAction::triggered, this, &MainWindow::onActionMoveLineUp);
    auto *moveDownAction = lineOpsMenu->addAction(tr("Move Line Down"));
    moveDownAction->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Down));
    connect(moveDownAction, &QAction::triggered, this, &MainWindow::onActionMoveLineDown);

    // -- Blank operations submenu ---
    auto *blankOpsMenu = editMenu->addMenu(tr("&Blank Operations"));
    blankOpsMenu->addAction(tr("Trim Trailing Space"), this, &MainWindow::onActionTrimTrailingSpace);
    blankOpsMenu->addAction(tr("Trim Leading Space"), this, &MainWindow::onActionTrimLeadingSpace);
    blankOpsMenu->addAction(tr("Trim Leading and Trailing"), this, &MainWindow::onActionTrimBothSpace);
    blankOpsMenu->addAction(tr("EOL to Space"), this, &MainWindow::onActionEolToSpace);
    blankOpsMenu->addAction(tr("TAB to Space"), this, &MainWindow::onActionTabToSpace);
    blankOpsMenu->addAction(tr("Space to TAB (All)"), this, &MainWindow::onActionSpaceToTabAll);
    blankOpsMenu->addAction(tr("Space to TAB (Leading)"), this, &MainWindow::onActionSpaceToTabLeading);

    editMenu->addSeparator();

    // -- Clipboard path submenu ---
    auto *clipPathMenu = editMenu->addMenu(tr("Copy to Clipboar&d"));
    clipPathMenu->addAction(tr("Current Full File Path"), this, &MainWindow::onActionCopyFullPath);
    clipPathMenu->addAction(tr("Current Filename"), this, &MainWindow::onActionCopyFileName);
    clipPathMenu->addAction(tr("Current Directory Path"), this, &MainWindow::onActionCopyDirectory);

    // ===== Search Menu ======================================================
    auto *searchMenu = menuBar()->addMenu(tr("&Search"));
    auto *findAction = searchMenu->addAction(tr("&Find..."));
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, &QAction::triggered, this, &MainWindow::onActionSearch);
    auto *findNextAction = searchMenu->addAction(tr("Find &Next"));
    findNextAction->setShortcut(QKeySequence(Qt::Key_F3));
    connect(findNextAction, &QAction::triggered, this, &MainWindow::onActionFindNext);
    auto *findPrevAction = searchMenu->addAction(tr("Find &Previous"));
    findPrevAction->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F3));
    connect(findPrevAction, &QAction::triggered, this, &MainWindow::onActionFindPrevious);
    auto *replaceAction = searchMenu->addAction(tr("&Replace..."));
    replaceAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    connect(replaceAction, &QAction::triggered, this, &MainWindow::onActionReplace);
    searchMenu->addSeparator();
    searchMenu->addAction(tr("Find in F&iles..."), this, &MainWindow::onActionFindInFiles);
    searchMenu->addSeparator();
    auto *gotoAction = searchMenu->addAction(tr("&Go to Line..."));
    gotoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    connect(gotoAction, &QAction::triggered, this, &MainWindow::onActionGoToLine);

    // ===== View Menu ========================================================
    auto *viewMenu = menuBar()->addMenu(tr("&View"));

    auto *actWordWrap = viewMenu->addAction(tr("&Word Wrap"));
    actWordWrap->setCheckable(true);
    connect(actWordWrap, &QAction::toggled, this, &MainWindow::onActionWordWrap);

    auto *actFullScreen = viewMenu->addAction(tr("Full &Screen"));
    actFullScreen->setCheckable(true);
    actFullScreen->setShortcut(QKeySequence(Qt::Key_F11));
    connect(actFullScreen, &QAction::toggled, this, &MainWindow::onActionFullScreen);

    viewMenu->addSeparator();

    // -- Show symbols submenu ---
    auto *showMenu = viewMenu->addMenu(tr("Show S&ymbols"));
    auto *actShowEol = showMenu->addAction(tr("Show End of Line"));
    actShowEol->setCheckable(true);
    connect(actShowEol, &QAction::toggled, this, &MainWindow::onActionShowEndOfLine);

    auto *actShowSpaces = showMenu->addAction(tr("Show Spaces"));
    actShowSpaces->setCheckable(true);
    connect(actShowSpaces, &QAction::toggled, this, &MainWindow::onActionShowSpaces);

    auto *actShowTabs = showMenu->addAction(tr("Show Tabs"));
    actShowTabs->setCheckable(true);
    connect(actShowTabs, &QAction::toggled, this, &MainWindow::onActionShowTabs);

    auto *actShowAll = showMenu->addAction(tr("Show All Characters"));
    actShowAll->setCheckable(true);
    connect(actShowAll, &QAction::toggled, this, &MainWindow::onActionShowAllCharacters);

    viewMenu->addSeparator();

    auto *zoomInAction = viewMenu->addAction(tr("Zoom &In"));
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::onActionZoomIn);
    auto *zoomOutAction = viewMenu->addAction(tr("Zoom &Out"));
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::onActionZoomOut);
    auto *zoomResetAction = viewMenu->addAction(tr("Restore Default &Zoom"));
    zoomResetAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(zoomResetAction, &QAction::triggered, this, &MainWindow::onActionRestoreZoom);

    viewMenu->addSeparator();

    auto *actSmartIndent = viewMenu->addAction(tr("S&mart Indent"));
    actSmartIndent->setCheckable(true);
    actSmartIndent->setChecked(true);
    connect(actSmartIndent, &QAction::toggled, this, &MainWindow::onActionSmartIndent);

    auto *actMath = viewMenu->addAction(tr("Math &Rendering"));
    actMath->setCheckable(true);
    connect(actMath, &QAction::toggled, this, &MainWindow::onActionMathRendering);

    viewMenu->addSeparator();

    auto *actMenuBar = viewMenu->addAction(tr("Show Menu &Bar"));
    actMenuBar->setCheckable(true);
    actMenuBar->setChecked(true);
    connect(actMenuBar, &QAction::toggled, this, &MainWindow::onActionShowMenuBar);

    auto *actToolBar = viewMenu->addAction(tr("Show &Toolbar"));
    actToolBar->setCheckable(true);
    actToolBar->setChecked(true);
    connect(actToolBar, &QAction::toggled, this, &MainWindow::onActionShowToolBar);

    // ===== Encoding Menu ====================================================
    auto *encMenu = menuBar()->addMenu(tr("E&ncoding"));
    encMenu->addAction(tr("UTF-8"), this, &MainWindow::onActionUtf8);
    encMenu->addAction(tr("UTF-8 without BOM"), this, &MainWindow::onActionUtf8NoBom);
    encMenu->addAction(tr("UTF-16 BE"), this, &MainWindow::onActionUtf16BE);
    encMenu->addAction(tr("UTF-16 LE"), this, &MainWindow::onActionUtf16LE);
    encMenu->addSeparator();
    encMenu->addAction(tr("Interpret as UTF-8"), this, &MainWindow::onActionInterpretAsUtf8);
    encMenu->addAction(tr("Interpret as UTF-8 (no BOM)"), this, &MainWindow::onActionInterpretAsUtf8NoBom);
    encMenu->addAction(tr("Interpret as UTF-16 BE"), this, &MainWindow::onActionInterpretAsUtf16BE);
    encMenu->addAction(tr("Interpret as UTF-16 LE"), this, &MainWindow::onActionInterpretAsUtf16LE);
    encMenu->addSeparator();
    encMenu->addAction(tr("Convert to..."), this, &MainWindow::onActionConvertEncoding);
    encMenu->addAction(tr("Reload File Interpreted As..."), this, &MainWindow::onActionReloadInterpretedAs);

    // ===== Language Menu ====================================================
    auto *langMenu = menuBar()->addMenu(tr("&Language"));
    langMenu->addAction(tr("&Plain Text"), this, &MainWindow::onActionPlainText);
    langMenu->addSeparator();
    // Languages are populated in setupLanguagesMenu()

    // ===== EOL Format Menu ==================================================
    auto *eolMenu = menuBar()->addMenu(tr("EOL &Format"));
    eolMenu->addAction(tr("UNIX (LF)"), this, &MainWindow::onActionUnixFormat);
    eolMenu->addAction(tr("Windows (CR LF)"), this, &MainWindow::onActionWindowsFormat);
    eolMenu->addAction(tr("Old Mac (CR)"), this, &MainWindow::onActionMacFormat);

    // ===== Indentation Menu =================================================
    auto *indentMenu = menuBar()->addMenu(tr("&Indentation"));
    indentMenu->addAction(tr("Default Settings..."), this, &MainWindow::onActionIndentDefault);
    indentMenu->addAction(tr("Custom..."), this, &MainWindow::onActionIndentCustom);

    // ===== Window Menu ======================================================
    auto *windowMenu = menuBar()->addMenu(tr("&Window"));
    windowMenu->addAction(tr("Open a &New Window"), this, &MainWindow::onActionNewWindow);
    windowMenu->addAction(tr("Open in New &Window"), this, &MainWindow::onActionOpenInNewWindow);
    windowMenu->addAction(tr("&Move to New Window"), this, &MainWindow::onActionMoveToNewWindow);
    windowMenu->addAction(tr("Move to &Other View"), this, &MainWindow::onActionMoveToOtherView);
    windowMenu->addSeparator();
    auto *toggleTabAction = windowMenu->addAction(tr("Toggle to &Former Tab"));
    toggleTabAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Tab));
    connect(toggleTabAction, &QAction::triggered, this, &MainWindow::onActionToggleFormerTab);

    // ===== Run Menu =========================================================
    auto *runMenu = menuBar()->addMenu(tr("&Run"));
    auto *runCmdAction = runMenu->addAction(tr("&Run Command..."));
    runCmdAction->setShortcut(QKeySequence(Qt::Key_F5));
    connect(runCmdAction, &QAction::triggered, this, &MainWindow::onRunCommand);
    runMenu->addAction(tr("&Modify Run Commands..."), this, &MainWindow::onModifyRunCommands);

    // ===== Extensions Menu ==================================================
    auto *extMenu = menuBar()->addMenu(tr("E&xtensions"));
    extMenu->addAction(tr("&Install Extension..."), this, &MainWindow::onActionInstallExtension);
    extMenu->setObjectName("menuExtensions");

    // ===== Settings Menu ====================================================
    auto *settMenu = menuBar()->addMenu(tr("&Settings"));
    auto *prefAction = settMenu->addAction(tr("&Preferences..."));
    prefAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Comma));
    connect(prefAction, &QAction::triggered, this, [this]() {
        NovaPad::UI::Dialogs::PreferencesDialog dlg(this);
        dlg.exec();
    });

    // ===== Help Menu ========================================================
    auto *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About NovaPad"), this, &MainWindow::onActionAboutNovaPad);
    helpMenu->addAction(tr("About &Qt"), this, &MainWindow::onActionAboutQt);
}

// ---------------------------------------------------------------------------
// Status bar
// ---------------------------------------------------------------------------

void MainWindow::createStatusBar()
{
    m_sbDocInfoLabel  = new QLabel(this);
    m_sbFileFormatBtn = new QPushButton(this);
    m_sbEolFormatBtn  = new QPushButton(this);
    m_sbTextFormatBtn = new QPushButton(this);
    m_sbOvertypeBtn   = new QPushButton(this);

    m_sbFileFormatBtn->setFlat(true);
    m_sbEolFormatBtn->setFlat(true);
    m_sbTextFormatBtn->setFlat(true);
    m_sbOvertypeBtn->setFlat(true);

    m_sbOvertypeBtn->setText(tr("INS"));

    statusBar()->addPermanentWidget(m_sbDocInfoLabel);
    statusBar()->addPermanentWidget(m_sbTextFormatBtn);
    statusBar()->addPermanentWidget(m_sbEolFormatBtn);
    statusBar()->addPermanentWidget(m_sbFileFormatBtn);
    statusBar()->addPermanentWidget(m_sbOvertypeBtn);
}

// ---------------------------------------------------------------------------
// Toolbar
// ---------------------------------------------------------------------------

void MainWindow::loadToolBar()
{
    if (m_mainToolBar) {
        removeToolBar(m_mainToolBar);
        delete m_mainToolBar;
    }

    m_mainToolBar = addToolBar(tr("Main Toolbar"));
    m_mainToolBar->setObjectName("mainToolBar");
    m_mainToolBar->setMovable(false);

    // Default toolbar actions — use IconProvider for theme-aware icons
    auto *newAction = m_mainToolBar->addAction(IconProvider::icon("document-new"), tr("New"));
    connect(newAction, &QAction::triggered, this, &MainWindow::onActionNew);
    auto *openAction = m_mainToolBar->addAction(IconProvider::icon("document-open"), tr("Open"));
    connect(openAction, &QAction::triggered, this, &MainWindow::onActionOpen);
    auto *saveAction = m_mainToolBar->addAction(IconProvider::icon("document-save"), tr("Save"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::onActionSave);
    auto *saveAllAction = m_mainToolBar->addAction(IconProvider::icon("document-save-all"), tr("Save All"));
    connect(saveAllAction, &QAction::triggered, this, &MainWindow::onActionSaveAll);
    auto *closeAction = m_mainToolBar->addAction(IconProvider::icon("document-close"), tr("Close"));
    connect(closeAction, &QAction::triggered, this, &MainWindow::onActionClose);
    m_mainToolBar->addSeparator();
    auto *undoAction = m_mainToolBar->addAction(IconProvider::icon("edit-undo"), tr("Undo"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::onActionUndo);
    auto *redoAction = m_mainToolBar->addAction(IconProvider::icon("edit-redo"), tr("Redo"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::onActionRedo);
    m_mainToolBar->addSeparator();
    auto *cutAction = m_mainToolBar->addAction(IconProvider::icon("edit-cut"), tr("Cut"));
    connect(cutAction, &QAction::triggered, this, &MainWindow::onActionCut);
    auto *copyAction = m_mainToolBar->addAction(IconProvider::icon("edit-copy"), tr("Copy"));
    connect(copyAction, &QAction::triggered, this, &MainWindow::onActionCopy);
    auto *pasteAction = m_mainToolBar->addAction(IconProvider::icon("edit-paste"), tr("Paste"));
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onActionPaste);
    m_mainToolBar->addSeparator();
    auto *findAction = m_mainToolBar->addAction(IconProvider::icon("edit-find"), tr("Find"));
    connect(findAction, &QAction::triggered, this, &MainWindow::onActionSearch);
    auto *replaceAction = m_mainToolBar->addAction(IconProvider::icon("edit-find-replace"), tr("Replace"));
    connect(replaceAction, &QAction::triggered, this, &MainWindow::onActionReplace);
    m_mainToolBar->addSeparator();
    auto *zoomInAction = m_mainToolBar->addAction(IconProvider::icon("zoom-in"), tr("Zoom In"));
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::onActionZoomIn);
    auto *zoomOutAction = m_mainToolBar->addAction(IconProvider::icon("zoom-out"), tr("Zoom Out"));
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::onActionZoomOut);
    auto *zoomResetAction = m_mainToolBar->addAction(IconProvider::icon("zoom-original"), tr("Zoom Reset"));
    connect(zoomResetAction, &QAction::triggered, this, &MainWindow::onActionRestoreZoom);
}

QToolBar *MainWindow::toolBar() const { return m_mainToolBar; }

QString MainWindow::defaultToolBarString() const
{
    return QStringLiteral("actionNew|actionOpen|actionSave|actionSaveAll|actionClose|"
                          "|actionUndo|actionRedo||actionCut|actionCopy|actionPaste|"
                          "|actionSearch|actionReplace||actionZoomIn|actionZoomOut|actionZoomReset");
}

QList<QAction *> MainWindow::allActions() const
{
    return findChildren<QAction *>();
}

QList<const QMenu *> MainWindow::allMenus() const
{
    QList<const QMenu *> result;
    for (auto *m : menuBar()->findChildren<QMenu *>()) {
        result.append(m);
    }
    return result;
}

// ---------------------------------------------------------------------------
// Signal connections
// ---------------------------------------------------------------------------

void MainWindow::connectEditorContainerSignals()
{
    connect(m_topEditorContainer, &TopEditorContainer::currentEditorChanged,
            this, &MainWindow::onCurrentEditorChanged);
    connect(m_topEditorContainer, &TopEditorContainer::tabCloseRequested,
            this, &MainWindow::onTabCloseRequested);
    connect(m_topEditorContainer, &TopEditorContainer::editorAdded,
            this, &MainWindow::onEditorAdded);
    connect(m_topEditorContainer, &TopEditorContainer::customTabContextMenuRequested,
            this, &MainWindow::onTabContextMenu);
    connect(m_topEditorContainer, &TopEditorContainer::tabBarDoubleClicked,
            this, &MainWindow::onTabBarDoubleClicked);
    connect(m_topEditorContainer, &TopEditorContainer::editorMouseWheel,
            this, &MainWindow::onEditorMouseWheel);
}

// ---------------------------------------------------------------------------
// File actions implementation
// ---------------------------------------------------------------------------

void MainWindow::onActionNew()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    static int newDocCounter = 1;
    tw->addEditorTab(true, tr("New %1").arg(newDocCounter++));
}

void MainWindow::onActionOpen()
{
    const QString dir = m_settings.general().lastSelectedDir();
    const QStringList files = QFileDialog::getOpenFileNames(
        this, tr("Open File"), dir, tr("All Files (*)"));

    if (files.isEmpty()) return;

    m_settings.general().setLastSelectedDir(QFileInfo(files.first()).absolutePath());

    for (const QString &file : files) {
        // DocEngine would load the file here
        auto *tw = m_topEditorContainer->currentTabWidget();
        if (tw) {
            int idx = tw->addEditorTab(true, QFileInfo(file).fileName());
            auto editor = tw->editor(idx);
            if (editor) {
                // Read file content and set it
                QFile f(file);
                if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    editor->setContent(f.readAll());
                    editor->setFilePath(QUrl::fromLocalFile(file));
                    editor->setLanguageFromFilePath();
                    editor->markClean();
                }
            }
        }
    }
}

void MainWindow::onActionOpenFolder()
{
    const QString dir = QFileDialog::getExistingDirectory(
        this, tr("Open Folder"), m_workingDirectory);

    if (dir.isEmpty()) return;

    // Show the file tree dock with the selected directory
    if (m_fileTreeDock) {
        m_fileTreeDock->setRootPath(dir);
        m_fileTreeDock->show();
    }
}

void MainWindow::onActionSave()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    save(tw, tw->currentIndex());
}

void MainWindow::onActionSaveAs()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    saveAs(tw, tw->currentIndex(), false);
}

void MainWindow::onActionSaveCopyAs()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    saveAs(tw, tw->currentIndex(), true);
}

void MainWindow::onActionSaveAll()
{
    m_topEditorContainer->forEachEditor([this](int twIdx, int tabIdx,
                                               EditorTabWidget *tw,
                                               std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor) {
        if (editor && !editor->isClean()) {
            save(tw, tabIdx);
        }
        return true; // continue iteration
    });
}

void MainWindow::onActionClose()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    closeTab(tw, tw->currentIndex());
}

void MainWindow::onActionCloseAll()
{
    m_topEditorContainer->forEachEditor(true, [this](int, int tabIdx,
                                                     EditorTabWidget *tw,
                                                     std::shared_ptr<NovaPad::Editor::CodeEditorWidget>) {
        closeTab(tw, tabIdx);
        return true;
    });
}

void MainWindow::onActionCloseAllButCurrent()
{
    auto *currentTw = m_topEditorContainer->currentTabWidget();
    if (!currentTw) return;
    int currentIdx = currentTw->currentIndex();

    for (int i = currentTw->count() - 1; i >= 0; --i) {
        if (i != currentIdx) {
            closeTab(currentTw, i);
        }
    }
}

void MainWindow::onActionCloseLeft()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    int current = tw->currentIndex();
    for (int i = current - 1; i >= 0; --i) {
        closeTab(tw, i);
    }
}

void MainWindow::onActionCloseRight()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    for (int i = tw->count() - 1; i > tw->currentIndex(); --i) {
        closeTab(tw, i);
    }
}

void MainWindow::onActionReloadFromDisk()
{
    // Reload current file from disk
    auto editor = currentEditor();
    if (!editor || !editor->filePath().isValid()) return;

    QFile f(editor->filePath().toLocalFile());
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        editor->setContent(f.readAll());
        editor->markClean();
    }
}

void MainWindow::onActionRename()
{
    auto editor = currentEditor();
    if (!editor || !editor->filePath().isValid()) return;

    bool ok = false;
    QString newName = QInputDialog::getText(this, tr("Rename"),
                                            tr("New file name:"),
                                            QLineEdit::Normal,
                                            Core::Globals::fileNameFromUrl(editor->filePath()),
                                            &ok);
    if (!ok || newName.isEmpty()) return;

    // Rename the file on disk and update the editor
    const QString oldPath = editor->filePath().toLocalFile();
    const QString newPath = QFileInfo(oldPath).absolutePath() + "/" + newName;
    if (QFile::rename(oldPath, newPath)) {
        editor->setFilePath(QUrl::fromLocalFile(newPath));
    }
}

void MainWindow::onActionPrint()
{
    auto editor = currentEditor();
    if (!editor) return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested,
            this, [&editor](QPrinter *p) { editor->print(p); });
    preview.exec();
}

void MainWindow::onActionPrintNow()
{
    auto editor = currentEditor();
    if (!editor) return;

    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dlg(&printer, this);
    if (dlg.exec() == QDialog::Accepted) {
        editor->print(&printer);
    }
}

void MainWindow::onActionExit()
{
    close();
}

// ---------------------------------------------------------------------------
// Edit actions implementation
// ---------------------------------------------------------------------------

void MainWindow::onActionUndo()   { auto e = currentEditor(); if (e) e->undo(); }
void MainWindow::onActionRedo()   { auto e = currentEditor(); if (e) e->redo(); }
void MainWindow::onActionCut()    { auto e = currentEditor(); if (e) e->cut(); }
void MainWindow::onActionCopy()   { auto e = currentEditor(); if (e) e->copy(); }
void MainWindow::onActionPaste()  { auto e = currentEditor(); if (e) e->paste(); }
void MainWindow::onActionDelete() {
    auto e = currentEditor();
    if (e) { QTextCursor tc = e->textCursor(); tc.removeSelectedText(); }
}
void MainWindow::onActionSelectAll() { auto e = currentEditor(); if (e) e->selectAll(); }

void MainWindow::onActionBeginEndSelect()
{
    auto e = currentEditor();
    if (!e) return;

    if (!m_beginSelectPosSet) {
        auto pos = e->cursorPos();
        m_beginSelectPos = { pos.line, pos.column };
        m_beginSelectPosSet = true;
    } else {
        auto pos = e->cursorPos();
        e->setSelection(m_beginSelectPos.first, m_beginSelectPos.second,
                        pos.line, pos.column);
        m_beginSelectPosSet = false;
    }
}

void MainWindow::onActionUpperCase()
{
    transformSelectedText([](const QString &s) { return s.toUpper(); });
}

void MainWindow::onActionLowerCase()
{
    transformSelectedText([](const QString &s) { return s.toLower(); });
}

void MainWindow::onActionDeleteLine()
{
    auto e = currentEditor();
    if (!e) return;
    QTextCursor tc = e->textCursor();
    tc.select(QTextCursor::LineUnderCursor);
    tc.removeSelectedText();
    tc.deleteChar(); // remove the newline
    e->setTextCursor(tc);
}

void MainWindow::onActionDuplicateLine()
{
    auto e = currentEditor();
    if (!e) return;
    QTextCursor tc = e->textCursor();
    tc.movePosition(QTextCursor::StartOfLine);
    tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    const QString line = tc.selectedText();
    tc.movePosition(QTextCursor::EndOfLine);
    tc.insertText("\n" + line);
    e->setTextCursor(tc);
}

void MainWindow::onActionMoveLineUp()
{
    auto e = currentEditor();
    if (!e) return;
    QTextCursor tc = e->textCursor();
    int line = tc.blockNumber();
    if (line == 0) return;

    tc.beginEditBlock();
    tc.movePosition(QTextCursor::StartOfLine);
    tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    QString text = tc.selectedText();
    tc.removeSelectedText();
    tc.deletePreviousChar(); // remove newline
    tc.movePosition(QTextCursor::StartOfLine);
    tc.insertText(text + "\n");
    tc.movePosition(QTextCursor::Up);
    tc.endEditBlock();
    e->setTextCursor(tc);
}

void MainWindow::onActionMoveLineDown()
{
    auto e = currentEditor();
    if (!e) return;
    QTextCursor tc = e->textCursor();

    tc.beginEditBlock();
    tc.movePosition(QTextCursor::StartOfLine);
    tc.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    QString text = tc.selectedText();
    tc.removeSelectedText();
    tc.deleteChar(); // remove newline
    tc.movePosition(QTextCursor::EndOfLine);
    tc.insertText("\n" + text);
    tc.endEditBlock();
    e->setTextCursor(tc);
}

void MainWindow::onActionTrimTrailingSpace()
{
    transformSelectedText([](const QString &s) {
        QStringList lines = s.split('\n');
        for (auto &line : lines) {
            while (line.endsWith(' ') || line.endsWith('\t'))
                line.chop(1);
        }
        return lines.join('\n');
    });
}

void MainWindow::onActionTrimLeadingSpace()
{
    transformSelectedText([](const QString &s) {
        QStringList lines = s.split('\n');
        for (auto &line : lines) {
            while (line.startsWith(' ') || line.startsWith('\t'))
                line.remove(0, 1);
        }
        return lines.join('\n');
    });
}

void MainWindow::onActionTrimBothSpace() { onActionTrimLeadingSpace(); onActionTrimTrailingSpace(); }
void MainWindow::onActionEolToSpace()    { transformSelectedText([](const QString &s) { QString r = s; return r.replace('\n', ' '); }); }
void MainWindow::onActionTabToSpace()    { transformSelectedText([](const QString &s) { QString r = s; return r.replace('\t', "    "); }); }
void MainWindow::onActionSpaceToTabAll() { transformSelectedText([](const QString &s) { QString r = s; return r.replace("    ", "\t"); }); }

void MainWindow::onActionSpaceToTabLeading()
{
    transformSelectedText([](const QString &s) {
        QStringList lines = s.split('\n');
        for (auto &line : lines) {
            int spaces = 0;
            for (const QChar &ch : std::as_const(line)) {
                if (ch == ' ') ++spaces; else break;
            }
            int tabs = spaces / 4;
            int rem  = spaces % 4;
            line = QString(tabs, '\t') + QString(rem, ' ') + line.mid(spaces);
        }
        return lines.join('\n');
    });
}

// ---------------------------------------------------------------------------
// Search actions
// ---------------------------------------------------------------------------

void MainWindow::onActionSearch()
{
    if (!m_searchReplaceDialog) {
        m_searchReplaceDialog = new NovaPad::Search::SearchReplaceDialog(m_topEditorContainer, this);
    }
    m_searchReplaceDialog->show(NovaPad::Search::SearchReplaceDialog::TabSearch);
}

void MainWindow::onActionReplace()
{
    if (!m_searchReplaceDialog) {
        m_searchReplaceDialog = new NovaPad::Search::SearchReplaceDialog(m_topEditorContainer, this);
    }
    m_searchReplaceDialog->show(NovaPad::Search::SearchReplaceDialog::TabReplace);
}

void MainWindow::onActionFindNext()
{
    if (m_searchReplaceDialog) m_searchReplaceDialog->findFromUI(true);
}

void MainWindow::onActionFindPrevious()
{
    if (m_searchReplaceDialog) m_searchReplaceDialog->findFromUI(false);
}

void MainWindow::onActionFindInFiles()
{
    if (m_advSearchDock) m_advSearchDock->show(true);
}

void MainWindow::onActionGoToLine()
{
    auto editor = currentEditor();
    if (!editor) return;

    NovaPad::UI::Dialogs::GoToLineDialog dlg(editor->lineCount(), this);
    if (dlg.exec() == QDialog::Accepted) {
        editor->setCursorPos(dlg.selectedLine() - 1, 0);
    }
}

// ---------------------------------------------------------------------------
// View actions
// ---------------------------------------------------------------------------

void MainWindow::onActionWordWrap(bool on)       { auto e = currentEditor(); if (e) e->setLineWrap(on); }
void MainWindow::onActionFullScreen(bool on)      { on ? showFullScreen() : showNormal(); }
void MainWindow::onActionShowEndOfLine(bool on)   { auto e = currentEditor(); if (e) e->setShowEndOfLine(on); }
void MainWindow::onActionShowAllCharacters(bool on){ auto e = currentEditor(); if (e) { e->setShowWhitespace(on); e->setShowEndOfLine(on); } }
void MainWindow::onActionShowSpaces(bool on)      { auto e = currentEditor(); if (e) e->setShowWhitespace(on); }
void MainWindow::onActionShowTabs(bool on)        { auto e = currentEditor(); if (e) e->setShowTabs(on); }
void MainWindow::onActionShowMenuBar(bool on)     { menuBar()->setVisible(on); }
void MainWindow::onActionShowToolBar(bool on)     { if (m_mainToolBar) m_mainToolBar->setVisible(on); }
void MainWindow::onActionSmartIndent(bool on)     { auto e = currentEditor(); if (e) e->setSmartIndent(on); }
void MainWindow::onActionMathRendering(bool /*on*/) { /* TODO: math rendering support */ }

void MainWindow::onActionZoomIn()
{
    auto e = currentEditor();
    if (e) e->setZoomFactor(e->zoomFactor() + 0.1);
}

void MainWindow::onActionZoomOut()
{
    auto e = currentEditor();
    if (e) e->setZoomFactor(e->zoomFactor() - 0.1);
}

void MainWindow::onActionRestoreZoom()
{
    auto e = currentEditor();
    if (e) e->setZoomFactor(1.0);
}

void MainWindow::onActionToggleFormerTab()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (tw) tw->setCurrentIndex(tw->formerTabIndex());
}

// ---------------------------------------------------------------------------
// Encoding actions (stubs — full implementation requires DocEngine)
// ---------------------------------------------------------------------------

void MainWindow::onActionUtf8()                { /* TODO: set encoding via DocEngine */ }
void MainWindow::onActionUtf8NoBom()           { /* TODO */ }
void MainWindow::onActionUtf16BE()             { /* TODO */ }
void MainWindow::onActionUtf16LE()             { /* TODO */ }
void MainWindow::onActionInterpretAsUtf8()     { /* TODO */ }
void MainWindow::onActionInterpretAsUtf8NoBom(){ /* TODO */ }
void MainWindow::onActionInterpretAsUtf16BE()  { /* TODO */ }
void MainWindow::onActionInterpretAsUtf16LE()  { /* TODO */ }
void MainWindow::onActionConvertEncoding()     { /* TODO */ }
void MainWindow::onActionReloadInterpretedAs() { /* TODO */ }

// ---------------------------------------------------------------------------
// EOL format actions
// ---------------------------------------------------------------------------

void MainWindow::onActionUnixFormat()    { auto e = currentEditor(); if (e) e->setEolSequence("\n"); }
void MainWindow::onActionWindowsFormat() { auto e = currentEditor(); if (e) e->setEolSequence("\r\n"); }
void MainWindow::onActionMacFormat()     { auto e = currentEditor(); if (e) e->setEolSequence("\r"); }

// ---------------------------------------------------------------------------
// Language menu
// ---------------------------------------------------------------------------

void MainWindow::setupLanguagesMenu()
{
    // The language menu is populated dynamically from the LanguageManager
    // catalogue. Each entry sets the current editor's language.
    auto *langMenu = findChild<QMenu *>("menuLanguage");
    if (!langMenu) {
        // Find the Language menu by iterating through menu bar children
        for (auto *menu : menuBar()->findChildren<QMenu *>()) {
            if (menu->title().contains("Language", Qt::CaseInsensitive)) {
                langMenu = menu;
                langMenu->setObjectName("menuLanguage");
                break;
            }
        }
    }
    if (!langMenu) return;

    // Clear any existing language actions (except Plain Text which is at index 0-1)
    QList<QAction *> existingActions = langMenu->actions();
    for (int i = existingActions.size() - 1; i >= 2; --i) {
        langMenu->removeAction(existingActions[i]);
        delete existingActions[i];
    }

    // Add separator after Plain Text if not present
    if (existingActions.size() < 2 || !existingActions[1]->isSeparator()) {
        langMenu->addSeparator();
    }

    // Define the list of fully supported languages (those with LanguageSupport implementations)
    const QStringList supportedLanguageIds = {
        "c", "cpp", "csharp", "rust", "go", "python",
        "javascript", "kotlin", "swift", "php", "perl",
        "html", "css", "json", "xml", "markdown", "sql",
        "plaintext"
    };

    // Populate with only supported languages from LanguageManager
    const auto &languages = NovaPad::Editor::LanguageManager::instance().languages();
    
    // Filter and sort languages by name
    QList<const NovaPad::Editor::Language *> filteredLanguages;
    for (const auto &lang : languages) {
        if (supportedLanguageIds.contains(lang.id)) {
            filteredLanguages.append(&lang);
        }
    }
    std::sort(filteredLanguages.begin(), filteredLanguages.end(),
              [](const NovaPad::Editor::Language *a, const NovaPad::Editor::Language *b) {
                  return a->name.compare(b->name, Qt::CaseInsensitive) < 0;
              });

    // Add language actions (skip plain text as it's already added)
    for (const auto *lang : filteredLanguages) {
        if (lang->id == "plaintext") continue;
        
        auto *action = langMenu->addAction(lang->name);
        connect(action, &QAction::triggered, this, [this, langId = lang->id]() {
            setCurrentEditorLanguage(langId);
        });
    }
}

void MainWindow::setCurrentEditorLanguage(const QString &language)
{
    auto e = currentEditor();
    if (e) e->setLanguage(language);
}

void MainWindow::onActionPlainText()
{
    setCurrentEditorLanguage("plaintext");
}

void MainWindow::onActionIndentDefault() { /* TODO: show default indent dialog */ }
void MainWindow::onActionIndentCustom()  { /* TODO: show custom indent dialog */ }

// ---------------------------------------------------------------------------
// Window actions
// ---------------------------------------------------------------------------

void MainWindow::onActionNewWindow()
{
    auto *win = new MainWindow(QStringList(), nullptr);
    win->show();
}

void MainWindow::onActionOpenInNewWindow()
{
    auto editor = currentEditor();
    if (!editor || !editor->filePath().isValid()) return;

    auto *win = new MainWindow(QStringList(), nullptr);
    win->show();
    // Open the same file in the new window
    win->openCommandLineUrls(m_workingDirectory, { editor->filePath().toLocalFile() });
}

void MainWindow::onActionMoveToNewWindow()
{
    // TODO: transfer the current tab to a new window
    onActionOpenInNewWindow();
}

void MainWindow::onActionMoveToOtherView()
{
    auto *tw = m_topEditorContainer->currentTabWidget();
    if (!tw) return;
    auto *otherTw = m_topEditorContainer->inactiveTabWidget(true);
    if (otherTw) {
        otherTw->transferEditorTab(true, tw, tw->currentIndex());
    }
}

// ---------------------------------------------------------------------------
// Session actions
// ---------------------------------------------------------------------------

void MainWindow::onActionLoadSession()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Load Session"), {}, tr("NovaPad Session (*.nps);;All Files (*)"));
    if (!path.isEmpty()) {
        NovaPad::Sessions::SessionManager::loadSession(m_docEngine, m_topEditorContainer, path);
    }
}

void MainWindow::onActionSaveSession()
{
    const QString path = QFileDialog::getSaveFileName(
        this, tr("Save Session"), {}, tr("NovaPad Session (*.nps);;All Files (*)"));
    if (!path.isEmpty()) {
        NovaPad::Sessions::SessionManager::saveSession(m_docEngine, m_topEditorContainer, path);
    }
}

// ---------------------------------------------------------------------------
// Extension actions
// ---------------------------------------------------------------------------

void MainWindow::onActionInstallExtension() { /* TODO */ }

QAction *MainWindow::addExtensionMenuItem(const QString & /*extensionId*/, const QString &text)
{
    auto *extMenu = menuBar()->findChild<QMenu *>("menuExtensions");
    if (!extMenu) return nullptr;
    return extMenu->addAction(text);
}

void MainWindow::showExtensionsMenu(bool show)
{
    auto *extMenu = menuBar()->findChild<QMenu *>("menuExtensions");
    if (extMenu) extMenu->menuAction()->setVisible(show);
}

// ---------------------------------------------------------------------------
// Clipboard path actions
// ---------------------------------------------------------------------------

void MainWindow::onActionCopyFullPath()
{
    auto e = currentEditor();
    if (e && e->filePath().isValid()) {
        QApplication::clipboard()->setText(e->filePath().toLocalFile());
    }
}

void MainWindow::onActionCopyFileName()
{
    auto e = currentEditor();
    if (e && e->filePath().isValid()) {
        QApplication::clipboard()->setText(Core::Globals::fileNameFromUrl(e->filePath()));
    }
}

void MainWindow::onActionCopyDirectory()
{
    auto e = currentEditor();
    if (e && e->filePath().isValid()) {
        QApplication::clipboard()->setText(QFileInfo(e->filePath().toLocalFile()).absolutePath());
    }
}

// ---------------------------------------------------------------------------
// Recent files
// ---------------------------------------------------------------------------

void MainWindow::onActionEmptyRecentFiles()
{
    m_settings.general().setRecentDocuments({});
    updateRecentDocsMenu();
}

void MainWindow::onActionOpenAllRecent() { /* TODO */ }
void MainWindow::updateRecentDocsMenu()  { /* TODO: rebuild recent-files submenu */ }

// ---------------------------------------------------------------------------
// About dialogs
// ---------------------------------------------------------------------------

void MainWindow::onActionAboutNovaPad()
{
    NovaPad::UI::Dialogs::AboutDialog dlg(this);
    dlg.exec();
}

void MainWindow::onActionAboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

// ---------------------------------------------------------------------------
// Run menu
// ---------------------------------------------------------------------------

void MainWindow::onRunCommand()
{
    NovaPad::UI::Dialogs::RunCommandDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        // Execute the command and show output in the output dock
    }
}

void MainWindow::onModifyRunCommands() { /* TODO */ }

void MainWindow::generateRunMenu()
{
    // Rebuild the Run menu from stored commands in settings
}

// ---------------------------------------------------------------------------
// Internal signals from child widgets
// ---------------------------------------------------------------------------

void MainWindow::onTabCloseRequested(EditorTabWidget *tabWidget, int tab)
{
    closeTab(tabWidget, tab);
}

void MainWindow::onCurrentEditorChanged(EditorTabWidget * /*tabWidget*/, int /*tab*/)
{
    auto editor = currentEditor();
    if (editor) {
        refreshEditorUiInfo(editor);
    }
}

void MainWindow::onEditorAdded(EditorTabWidget * /*tabWidget*/, int /*tab*/) { }
void MainWindow::onTabContextMenu(QPoint /*point*/, EditorTabWidget * /*tabWidget*/, int /*tab*/) { }
void MainWindow::onTabBarDoubleClicked(EditorTabWidget * /*tabWidget*/, int /*tab*/) { onActionNew(); }

void MainWindow::onEditorMouseWheel(EditorTabWidget * /*tabWidget*/, int /*tab*/, QWheelEvent * /*ev*/)
{
    // Zoom is handled inside CodeEditorWidget::wheelEvent
}

void MainWindow::onEditorUrlsDropped(const QList<QUrl> &urls)
{
    for (const QUrl &url : urls) {
        if (url.isLocalFile()) {
            // Open each dropped file
            auto *tw = m_topEditorContainer->currentTabWidget();
            if (tw) {
                int idx = tw->addEditorTab(true, QFileInfo(url.toLocalFile()).fileName());
                auto editor = tw->editor(idx);
                if (editor) {
                    QFile f(url.toLocalFile());
                    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        editor->setContent(f.readAll());
                        editor->setFilePath(url);
                        editor->setLanguageFromFilePath();
                        editor->markClean();
                    }
                }
            }
        }
    }
}

void MainWindow::onFileOnDiskChanged(EditorTabWidget * /*tabWidget*/, int /*tab*/, bool /*removed*/) { }
void MainWindow::onDocumentSaved(EditorTabWidget * /*tabWidget*/, int /*tab*/) { }
void MainWindow::onDocumentReloaded(EditorTabWidget * /*tabWidget*/, int /*tab*/) { }
void MainWindow::onDocumentLoaded(EditorTabWidget * /*tabWidget*/, int /*tab*/, bool, bool) { }
void MainWindow::onBannerRemoved(QWidget * /*banner*/) { }
void MainWindow::onSearchDockItemInteracted() { }

// ---------------------------------------------------------------------------
// Status bar refresh
// ---------------------------------------------------------------------------

void MainWindow::refreshEditorUiInfo(std::shared_ptr<NovaPad::Editor::CodeEditorWidget> editor)
{
    if (!editor) return;

    auto pos = editor->cursorPos();
    m_sbDocInfoLabel->setText(tr("Ln %1, Col %2").arg(pos.line + 1).arg(pos.column + 1));

    // Encoding label
    if (editor->codec()) {
        m_sbFileFormatBtn->setText(QString::fromLatin1(editor->codec()->name()));
    }

    // EOL label
    const QString &eol = editor->eolSequence();
    if (eol == "\r\n")     m_sbEolFormatBtn->setText("Windows (CRLF)");
    else if (eol == "\r")  m_sbEolFormatBtn->setText("Mac (CR)");
    else                   m_sbEolFormatBtn->setText("Unix (LF)");

    // Language label
    const auto *lang = editor->currentLanguage();
    m_sbTextFormatBtn->setText(lang ? lang->name : tr("Plain Text"));
}

void MainWindow::refreshCursorInfo()
{
    refreshEditorUiInfo(currentEditor());
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

void MainWindow::openCommandLineUrls(const QString &workingDir, const QStringList &args)
{
    auto parser = Core::CommandLineParser::create(args);
    const QStringList positional = parser->positionalArguments();

    for (const QString &arg : positional) {
        QUrl url = stringToUrl(arg, workingDir);
        if (url.isValid() && url.isLocalFile()) {
            auto *tw = m_topEditorContainer->currentTabWidget();
            if (tw) {
                int idx = tw->addEditorTab(true, QFileInfo(url.toLocalFile()).fileName());
                auto editor = tw->editor(idx);
                if (editor) {
                    QFile f(url.toLocalFile());
                    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        editor->setContent(f.readAll());
                        editor->setFilePath(url);
                        editor->setLanguageFromFilePath();
                        editor->markClean();
                    }
                }
            }
        }
    }
}

QUrl MainWindow::stringToUrl(const QString &fileName, const QString &workingDir)
{
    QFileInfo fi(fileName);
    if (fi.isRelative() && !workingDir.isEmpty()) {
        fi.setFile(QDir(workingDir), fileName);
    }
    return QUrl::fromLocalFile(fi.absoluteFilePath());
}

int MainWindow::save(EditorTabWidget *tabWidget, int tab)
{
    auto editor = tabWidget->editor(tab);
    if (!editor) return TabClosed_Canceled;

    if (!editor->filePath().isValid()) {
        return saveAs(tabWidget, tab, false);
    }

    QFile file(editor->filePath().toLocalFile());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(editor->content().toUtf8());
        editor->markClean();
        tabWidget->setSavedIcon(tab, true);
        return TabClosed_Saved;
    }
    return TabClosed_Canceled;
}

int MainWindow::saveAs(EditorTabWidget *tabWidget, int tab, bool copy)
{
    auto editor = tabWidget->editor(tab);
    if (!editor) return TabClosed_Canceled;

    QString defaultPath = editor->filePath().isValid()
                              ? editor->filePath().toLocalFile()
                              : m_workingDirectory + "/" + tabWidget->tabText(tab);

    const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), defaultPath);
    if (path.isEmpty()) return TabClosed_Canceled;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(editor->content().toUtf8());
        if (!copy) {
            editor->setFilePath(QUrl::fromLocalFile(path));
            editor->markClean();
            tabWidget->setTabText(tab, QFileInfo(path).fileName());
            tabWidget->setSavedIcon(tab, true);
        }
        return TabClosed_Saved;
    }
    return TabClosed_Canceled;
}

QUrl MainWindow::getSaveDialogDefaultFileName(EditorTabWidget *tabWidget, int tab)
{
    auto editor = tabWidget->editor(tab);
    if (editor && editor->filePath().isValid()) return editor->filePath();
    return QUrl::fromLocalFile(m_workingDirectory);
}

int MainWindow::closeTab(EditorTabWidget *tabWidget, int tab, bool remove, bool force)
{
    auto editor = tabWidget->editor(tab);
    if (!editor) return TabClosed_Canceled;

    if (!force && !editor->isClean()) {
        int result = askIfWantToSave(tabWidget, tab, SaveReason_TabClosing);
        if (result == TabClosed_Canceled) return TabClosed_Canceled;
    }

    if (remove) {
        tabWidget->removeTab(tab);
    }

    // Ensure at least one tab exists
    if (tabWidget->count() == 0) {
        static int counter = 1;
        tabWidget->addEditorTab(true, tr("New %1").arg(counter++));
    }

    m_topEditorContainer->removeTabWidgetIfEmpty(tabWidget);
    return TabClosed_Saved;
}

int MainWindow::askIfWantToSave(EditorTabWidget *tabWidget, int tab, int /*reason*/)
{
    auto editor = tabWidget->editor(tab);
    if (!editor) return TabClosed_Canceled;

    const QString name = tabWidget->tabText(tab);
    const int ret = QMessageBox::question(
        this, tr("Save Changes"),
        tr("Do you want to save changes to \"%1\"?").arg(name),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    switch (ret) {
    case QMessageBox::Save:    return save(tabWidget, tab);
    case QMessageBox::Discard: return TabClosed_NotSaved;
    default:                   return TabClosed_Canceled;
    }
}

bool MainWindow::saveTabsToCache()  { return true; /* TODO */ }
bool MainWindow::finalizeAllTabs()  { return true; /* TODO */ }

void MainWindow::transformSelectedText(std::function<QString(const QString &)> func)
{
    auto e = currentEditor();
    if (!e) return;

    QTextCursor tc = e->textCursor();
    if (!tc.hasSelection()) {
        // Select the entire document if nothing is selected
        tc.select(QTextCursor::Document);
    }
    const QString original = tc.selectedText();
    tc.insertText(func(original));
}

void MainWindow::checkIndentationMode(std::shared_ptr<NovaPad::Editor::CodeEditorWidget> /*editor*/) { }
void MainWindow::openRecentFileEntry(const QUrl & /*url*/) { }
void MainWindow::toggleOverwrite() { m_overwrite = !m_overwrite; m_sbOvertypeBtn->setText(m_overwrite ? tr("OVR") : tr("INS")); }
bool MainWindow::updateSymbols(bool /*on*/) { return false; }
void MainWindow::restoreWindowSettings()  { /* TODO: restore geometry from settings */ }
void MainWindow::loadIcons()
{
    // Install custom icon theme for bundled SVG icons
    NovaPad::UI::IconProvider::installTheme();
    
    // Listen for theme changes to refresh icons
    connect(&NovaPad::Themes::ThemeManager::instance(), &NovaPad::Themes::ThemeManager::themeChanged,
            this, &MainWindow::onThemeChanged);
}

void MainWindow::onThemeChanged(const QString &themeId)
{
    // Force toolbar to reload with new theme colors
    loadToolBar();
    
    // Update any other icon-dependent UI elements
    update();
}
void MainWindow::configureUserInterface() { /* TODO: apply settings to UI */ }
void MainWindow::fixKeyboardShortcuts()   { /* TODO: workaround for broken shortcuts */ }
void MainWindow::removeTabWidgetIfEmpty(EditorTabWidget *tw) { m_topEditorContainer->removeTabWidgetIfEmpty(tw); }

// ---------------------------------------------------------------------------
// Event overrides
// ---------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (finalizeAllTabs()) {
        saveTabsToCache();
        event->accept();
        emit windowClosed();
    } else {
        event->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        onEditorUrlsDropped(event->mimeData()->urls());
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Insert/Overwrite toggle
    if (event->key() == Qt::Key_Insert && event->modifiers() == Qt::NoModifier) {
        toggleOverwrite();
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange && isActiveWindow()) {
        // Move this window to the end of the instance list (most recently active)
        s_instances.removeAll(this);
        s_instances.append(this);
    }
    QMainWindow::changeEvent(event);
}

} // namespace NovaPad::UI
