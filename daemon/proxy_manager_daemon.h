/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c ProxyManagerDaemon -a proxy_manager_daemon.h proxy_manager_interface.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef PROXY_MANAGER_DAEMON_H
#define PROXY_MANAGER_DAEMON_H

#include <QCoreApplication>
#include <QFileSystemWatcher>
#include <netdb.h>
#include <QSet>
#include <QStringList>
#include <QString>

/*
 * Adaptor class for interface dk.surlykke.it.ProxyManager
 */
class ProxyManagerDaemon: public QCoreApplication {
    Q_OBJECT
public:
    ProxyManagerDaemon(int argc, char* argv[]);
    virtual ~ProxyManagerDaemon();


    void configureProxy(bool use_proxy, const QString &host, ushort port, const QStringList &host_exceptions, const QStringList &domain_exceptions);
    QString networkSignature();

Q_SIGNALS:
    void NetworkChanged(const QString newSignature);

private slots:
    void fileChanged(QString path);

private:
    QFileSystemWatcher resolvConfWatcher;
    bool notWhiteSpace(QString& string);

};

#endif

