/****************************************************************************
** Meta object code from reading C++ file 'QListWidgetItemPlot.h'
**
** Created: Wed 19. Nov 21:12:15 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QListWidgetItemPlot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QListWidgetItemPlot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QListWidgetItemPlot[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      26,   20,   21,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QListWidgetItemPlot[] = {
    "QListWidgetItemPlot\0\0bool\0EditItem()\0"
};

const QMetaObject QListWidgetItemPlot::staticMetaObject = {
    { &QListWidgetItem::staticMetaObject, qt_meta_stringdata_QListWidgetItemPlot,
      qt_meta_data_QListWidgetItemPlot, 0 }
};

const QMetaObject *QListWidgetItemPlot::metaObject() const
{
    return &staticMetaObject;
}

void *QListWidgetItemPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QListWidgetItemPlot))
        return static_cast<void*>(const_cast< QListWidgetItemPlot*>(this));
    return QListWidgetItem::qt_metacast(_clname);
}

int QListWidgetItemPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidgetItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = EditItem();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
