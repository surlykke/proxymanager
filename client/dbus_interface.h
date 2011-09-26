/*
  Copyright 2010, 2011 Christian Surlykke

  This file is part of ProxyManager.

  PitchTrainer is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3

  PitchTrainer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with PitchTrainer.  If not, see <http://www.gnu.org/licenses/>.
*/
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

