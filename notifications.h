/*
  Copyright 2011, 2012 Christian Surlykke

  This file is part of ProxyManager

  ProxyManager is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3

  ProxyManager is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with ProxyManager.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NOTIFICATIONS_H_1315590705
#define NOTIFICATIONS_H_1315590705

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <QMap>
/*
 * Proxy class for interface org.freedesktop.Notifications
 */
class Notifications: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.Notifications"; }

public:
    Notifications(const QDBusConnection &connection, QObject *parent = 0);

    ~Notifications();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<uint> Notify(const QString &app_name, uint id, const QString &icon, const QString &summary, const QString &body, const QStringList &actions, const QMap<QString, QVariant> &hints, int timeout)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(app_name) << qVariantFromValue(id) << qVariantFromValue(icon) << qVariantFromValue(summary) << qVariantFromValue(body) << qVariantFromValue(actions) << qVariantFromValue(hints) << qVariantFromValue(timeout);
        return asyncCallWithArgumentList(QLatin1String("Notify"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

#endif


