/****************************************************************************
** Meta object code from reading C++ file 'proxy_manager_interface.h'
**
** Created: Fri Aug 26 17:32:00 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "proxy_manager_interface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'proxy_manager_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProxyManagerInterface[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   23,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     131,   77,   57,   22, 0x0a,
     218,   22,  191,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ProxyManagerInterface[] = {
    "ProxyManagerInterface\0\0signature\0"
    "NetworkChanged(QString)\0QDBusPendingReply<>\0"
    "use_proxy,host,port,host_exceptions,domain_exceptions\0"
    "ConfigureProxy(bool,QString,ushort,QStringList,QStringList)\0"
    "QDBusPendingReply<QString>\0"
    "NetworkSignature()\0"
};

const QMetaObject ProxyManagerInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_ProxyManagerInterface,
      qt_meta_data_ProxyManagerInterface, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProxyManagerInterface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProxyManagerInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProxyManagerInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProxyManagerInterface))
        return static_cast<void*>(const_cast< ProxyManagerInterface*>(this));
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int ProxyManagerInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: NetworkChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: { QDBusPendingReply<> _r = ConfigureProxy((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4])),(*reinterpret_cast< const QStringList(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = _r; }  break;
        case 2: { QDBusPendingReply<QString> _r = NetworkSignature();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QString>*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ProxyManagerInterface::NetworkChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
