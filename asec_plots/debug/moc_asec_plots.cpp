/****************************************************************************
** Meta object code from reading C++ file 'asec_plots.h'
**
** Created: Thu 20. Nov 00:20:34 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../asec_plots.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'asec_plots.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_asec_plots[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      35,   11,   11,   11, 0x0a,
      58,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_asec_plots[] = {
    "asec_plots\0\0on_btAddPlot_clicked()\0"
    "on_btDelPlot_clicked()\0on_btEditPlot_clicked()\0"
};

const QMetaObject asec_plots::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_asec_plots,
      qt_meta_data_asec_plots, 0 }
};

const QMetaObject *asec_plots::metaObject() const
{
    return &staticMetaObject;
}

void *asec_plots::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_asec_plots))
        return static_cast<void*>(const_cast< asec_plots*>(this));
    return QWidget::qt_metacast(_clname);
}

int asec_plots::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_btAddPlot_clicked(); break;
        case 1: on_btDelPlot_clicked(); break;
        case 2: on_btEditPlot_clicked(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
