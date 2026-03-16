/****************************************************************************
** Meta object code from reading C++ file 'top_editor_container.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ui/top_editor_container.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'top_editor_container.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t {};
} // unnamed namespace

template <> constexpr inline auto NovaPad::UI::TopEditorContainer::qt_create_metaobjectdata<qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NovaPad::UI::TopEditorContainer",
        "currentTabChanged",
        "",
        "EditorTabWidget*",
        "tabWidget",
        "tab",
        "currentTabWidgetChanged",
        "currentEditorChanged",
        "customTabContextMenuRequested",
        "QPoint",
        "point",
        "tabCloseRequested",
        "editorAdded",
        "editorMouseWheel",
        "QWheelEvent*",
        "ev",
        "tabBarDoubleClicked",
        "onCurrentTabChanged",
        "index",
        "onCurrentTabWidgetChanged",
        "onCustomContextMenuRequested",
        "onTabCloseRequested",
        "onEditorAdded"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentTabChanged'
        QtMocHelpers::SignalData<void(EditorTabWidget *, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'currentTabWidgetChanged'
        QtMocHelpers::SignalData<void(EditorTabWidget *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'currentEditorChanged'
        QtMocHelpers::SignalData<void(EditorTabWidget *, int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'customTabContextMenuRequested'
        QtMocHelpers::SignalData<void(QPoint, EditorTabWidget *, int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 }, { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'tabCloseRequested'
        QtMocHelpers::SignalData<void(EditorTabWidget *, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'editorAdded'
        QtMocHelpers::SignalData<void(EditorTabWidget *, int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'editorMouseWheel'
        QtMocHelpers::SignalData<void(EditorTabWidget *, int, QWheelEvent *)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 }, { 0x80000000 | 14, 15 },
        }}),
        // Signal 'tabBarDoubleClicked'
        QtMocHelpers::SignalData<void(EditorTabWidget *, int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Slot 'onCurrentTabChanged'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'onCurrentTabWidgetChanged'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCustomContextMenuRequested'
        QtMocHelpers::SlotData<void(QPoint)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'onTabCloseRequested'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'onEditorAdded'
        QtMocHelpers::SlotData<void(int)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TopEditorContainer, qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NovaPad::UI::TopEditorContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<QSplitter::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t>.metaTypes,
    nullptr
} };

void NovaPad::UI::TopEditorContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TopEditorContainer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentTabChanged((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->currentTabWidgetChanged((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1]))); break;
        case 2: _t->currentEditorChanged((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->customTabContextMenuRequested((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 4: _t->tabCloseRequested((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->editorAdded((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->editorMouseWheel((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QWheelEvent*>>(_a[3]))); break;
        case 7: _t->tabBarDoubleClicked((*reinterpret_cast<std::add_pointer_t<EditorTabWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->onCurrentTabChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->onCurrentTabWidgetChanged(); break;
        case 10: _t->onCustomContextMenuRequested((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 11: _t->onTabCloseRequested((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->onEditorAdded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< EditorTabWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * , int )>(_a, &TopEditorContainer::currentTabChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * )>(_a, &TopEditorContainer::currentTabWidgetChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * , int )>(_a, &TopEditorContainer::currentEditorChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(QPoint , EditorTabWidget * , int )>(_a, &TopEditorContainer::customTabContextMenuRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * , int )>(_a, &TopEditorContainer::tabCloseRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * , int )>(_a, &TopEditorContainer::editorAdded, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * , int , QWheelEvent * )>(_a, &TopEditorContainer::editorMouseWheel, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (TopEditorContainer::*)(EditorTabWidget * , int )>(_a, &TopEditorContainer::tabBarDoubleClicked, 7))
            return;
    }
}

const QMetaObject *NovaPad::UI::TopEditorContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NovaPad::UI::TopEditorContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7NovaPad2UI18TopEditorContainerE_t>.strings))
        return static_cast<void*>(this);
    return QSplitter::qt_metacast(_clname);
}

int NovaPad::UI::TopEditorContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSplitter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void NovaPad::UI::TopEditorContainer::currentTabChanged(EditorTabWidget * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void NovaPad::UI::TopEditorContainer::currentTabWidgetChanged(EditorTabWidget * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void NovaPad::UI::TopEditorContainer::currentEditorChanged(EditorTabWidget * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void NovaPad::UI::TopEditorContainer::customTabContextMenuRequested(QPoint _t1, EditorTabWidget * _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}

// SIGNAL 4
void NovaPad::UI::TopEditorContainer::tabCloseRequested(EditorTabWidget * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void NovaPad::UI::TopEditorContainer::editorAdded(EditorTabWidget * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void NovaPad::UI::TopEditorContainer::editorMouseWheel(EditorTabWidget * _t1, int _t2, QWheelEvent * _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2, _t3);
}

// SIGNAL 7
void NovaPad::UI::TopEditorContainer::tabBarDoubleClicked(EditorTabWidget * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2);
}
QT_WARNING_POP
