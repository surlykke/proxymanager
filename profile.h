/*
  Copyright 2011 Christian Surlykke

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
#ifndef PROFILE_H
#define PROFILE_H
#include <QString>
#include <QList>
#include <QDebug>

struct Profile
{
    QString id;
    QString name;
    bool useProxy;
    QString host;
    int port;
    QString exceptions;
    bool useAuthentication;
    QString ntDomain;
    QString userId;
    QString password;

    void save();
    static Profile loadProfile(QString id);
    static QList<Profile> loadAll();
    static Profile newProfile();
    static void deleteProfile(QString id);

private:
    void load();

};

QDebug operator<<(QDebug debug, Profile& profile);


#endif // PROFILE_H
