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
#ifndef PROFILE_H
#define PROFILE_H
#include <QtCore>
#include <QDebug>
#include <QVariant>
#include <QMap>
#include <QString>
#include <QStringListModel>

struct Profile
{

    QString id; // Don't touch
    QString name; 
    bool useProxy;
    QString proxyHost;
    int proxyPort;
    QStringList hostExceptions;
    QStringList domainExceptions;

    void save();
    void load();
    void remove();


};

QDebug operator<<(QDebug dbg, const Profile &c);

#endif // PROFILE_H
