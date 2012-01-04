/*
 * profilelistmodel.cpp
 *
 *  Created on: 12/03/2010j
 *      Author: christian
 */

#include <QUuid>
#include "profilelistmodel.h"
#include <QMessageBox>
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include <QFile>
#include <QSettings>
#include "profile.h"

ProfileListModel::ProfileListModel(QObject *parent) : QStandardItemModel(parent) {
    loadProfiles();
}

ProfileListModel::~ProfileListModel() {
    setColumnCount(COLUMNS);
}

QString ProfileListModel::id(int row) {
    return item(row, ID)->data(Qt::DisplayRole).toString();
}

QString ProfileListModel::name(int row) {
    return item(row, NAME)->data(Qt::DisplayRole).toString();
}

bool ProfileListModel::exists(QString id) {
    for (int row = 0; row < rowCount(); row++) {
        if (id == item(row, ID)->data(Qt::DisplayRole).toString()) {
            return true;
        }
    }
    return false;
}

bool ProfileListModel::newProfile() {
    Profile profile = Profile::newProfile();
    createdProfiles << profile.id;
    append(profile);
    return true;
}

void ProfileListModel::deleteProfile(int row) {
    if (row >= 0 && row < rowCount()) {
        pendingDeletes << item(row, ID)->data(Qt::DisplayRole).toString();
        removeRow(row, QModelIndex());
    }
}

Profile ProfileListModel::id2profile(QString id) {
    for (int row = 0; row < rowCount(); row++) {
        if (id == item(row, ID)->data(Qt::DisplayRole).toString()) {
            return row2profile(row);
        }
    }
    return Profile();
}

bool ProfileListModel::loadProfiles() {
    clear();
    QList<Profile> profiles = Profile::loadAll();
    Profile profile;
    foreach (profile, profiles) {
        qDebug() << "Appending: " << profile;
        append(profile);
    }

    pendingDeletes.clear();
    createdProfiles.clear();
    return true;
}

bool ProfileListModel::commit() {
    QString id;
    foreach (id, pendingDeletes) {
        Profile::deleteProfile(id);
    }

    // FIXME only save changed profiles
    for (int row = 0; row < rowCount(); row++) {
        row2profile(row).save();
    }

    pendingDeletes.clear();
    createdProfiles.clear();

    return true;
}

bool ProfileListModel::rollback() {
    QString id;
    foreach (id, createdProfiles) {
        Profile::deleteProfile(id);
    }

    loadProfiles();
    pendingDeletes.clear();
    createdProfiles.clear();

    return true;
}


void ProfileListModel::append(Profile &profile) {
    int row = rowCount();
    setItem(row, NAME, new QStandardItem(profile.name));
    setItem(row, ID, new QStandardItem(profile.id));
    setItem(row, USE_PROXY, new QStandardItem(profile.useProxy ? "true" : "false"));
    setItem(row, HOST, new QStandardItem(profile.host));
    setItem(row, PORT, new QStandardItem(QString::number(profile.port)));
    setItem(row, EXCEPTIONS, new QStandardItem(profile.exceptions));
    setItem(row, USE_AUTHENTICATION, new QStandardItem(profile.useAuthentication ? "true" : "false"));
    setItem(row, NTDOMAIN, new QStandardItem(profile.ntDomain));
    setItem(row, USERID, new QStandardItem(profile.userId));
    setItem(row, PASSWORD, new QStandardItem(profile.password));
}

Profile ProfileListModel::row2profile(int row) {
    Profile profile;
    profile.name = item(row, NAME)->data(Qt::DisplayRole).toString();
    profile.id = item(row, ID)->data(Qt::DisplayRole).toString();
    profile.useProxy = item(row, USE_PROXY)->data(Qt::DisplayRole).toBool();
    profile.host = item(row, HOST)->data(Qt::DisplayRole).toString();
    profile.port = item(row, PORT)->data(Qt::DisplayRole).toInt();
    profile.exceptions = item(row, EXCEPTIONS)->data(Qt::DisplayRole).toString();
    profile.useAuthentication = item(row, USE_AUTHENTICATION)->data(Qt::DisplayRole).toBool();
    profile.ntDomain = item(row, NTDOMAIN)->data(Qt::DisplayRole).toString();
    profile.userId = item(row, USERID)->data(Qt::DisplayRole).toString();
    profile.password = item(row, PASSWORD)->data(Qt::DisplayRole).toString();
    return profile;
}
