#include "dbus_adaptor.h"
/*
 * Implementation of adaptor class DBusAdaptor
 */

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
