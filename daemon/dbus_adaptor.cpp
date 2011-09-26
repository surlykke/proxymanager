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
#include "dbus_adaptor.h"

DBusAdaptor::DBusAdaptor(ProxyManagerDaemon *daemon)
    : QDBusAbstractAdaptor(daemon)
{
    // constructor
    this->daemon = daemon;
    setAutoRelaySignals(true);
}

DBusAdaptor::~DBusAdaptor()
{
    // destructor
}

void DBusAdaptor::ConfigureProxy(bool use_proxy, const QString &host, ushort port, const QStringList &host_exceptions, const QStringList &domain_exceptions)
{
    daemon->configureProxy(use_proxy, host, port, host_exceptions, domain_exceptions);
}

QString DBusAdaptor::NetworkSignature()
{
    // handle method call dk.surlykke.it.ProxyManager.NetworkSignature
    return daemon->networkSignature();
}
