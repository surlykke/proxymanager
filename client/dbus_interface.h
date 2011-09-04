#ifndef DBUS_INTERFACE_H
#define DBUS_INTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface dk.surlykke.it.ProxyManager
 */
class DBusInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "dk.surlykke.it.ProxyManager"; }

public:
//    DBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);
    DBusInterface(const QDBusConnection &connection, QObject *parent = 0);


    ~DBusInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> ConfigureProxy(bool use_proxy, const QString &host, ushort port, const QStringList &host_exceptions, const QStringList &domain_exceptions)
    {
        qDebug() << "Ind i DBusInterface.ConfigureProxy";
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(use_proxy) << qVariantFromValue(host) << qVariantFromValue(port) << qVariantFromValue(host_exceptions) << qVariantFromValue(domain_exceptions);
        return asyncCallWithArgumentList(QLatin1String("ConfigureProxy"), argumentList);
    }

    inline QDBusPendingReply<QString> NetworkSignature()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("NetworkSignature"), argumentList);
    }

    inline void nch(const QString &signature) {
        qDebug() << "nch: " << signature;
    }

Q_SIGNALS: // SIGNALS
    void NetworkChanged(const QString &signature);
};

#endif

