#include "notifications.h"
/*
 * Implementation of interface class Notifications
 */

Notifications::Notifications(const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", staticInterfaceName(), connection, parent)
{
}

Notifications::~Notifications()
{
}
