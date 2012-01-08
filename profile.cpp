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
#include "profile.h"
#include <QSettings>
#include <QUuid>
#include <QStringList>
#include <QDebug>

// Will only save if profile already exists in settings
void Profile::save() {
    QString prefix = "profiles/" + id + "/";
    QSettings settings;
    if (settings.contains(prefix + "id")) {
        settings.setValue(prefix + "name", name);
        settings.setValue(prefix + "useProxy", useProxy);
        settings.setValue(prefix + "host", host);
        settings.setValue(prefix + "port", port);
        settings.setValue(prefix + "exceptions", exceptions);
        settings.setValue(prefix + "useAuthentication", useAuthentication);
        settings.setValue(prefix + "ntDomain", ntDomain);
        settings.setValue(prefix + "userId", userId);
        settings.setValue(prefix + "password", password);
    }
}

Profile Profile::loadProfile(QString id) {
    Profile profile;
    profile.id = id;
    if (QSettings().contains("profiles/" + id)) {
        profile.load();
    }
    return profile;
}

// Caller must ensure id is set and exists.
void Profile::load() {
    QSettings settings;
    QString prefix = "profiles/" + id + "/";
    name = settings.value(prefix + "name").toString();
    useProxy = settings.value(prefix + "useProxy").toBool();
    host = settings.value(prefix + "host").toString();
    port = settings.value(prefix + "port").toInt();
    exceptions = settings.value(prefix + "exceptions").toString();
    useAuthentication = settings.value(prefix + "useAuthentication").toBool();
    ntDomain = settings.value(prefix + "ntDomain").toString();
    userId = settings.value(prefix + "userId").toString();
    password = settings.value(prefix + "password").toString();
}

QList<Profile> Profile::loadAll() {
    QList<Profile> result;
    QSettings settings;
    settings.beginGroup("profiles");
    QString id;
    foreach (id, settings.childGroups()) {
       Profile profile;
       profile.id = id;
       profile.load();
       result << profile;
    }
    settings.endGroup();
    return result;
}

Profile Profile::newProfile() {
    QString id = QUuid::createUuid().toString();
    // Strip leading '{' and trailing '}'
    id = id.right(id.size() - 1);
    id = id.left(id.size() - 1);

    Profile profile;
    profile.id = id;
    profile.name = "New profile...";
    QSettings().setValue("profiles/" + id + "/id", id);
    profile.save();
    return profile;
}

void Profile::deleteProfile(QString id) {
    QSettings().remove("profiles/" + id);
}

QDebug operator<<(QDebug debug, Profile& profile) {
    debug.nospace() << "id:" << profile.id << ", name:" << profile.name << ", useProxy:" << profile.useProxy << ", host:" << profile.host <<
                       ", port:" << profile.port << ", exceptions:" << profile.exceptions << ", useAuthentication:" <<
                       profile.useAuthentication << ", ntDomain:" << profile.ntDomain << ", userId:" << profile.userId << ", password:" << profile.password;
    return debug.space();
}
