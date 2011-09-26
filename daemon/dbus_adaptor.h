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
#ifndef DBUS_ADAPTOR_H
#define DBUS_ADAPTOR_H

#include <QtCore/QObject>
#include <QtCore/QMetaObject>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include "proxy_manager_daemon.h"

/*
 * Adaptor class for interface dk.surlykke.it.ProxyManager
 */
class DBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "dk.surlykke.it.ProxyManager")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"dk.surlykke.it.ProxyManager\">\n"
"    <method name=\"NetworkSignature\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"signature\"/>\n"
"    </method>\n"
"    <method name=\"ConfigureProxy\">\n"
"      <arg direction=\"in\" type=\"b\" name=\"use_proxy\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"host\"/>\n"
"      <arg direction=\"in\" type=\"q\" name=\"port\"/>\n"
"      <arg direction=\"in\" type=\"as\" name=\"host_exceptions\"/>\n"
"      <arg direction=\"in\" type=\"as\" name=\"domain_exceptions\"/>\n"
"    </method>\n"
"    <signal name=\"NetworkChanged\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"signature\"/>\n"
"    </signal>\n"
"  </interface>\n"
        "")
public:
    DBusAdaptor(ProxyManagerDaemon *daemon);
    virtual ~DBusAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void ConfigureProxy(bool use_proxy, const QString &host, ushort port, const QStringList &host_exceptions, const QStringList &domain_exceptions);
    QString NetworkSignature();
Q_SIGNALS: // SIGNALS
    void NetworkChanged(const QString signature);
private:
    ProxyManagerDaemon *daemon;
};

#endif

