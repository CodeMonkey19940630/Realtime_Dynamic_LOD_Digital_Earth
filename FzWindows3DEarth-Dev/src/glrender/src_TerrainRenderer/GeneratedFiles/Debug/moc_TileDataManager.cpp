/****************************************************************************
** Meta object code from reading C++ file 'TileDataManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TileDataManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TileDataManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TileDataManager_t {
    QByteArrayData data[8];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TileDataManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TileDataManager_t qt_meta_stringdata_TileDataManager = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TileDataManager"
QT_MOC_LITERAL(1, 16, 21), // "signal_load_tile_data"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 11), // "FzQuadTile*"
QT_MOC_LITERAL(4, 51, 5), // "_tile"
QT_MOC_LITERAL(5, 57, 26), // "std::shared_ptr<TILEDATA>*"
QT_MOC_LITERAL(6, 84, 10), // "_tile_data"
QT_MOC_LITERAL(7, 95, 19) // "slot_load_tile_data"

    },
    "TileDataManager\0signal_load_tile_data\0"
    "\0FzQuadTile*\0_tile\0std::shared_ptr<TILEDATA>*\0"
    "_tile_data\0slot_load_tile_data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TileDataManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

       0        // eod
};

void TileDataManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TileDataManager *_t = static_cast<TileDataManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_load_tile_data((*reinterpret_cast< FzQuadTile*(*)>(_a[1])),(*reinterpret_cast< std::shared_ptr<TILEDATA>*(*)>(_a[2]))); break;
        case 1: _t->slot_load_tile_data((*reinterpret_cast< FzQuadTile*(*)>(_a[1])),(*reinterpret_cast< std::shared_ptr<TILEDATA>*(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TileDataManager::*_t)(FzQuadTile * , std::shared_ptr<TILEDATA> * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TileDataManager::signal_load_tile_data)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TileDataManager::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TileDataManager.data,
      qt_meta_data_TileDataManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TileDataManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TileDataManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TileDataManager.stringdata0))
        return static_cast<void*>(const_cast< TileDataManager*>(this));
    return QThread::qt_metacast(_clname);
}

int TileDataManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TileDataManager::signal_load_tile_data(FzQuadTile * _t1, std::shared_ptr<TILEDATA> * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
