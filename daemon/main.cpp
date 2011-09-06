#include <QtCore/QCoreApplication>
#include "proxy_manager_daemon.h"
#include <QDBusConnection>
#include <QDebug>
#include "dbus_adaptor.h"
#include <iostream>
#include <QString>
#include <QFile>
#include <QDir>
#include "proxy_manager_consts.h"

int main(int argc, char *argv[])
{
    // Are we root?
    if (getuid() != 0) {
        std::cerr << "You must be root to run proxymanagerd!\n";
        return 1;
    }

    // Make shure vi have a dir under /var/run
    QDir rundir(RUN_DIR_PATH);
    if (! (rundir.exists() || rundir.mkpath(RUN_DIR_PATH))) {
        std::cerr << RUN_DIR_PATH << " could not be created\n";
        return 1;
    }

    // Are we allready running?
    QFile pidfile(PID_FILE_PATH);
    if (pidfile.exists()) {
        std::cerr << "proxymanagerd seems to be already running - found " << PID_FILE_PATH << "\n";
        std::cerr.flush();
        return 1;
    }

    // Reset squid config
    QFile configFile(SQUID_CONF_PATH);
    if (! configFile.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        std::cerr << "Could not open " << SQUID_CONF_PATH << "\n";
        return 1;
        configFile.close();
        system("squid -k reconfigure");
    }

    // Daemonize
    int pid = fork();
    if (pid < 0) {
        return 1;
    }
    else if (pid == 0) {
        // child process - gets to continue
    }
    else if (pid > 0) {
        return 0;
    }

    const QString SERVICE_NAME("dk.surlykke.it.ProxyManager");
    const QString OBJECT_PATH("/ProxyManager");

    ProxyManagerDaemon daemon(argc, argv);
    new DBusAdaptor(&daemon);


    if (!QDBusConnection::systemBus().isConnected()) {
        std::cerr << "Could not connect to the D-Bus system bus.\n";
        return 1;
    }

    if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
        QString msg = QDBusConnection::systemBus().lastError().message();
        std::cerr << "Problem registering service " << qPrintable(SERVICE_NAME) << ": " << qPrintable(msg) << "\n";
        return 1;
    }

    if (!QDBusConnection::systemBus().registerObject(OBJECT_PATH, &daemon)) {
        QString msg = QDBusConnection::systemBus().lastError().message();
        std::cerr << "Problemlem registering path " << qPrintable(OBJECT_PATH) << ": " << qPrintable(msg) << "\n";
    }

    // Write pid file for start-stop-daemon
    if (pidfile.open(QIODevice::WriteOnly)) {
        QTextStream(&pidfile) << "" << getpid();
        pidfile.close();
    }
    else {
        std::cerr << "Could not open " << PID_FILE_PATH << "\n";
        return 1;
    }

    return  daemon.exec();
}

