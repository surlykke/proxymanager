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
#include <QMenu>
#include <QList>
#include <QObject>
#include <QProcess>
#include <QTemporaryFile>
#include "profilemanager.h"
#include "profile.h"
#include "dbus_interface.h"
#include "notifications.h"

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TrayIcon(QWidget* parent = 0);
    void makeContextMenu();

private:
    QList<Profile> loadProfiles();
    void saveProfiles(QList<Profile> &profiles);
    QMenu menu;
    QSettings settings;

public slots:
    void activateProfile(QAction* action);
    void activateProfile(const Profile& profile);
    void rememberAssociation(QString networkSignature, QString profileId);
    QString profileId4signature(QString networkSignature);

    void manageProfiles();
    void exitProxyManager();
    void networkChanged(QString key);

private:

    DBusInterface *dbusInterface;
    QString currentSignature;
    QString currentProfileId;
    Notifications *notifications;

    ProfileListModel profileListModel;
};

#endif // TRAYICON_H
