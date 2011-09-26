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
#include "dbus_interface.h"

DBusInterface::DBusInterface(const QDBusConnection &connection, QObject *parent)
     : QDBusAbstractInterface("dk.surlykke.it.ProxyManager", "/ProxyManager", "dk.surlykke.it.ProxyManager", connection, parent) {
}

DBusInterface::~DBusInterface()
{
}
