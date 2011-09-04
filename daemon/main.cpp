#include <QtCore/QCoreApplication>
#include "proxy_manager_daemon.h"
#include <QDBusConnection>
#include <QDebug>
#include "dbus_adaptor.h"
#include <iostream>
#include <QString>

int main(int argc, char *argv[])
{

    const QString SERVICE_NAME("dk.surlykke.it.ProxyManager");
    const QString OBJECT_PATH("/ProxyManager");

    ProxyManagerDaemon daemon(argc, argv);
    new DBusAdaptor(&daemon);

    // Daemonize
    int pid = fork();
    if (pid < 0) {
        return 1;
    }
    else if (pid == 0) {
        // child process
    }
    else if (pid > 0) {
        return 0;
    }


/*    if (getuid() != 0) {
        std::cerr << "You must be root to run proxymanagerd!\n";
        return 1;
    }*/

    if (!QDBusConnection::systemBus().isConnected()) {
        std::cerr << "Could not connect to the D-Bus system bus.\n";
        return 1;
    }

    if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
        QString msg = QDBusConnection::systemBus().lastError().message();
        std::cerr << "Problem registering " << qPrintable(SERVICE_NAME) << ": " << qPrintable(msg) << "\n";
        return 1;
    }

    if (!QDBusConnection::systemBus().registerObject(OBJECT_PATH, &daemon)) {
        QString msg = QDBusConnection::systemBus().lastError().message();
        std::cerr << "Problemlem registering path " << qPrintable(OBJECT_PATH) << ": " << qPrintable(msg) << "\n";
    }

    return daemon.exec();
}

