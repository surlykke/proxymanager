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
#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QString>
#include <QUrl>
#include <QMenu>
#include <QList>
#include <QObject>
#include <QProcess>
#include <QTemporaryFile>
#include "profiledialog.h"
#include <QFileSystemWatcher>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTextStream>
#include <QProcess>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TrayIcon(QWidget* parent = 0);
    void makeContextMenu();

public slots:
    void close();
    void chooseProfile(QAction *action);
    void manageProfiles();
    void exitProxyManager();
    void resolvconfChanged();


private:
    void activateProfile(QString profileId);
    void writeForProtocol(QVariantMap& profile, QString protocol, QTextStream& textStream);
    QUrl proxySettingUrl;
    ProfileListModel profileListModel;
    void notify(QString summary, QString message);
    bool notWhiteSpace(QString& string);
    QString networkSignature();
    QString currentNetworkSignature;
    QString currentProfileId;
    QFileSystemWatcher resolvconfWatcher;
    QProcess cntlmProcess;
};

#endif // TRAYICON_H
