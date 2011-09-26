/*
 * profilelistmodel.cpp
 *
 *  Created on: 12/03/2010j
 *      Author: christian
 */

#include <QUuid>
#include "profilelistmodel.h"

ProfileListModel::ProfileListModel(QObject *parent) : QAbstractListModel(parent) {
    loadProfiles();
    selectedProfile = 0;
    pendingChanges = false;
}

ProfileListModel::~ProfileListModel() {
}


QVariant ProfileListModel::data(const QModelIndex & index, int role) const {
    if (index.isValid() &&
            (index.row() >= 0) &&
            (index.row() < profiles.size()) &&
            (index.column() == 0) &&
            role == Qt::DisplayRole) {
        return QVariant(profiles[index.row()].name);
    }
    else {
        return QVariant();
    }
}

int ProfileListModel::rowCount(const QModelIndex & parent) const {
    if (parent.isValid()) {
        return 0;
    }
    else {
        return profiles.size();
    }
}

bool ProfileListModel::removeRows(int row, int count, const QModelIndex & parent) {
    if ((!parent.isValid()) && row >= 0 && (row + count) <= profiles.size()) {
        QSettings settings;
        settings.beginGroup("profiles");
        beginRemoveRows(parent, row, row + count - 1);
        for (int i =  0; i < count; i++) {
            profiles[row].remove();
            profiles.removeAt(row);
        }
        endRemoveRows();
        settings.endGroup();
        pendingChanges = true;
        return true;
    }
    else {
        return false;
    }
}



void ProfileListModel::newProfile() {
    Profile profile;
    profile.id = QUuid::createUuid();
    profile.name = "Choose a profile name..";
    beginInsertRows(QModelIndex(), profiles.size(), profiles.size());
    profiles.append(profile);
    pendingChanges = true;
    endInsertRows();
    selectProfile(profiles.size() - 1);
}

QModelIndex ProfileListModel::currentIndex() {
    for (int i = 0; i < profiles.size(); i++) {
        if (selectedProfile->id == profiles[i].id) {
            return createIndex(i, 0);
        }
    }
    return QModelIndex();
}

void ProfileListModel::loadProfiles() {
    if (pendingChanges) {
        QSettings settings;
        settings.beginGroup("profiles");
        QStringList ids = settings.childGroups();
        for (int i = 0; i < ids.size(); i++) {
            Profile profile;
            profile.id = ids[i];
            profile.load();
            profiles.append(profile);
        }
        settings.endGroup();
        pendingChanges = false;
    }
}

void ProfileListModel::saveProfiles() {
    if (pendingChanges) {
        QSettings settings;
        settings.remove("profiles");
        for (int i = 0; i < profiles.size(); i++) {
            profiles[i].save();
        }
        pendingChanges = false;
    }
}

void ProfileListModel::selectProfile(QString profileId) {
    for (int i = 0; i < profiles.size(); i++) {
        if (profiles[i].id == profileId) {
            selectedProfile = &profiles[i];
            return;
        }
    }
    selectedProfile = 0;
}

void ProfileListModel::selectProfile(int row) {
    if (row < 0 || row >= profiles.size()) {
        selectedProfile = 0;
    }
    else {
        selectedProfile = &profiles[row];
    }
    emit selectedProfileChanged(selectedProfile);
}

void ProfileListModel::selectionChanged(QItemSelection selected, QItemSelection deselected) {
    QModelIndex index = selected.indexes()[0];
    if (index.isValid()) {
        selectProfile(index.row());
    }
}

void ProfileListModel::nameChanged(QString newName) {
    if (selectedProfile != 0 && newName != selectedProfile->name) {
        selectedProfile->name = newName;
        QModelIndex index = currentIndex();
        dataChanged(index, index);
        pendingChanges = true;
    }
}

void ProfileListModel::useProxyChanged(int useProxy) {
    if (selectedProfile != 0 && selectedProfile->useProxy != (useProxy != 0)) {
        selectedProfile->useProxy = (useProxy != 0);
        pendingChanges = true;
    }
}

void ProfileListModel::proxyHostChanged(QString newProxyHost) {
    if (selectedProfile != 0 && selectedProfile->proxyHost != newProxyHost) {
        selectedProfile->proxyHost = newProxyHost;
        pendingChanges = true;
    }
}

void ProfileListModel::proxyPortChanged(int newPort) {
    if (selectedProfile != 0 && selectedProfile->proxyPort != newPort) {
        selectedProfile->proxyPort = newPort;
        pendingChanges = true;
    }
}

void ProfileListModel::hostExceptionListChanged(QStringList newHostExceptionList) {
    if (selectedProfile != 0) {
        selectedProfile->hostExceptions.clear();
        selectedProfile->hostExceptions.append(newHostExceptionList);
        pendingChanges = true;
    }
}

void ProfileListModel::domainExceptionListChanged(QStringList newDomainExceptionList) {
    if (selectedProfile != 0) {
        selectedProfile->domainExceptions.clear();
        selectedProfile->domainExceptions.append(newDomainExceptionList);
        pendingChanges = true;
    }
}
