/****************************************************************************
** Meta object code from reading C++ file 'code_editor_widget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/editor/code_editor_widget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'code_editor_widget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto NovaPad::Editor::CodeEditorWidget::qt_create_metaobjectdata<qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NovaPad::Editor::CodeEditorWidget",
        "contentChanged",
        "",
        "cursorActivity",
        "cleanChanged",
        "isClean",
        "fileNameChanged",
        "QUrl",
        "oldName",
        "newName",
        "currentLanguageChanged",
        "id",
        "name",
        "bannerRemoved",
        "QWidget*",
        "banner",
        "mouseWheelUsed",
        "QWheelEvent*",
        "event",
        "urlsDropped",
        "QList<QUrl>",
        "urls"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'contentChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cursorActivity'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cleanChanged'
        QtMocHelpers::SignalData<void(bool)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 5 },
        }}),
        // Signal 'fileNameChanged'
        QtMocHelpers::SignalData<void(const QUrl &, const QUrl &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 }, { 0x80000000 | 7, 9 },
        }}),
        // Signal 'currentLanguageChanged'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Signal 'bannerRemoved'
        QtMocHelpers::SignalData<void(QWidget *)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Signal 'mouseWheelUsed'
        QtMocHelpers::SignalData<void(QWheelEvent *)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Signal 'urlsDropped'
        QtMocHelpers::SignalData<void(const QList<QUrl> &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CodeEditorWidget, qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NovaPad::Editor::CodeEditorWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QPlainTextEdit::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t>.metaTypes,
    nullptr
} };

void NovaPad::Editor::CodeEditorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CodeEditorWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->contentChanged(); break;
        case 1: _t->cursorActivity(); break;
        case 2: _t->cleanChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->fileNameChanged((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[2]))); break;
        case 4: _t->currentLanguageChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->bannerRemoved((*reinterpret_cast<std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 6: _t->mouseWheelUsed((*reinterpret_cast<std::add_pointer_t<QWheelEvent*>>(_a[1]))); break;
        case 7: _t->urlsDropped((*reinterpret_cast<std::add_pointer_t<QList<QUrl>>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)()>(_a, &CodeEditorWidget::contentChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)()>(_a, &CodeEditorWidget::cursorActivity, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)(bool )>(_a, &CodeEditorWidget::cleanChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)(const QUrl & , const QUrl & )>(_a, &CodeEditorWidget::fileNameChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)(const QString & , const QString & )>(_a, &CodeEditorWidget::currentLanguageChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)(QWidget * )>(_a, &CodeEditorWidget::bannerRemoved, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)(QWheelEvent * )>(_a, &CodeEditorWidget::mouseWheelUsed, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (CodeEditorWidget::*)(const QList<QUrl> & )>(_a, &CodeEditorWidget::urlsDropped, 7))
            return;
    }
}

const QMetaObject *NovaPad::Editor::CodeEditorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NovaPad::Editor::CodeEditorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad6Editor16CodeEditorWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int NovaPad::Editor::CodeEditorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void NovaPad::Editor::CodeEditorWidget::contentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NovaPad::Editor::CodeEditorWidget::cursorActivity()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NovaPad::Editor::CodeEditorWidget::cleanChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void NovaPad::Editor::CodeEditorWidget::fileNameChanged(const QUrl & _t1, const QUrl & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void NovaPad::Editor::CodeEditorWidget::currentLanguageChanged(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void NovaPad::Editor::CodeEditorWidget::bannerRemoved(QWidget * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void NovaPad::Editor::CodeEditorWidget::mouseWheelUsed(QWheelEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void NovaPad::Editor::CodeEditorWidget::urlsDropped(const QList<QUrl> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}
QT_WARNING_POP
