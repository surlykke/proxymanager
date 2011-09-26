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

    QString currentNetworkSignature;
};

#endif

