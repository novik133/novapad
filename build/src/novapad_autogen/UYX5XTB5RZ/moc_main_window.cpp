/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ui/main_window.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto NovaPad::UI::MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NovaPad::UI::MainWindow",
        "windowClosed",
        "",
        "refreshEditorUiInfo",
        "std::shared_ptr<NovaPad::Editor::CodeEditorWidget>",
        "editor",
        "refreshCursorInfo",
        "onActionNew",
        "onActionOpen",
        "onActionOpenFolder",
        "onActionSave",
        "onActionSaveAs",
        "onActionSaveCopyAs",
        "onActionSaveAll",
        "onActionClose",
        "onActionCloseAll",
        "onActionCloseAllButCurrent",
        "onActionCloseLeft",
        "onActionCloseRight",
        "onActionReloadFromDisk",
        "onActionRename",
        "onActionPrint",
        "onActionPrintNow",
        "onActionExit",
        "onActionUndo",
        "onActionRedo",
        "onActionCut",
        "onActionCopy",
        "onActionPaste",
        "onActionDelete",
        "onActionSelectAll",
        "onActionBeginEndSelect",
        "onActionUpperCase",
        "onActionLowerCase",
        "onActionDeleteLine",
        "onActionDuplicateLine",
        "onActionMoveLineUp",
        "onActionMoveLineDown",
        "onActionTrimTrailingSpace",
        "onActionTrimLeadingSpace",
        "onActionTrimBothSpace",
        "onActionEolToSpace",
        "onActionTabToSpace",
        "onActionSpaceToTabAll",
        "onActionSpaceToTabLeading",
        "onActionSearch",
        "onActionReplace",
        "onActionFindNext",
        "onActionFindPrevious",
        "onActionFindInFiles",
        "onActionGoToLine",
        "onActionWordWrap",
        "on",
        "onActionFullScreen",
        "onActionShowEndOfLine",
        "onActionShowAllCharacters",
        "onActionShowSpaces",
        "onActionShowTabs",
        "onActionShowMenuBar",
        "onActionShowToolBar",
        "onActionZoomIn",
        "onActionZoomOut",
        "onActionRestoreZoom",
        "onActionSmartIndent",
        "onActionMathRendering",
        "onActionToggleFormerTab",
        "onActionUtf8",
        "onActionUtf8NoBom",
        "onActionUtf16BE",
        "onActionUtf16LE",
        "onActionInterpretAsUtf8",
        "onActionInterpretAsUtf8NoBom",
        "onActionInterpretAsUtf16BE",
        "onActionInterpretAsUtf16LE",
        "onActionConvertEncoding",
        "onActionReloadInterpretedAs",
        "onActionUnixFormat",
        "onActionWindowsFormat",
        "onActionMacFormat",
        "setCurrentEditorLanguage",
        "language",
        "onActionPlainText",
        "onActionIndentDefault",
        "onActionIndentCustom",
        "onThemeChanged",
        "themeId",
        "onActionNewWindow",
        "onActionOpenInNewWindow",
        "onActionMoveToNewWindow",
        "onActionMoveToOtherView",
        "onActionLoadSession",
        "onActionSaveSession",
        "onActionInstallExtension",
        "onActionCopyFullPath",
        "onActionCopyFileName",
        "onActionCopyDirectory",
        "onActionEmptyRecentFiles",
        "onActionOpenAllRecent",
        "onActionAboutNovaPad",
        "onActionAboutQt",
        "onRunCommand",
        "onModifyRunCommands",
        "onTabCloseRequested",
        "EditorTabWidget*",
        "tabWidget",
        "tab",
        "onCurrentEditorChanged",
        "onEditorAdded",
        "onTabContextMenu",
        "QPoint",
        "point",
        "onTabBarDoubleClicked",
        "onEditorMouseWheel",
        "QWheelEvent*",
        "ev",
        "onEditorUrlsDropped",
        "QList<QUrl>",
        "urls",
        "onFileOnDiskChanged",
        "removed",
        "onDocumentSaved",
        "onDocumentReloaded",
        "onDocumentLoaded",
        "wasAlreadyOpened",
        "updateRecentDocs",
        "onBannerRemoved",
        "QWidget*",
        "banner",
        "onSearchDockItemInteracted"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'windowClosed'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshEditorUiInfo'
        QtMocHelpers::SlotData<void(std::shared_ptr<NovaPad::Editor::CodeEditorWidget>)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Slot 'refreshCursorInfo'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onActionNew'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionOpen'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionOpenFolder'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSave'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSaveAs'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSaveCopyAs'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSaveAll'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionClose'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCloseAll'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCloseAllButCurrent'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCloseLeft'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCloseRight'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionReloadFromDisk'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionRename'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionPrint'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionPrintNow'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionExit'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUndo'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionRedo'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCut'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCopy'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionPaste'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionDelete'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSelectAll'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionBeginEndSelect'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUpperCase'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionLowerCase'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionDeleteLine'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionDuplicateLine'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionMoveLineUp'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionMoveLineDown'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionTrimTrailingSpace'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionTrimLeadingSpace'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionTrimBothSpace'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionEolToSpace'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionTabToSpace'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSpaceToTabAll'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSpaceToTabLeading'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSearch'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionReplace'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionFindNext'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionFindPrevious'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionFindInFiles'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionGoToLine'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionWordWrap'
        QtMocHelpers::SlotData<void(bool)>(51, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionFullScreen'
        QtMocHelpers::SlotData<void(bool)>(53, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionShowEndOfLine'
        QtMocHelpers::SlotData<void(bool)>(54, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionShowAllCharacters'
        QtMocHelpers::SlotData<void(bool)>(55, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionShowSpaces'
        QtMocHelpers::SlotData<void(bool)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionShowTabs'
        QtMocHelpers::SlotData<void(bool)>(57, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionShowMenuBar'
        QtMocHelpers::SlotData<void(bool)>(58, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionShowToolBar'
        QtMocHelpers::SlotData<void(bool)>(59, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionZoomIn'
        QtMocHelpers::SlotData<void()>(60, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionZoomOut'
        QtMocHelpers::SlotData<void()>(61, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionRestoreZoom'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSmartIndent'
        QtMocHelpers::SlotData<void(bool)>(63, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionMathRendering'
        QtMocHelpers::SlotData<void(bool)>(64, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 52 },
        }}),
        // Slot 'onActionToggleFormerTab'
        QtMocHelpers::SlotData<void()>(65, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUtf8'
        QtMocHelpers::SlotData<void()>(66, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUtf8NoBom'
        QtMocHelpers::SlotData<void()>(67, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUtf16BE'
        QtMocHelpers::SlotData<void()>(68, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUtf16LE'
        QtMocHelpers::SlotData<void()>(69, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionInterpretAsUtf8'
        QtMocHelpers::SlotData<void()>(70, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionInterpretAsUtf8NoBom'
        QtMocHelpers::SlotData<void()>(71, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionInterpretAsUtf16BE'
        QtMocHelpers::SlotData<void()>(72, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionInterpretAsUtf16LE'
        QtMocHelpers::SlotData<void()>(73, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionConvertEncoding'
        QtMocHelpers::SlotData<void()>(74, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionReloadInterpretedAs'
        QtMocHelpers::SlotData<void()>(75, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionUnixFormat'
        QtMocHelpers::SlotData<void()>(76, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionWindowsFormat'
        QtMocHelpers::SlotData<void()>(77, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionMacFormat'
        QtMocHelpers::SlotData<void()>(78, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setCurrentEditorLanguage'
        QtMocHelpers::SlotData<void(const QString &)>(79, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 80 },
        }}),
        // Slot 'onActionPlainText'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionIndentDefault'
        QtMocHelpers::SlotData<void()>(82, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionIndentCustom'
        QtMocHelpers::SlotData<void()>(83, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onThemeChanged'
        QtMocHelpers::SlotData<void(const QString &)>(84, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 85 },
        }}),
        // Slot 'onActionNewWindow'
        QtMocHelpers::SlotData<void()>(86, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionOpenInNewWindow'
        QtMocHelpers::SlotData<void()>(87, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionMoveToNewWindow'
        QtMocHelpers::SlotData<void()>(88, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionMoveToOtherView'
        QtMocHelpers::SlotData<void()>(89, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionLoadSession'
        QtMocHelpers::SlotData<void()>(90, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionSaveSession'
        QtMocHelpers::SlotData<void()>(91, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionInstallExtension'
        QtMocHelpers::SlotData<void()>(92, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCopyFullPath'
        QtMocHelpers::SlotData<void()>(93, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCopyFileName'
        QtMocHelpers::SlotData<void()>(94, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionCopyDirectory'
        QtMocHelpers::SlotData<void()>(95, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionEmptyRecentFiles'
        QtMocHelpers::SlotData<void()>(96, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionOpenAllRecent'
        QtMocHelpers::SlotData<void()>(97, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionAboutNovaPad'
        QtMocHelpers::SlotData<void()>(98, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActionAboutQt'
        QtMocHelpers::SlotData<void()>(99, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRunCommand'
        QtMocHelpers::SlotData<void()>(100, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onModifyRunCommands'
        QtMocHelpers::SlotData<void()>(101, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTabCloseRequested'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int)>(102, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onCurrentEditorChanged'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int)>(106, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onEditorAdded'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int)>(107, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onTabContextMenu'
        QtMocHelpers::SlotData<void(QPoint, EditorTabWidget *, int)>(108, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 109, 110 }, { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onTabBarDoubleClicked'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int)>(111, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onEditorMouseWheel'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int, QWheelEvent *)>(112, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 }, { 0x80000000 | 113, 114 },
        }}),
        // Slot 'onEditorUrlsDropped'
        QtMocHelpers::SlotData<void(const QList<QUrl> &)>(115, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 116, 117 },
        }}),
        // Slot 'onFileOnDiskChanged'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int, bool)>(118, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 }, { QMetaType::Bool, 119 },
        }}),
        // Slot 'onDocumentSaved'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int)>(120, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onDocumentReloaded'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int)>(121, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onDocumentLoaded'
        QtMocHelpers::SlotData<void(EditorTabWidget *, int, bool, bool)>(122, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 103, 104 }, { QMetaType::Int, 105 }, { QMetaType::Bool, 123 }, { QMetaType::Bool, 124 },
        }}),
        // Slot 'onBannerRemoved'
        QtMocHelpers::SlotData<void(QWidget *)>(125, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 126, 127 },
        }}),
        // Slot 'onSearchDockItemInteracted'
        QtMocHelpers::SlotData<void()>(128, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NovaPad::UI::MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t>.metaTypes,
    nullptr
} };

void NovaPad::UI::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->windowClosed(); break;
        case 1: _t->refreshEditorUiInfo((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<NovaPad::Editor::CodeEditorWidget>>>(_a[1]))); break;
        case 2: _t->refreshCursorInfo(); break;
        case 3: _t->onActionNew(); break;
        case 4: _t->onActionOpen(); break;
        case 5: _t->onActionOpenFolder(); break;
        case 6: _t->onActionSave(); break;
        case 7: _t->onActionSaveAs(); break;
        case 8: _t->onActionSaveCopyAs(); break;
        case 9: _t->onActionSaveAll(); break;
        case 10: _t->onActionClose(); break;
        case 11: _t->onActionCloseAll(); break;
        case 12: _t->onActionCloseAllButCurrent(); break;
        case 13: _t->onActionCloseLeft(); break;
        case 14: _t->onActionCloseRight(); break;
        case 15: _t->onActionReloadFromDisk(); break;
        case 16: _t->onActionRename(); break;
        case 17: _t->onActionPrint(); break;
        case 18: _t->onActionPrintNow(); break;
        case 19: _t->onActionExit(); break;
        case 20: _t->onActionUndo(); break;
        case 21: _t->onActionRedo(); break;
        case 22: _t->onActionCut(); break;
        case 23: _t->onActionCopy(); break;
        case 24: _t->onActionPaste(); break;
        case 25: _t->onActionDelete(); break;
        case 26: _t->onActionSelectAll(); break;
        case 27: _t->onActionBeginEndSelect(); break;
        case 28: _t->onActionUpperCase(); break;
        case 29: _t->onActionLowerCase(); break;
        case 30: _t->onActionDeleteLine(); break;
        case 31: _t->onActionDuplicateLine(); break;
        case 32: _t->onActionMoveLineUp(); break;
        case 33: _t->onActionMoveLineDown(); break;
        case 34: _t->onActionTrimTrailingSpace(); break;
        case 35: _t->onActionTrimLeadingSpace(); break;
        case 36: _t->onActionTrimBothSpace(); break;
        case 37: _t->onActionEolToSpace(); break;
        case 38: _t->onActionTabToSpace(); break;
        case 39: _t->onActionSpaceToTabAll(); break;
        case 40: _t->onActionSpaceToTabLeading(); break;
        case 41: _t->onActionSearch(); break;
        case 42: _t->onActionReplace(); break;
        case 43: _t->onActionFindNext(); break;
        case 44: _t->onActionFindPrevious(); break;
        case 45: _t->onActionFindInFiles(); break;
        case 46: _t->onActionGoToLine(); break;
        case 47: _t->onActionWordWrap((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 48: _t->onActionFullScreen((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 49: _t->onActionShowEndOfLine((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 50: _t->onActionShowAllCharacters((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 51: _t->onActionShowSpaces((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 52: _t->onActionShowTabs((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 53: _t->onActionShowMenuBar((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 54: _t->onActionShowToolBar((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 55: _t->onActionZoomIn(); break;
        case 56: _t->onActionZoomOut(); break;
        case 57: _t->onActionRestoreZoom(); break;
        case 58: _t->onActionSmartIndent((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 59: _t->onActionMathRendering((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 60: _t->onActionToggleFormerTab(); break;
        case 61: _t->onActionUtf8(); break;
        case 62: _t->onActionUtf8NoBom(); break;
        case 63: _t->onActionUtf16BE(); break;
        case 64: _t->onActionUtf16LE(); break;
        case 65: _t->onActionInterpretAsUtf8(); break;
        case 66: _t->onActionInterpretAsUtf8NoBom(); break;
        case 67: _t->onActionInterpretAsUtf16BE(); break;
        case 68: _t->onActionInterpretAsUtf16LE(); break;
        case 69: _t->onActionConvertEncoding(); break;
        case 70: _t->onActionReloadInterpretedAs(); break;
        case 71: _t->onActionUnixFormat(); break;
        case 72: _t->onActionWindowsFormat(); break;
        case 73: _t->onActionMacFormat(); break;
        case 74: _t->setCurrentEditorLanguage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 75: _t->onActionPlainText(); break;
        case 76: _t->onActionIndentDefault(); break;
        case 77: _t->onActionIndentCustom(); break;
        case 78: _t->onThemeChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 79: _t->onActionNewWindow(); break;
        case 80: _t->onActionOpenInNewWindow(); break;
        case 81: _t->onActionMoveToNewWindow(); break;
        case 82: _t->onActionMoveToOtherView(); break;
        case 83: _t->onActionLoadSession(); break;
        case 84: _t->onActionSaveSession(); break;
        case 85: _t->onActionInstallExtension(); break;
        case 86: _t->onActionCopyFullPath(); break;
        case 87: _t->onActionCopyFileName(); break;
        case 88: _t->onActionCopyDirectory(); break;
        case 89: _t->onActionEmptyRecentFiles(); break;
        case 90: _t->onActionOpenAllRecent(); break;
        case 91: _t->onActionAboutNovaPad(); break;
        case 92: _t->onActionAboutQt(); break;
        case 93: _t->onRunCommand(); break;
        case 94: _t->onModifyRunCommands(); break;
        case 95: _t->onTabCloseRequested((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 96: _t->onCurrentEditorChanged((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 97: _t->onEditorAdded((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 98: _t->onTabContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 99: _t->onTabBarDoubleClicked((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 100: _t->onEditorMouseWheel((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QWheelEvent*>>(_a[3]))); break;
        case 101: _t->onEditorUrlsDropped((*reinterpret_cast<std::add_pointer_t<QList<QUrl>>>(_a[1]))); break;
        case 102: _t->onFileOnDiskChanged((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 103: _t->onDocumentSaved((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 104: _t->onDocumentReloaded((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 105: _t->onDocumentLoaded((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4]))); break;
        case 106: _t->onBannerRemoved((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 107: _t->onSearchDockItemInteracted(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 95:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 96:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 97:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 98:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 99:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 100:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 102:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 103:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 104:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 105:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 106:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)()>(_a, &MainWindow::windowClosed, 0))
            return;
    }
}

const QMetaObject *NovaPad::UI::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NovaPad::UI::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad2UI10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int NovaPad::UI::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 108)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 108;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 108)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 108;
    }
    return _id;
}

// SIGNAL 0
void NovaPad::UI::MainWindow::windowClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
