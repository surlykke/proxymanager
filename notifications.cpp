/*
  Copyright 2011 Christian Surlykke

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
