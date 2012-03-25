/*
  Copyright 2011, 2012 Christian Surlykke

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
#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QtGui/QDialog>
#include <QtCore>
#include <QDataWidgetMapper>
#include <QMap>
#include <QtGui>

static const char USEPROXY[] = "useProxy";
static const char HOST[] = "host";
static const char PORT[] = "port";
static const char EXCEPTIONS[] = "exceptions";
static const char USEAUTHENTICATION[] = "useAuthentication";
static const char NTDOMAIN[] = "ntDomain";
static const char USERID[] = "userId";
static const char PASSWORD[] = "password";

namespace Ui {
    class ProfileManager;
}

class ProfileDialog : public QDialog {
    Q_OBJECT

public:


    ProfileDialog(QString selectedProfileName, QWidget * parent = 0);
    ~ProfileDialog();

private slots:
    void newProfile();
    void deleteProfile();
    void loadProfile();
    void saveProfile();
    void itemChanged(QListWidgetItem *item, QListWidgetItem *oldItem);
    void itemChanged(QListWidgetItem *item);
    void closeOrResetButtonClicked(QAbstractButton *button);

private:
    void loadProfiles();
    void setSelection(QString profileName);
    QListWidgetItem* addItem(QString profileName);
    QString newName(QString hint = "");
    Ui::ProfileManager *m_ui;
    QString currentProfileName;
    QString selectedProfileName;
    QSettings settings;
    QMutex lock;
    QMap<QString, QVariant> rollbackPoint;
};

#endif // PROFILEMANAGER_H
