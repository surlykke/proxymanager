#include "dbus_interface.h"

//DBusInterface::DBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
DBusInterface::DBusInterface(const QDBusConnection &connection, QObject *parent)
     : QDBusAbstractInterface("dk.surlykke.it.ProxyManager", "/ProxyManager", "dk.surlykke.it.ProxyManager", connection, parent) {
}

DBusInterface::~DBusInterface()
{
}
