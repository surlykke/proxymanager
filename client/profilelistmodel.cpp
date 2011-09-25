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
    selectedProfile = -1;
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
    profile.save();
    beginInsertRows(QModelIndex(), profiles.size(), profiles.size());
    profiles.append(profile);
    endInsertRows();
    selectProfile(profiles.size() - 1);
}

QModelIndex ProfileListModel::currentIndex() {
    return createIndex(selectedProfile, 0);
}

int ProfileListModel::id2pos(QString id) {
    for (int i = 0; i < profiles.size(); i++) {
        if (profiles[i].id == id) {
            return i;
        }
    }
    return -1;
}

void ProfileListModel::loadProfiles() {
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
}

void ProfileListModel::saveProfiles() {
    QSettings settings;
    settings.remove("profiles");
    for (int i = 0; i < profiles.size(); i++) {
        profiles[i].save();
    }
}


void ProfileListModel::selectProfile(QString profileId) {
    for (int i = 0; i < profiles.size(); i++) {
        if (profiles[i].id == profileId) {
            selectProfile(i);
            return;
        }
    }
}

void ProfileListModel::selectProfile(int row) {
    if (row < 0 || row >= profiles.size()) {
        selectedProfile = -1;
    }
    else {
        selectedProfile = row;
    }
    emit selectedProfileChanged(selectedProfile);
}

void ProfileListModel::selectionChanged(QItemSelection selected, QItemSelection deselected) {
    QModelIndex index = selected.indexes()[0];
    if (index.isValid()) {
        selectProfile(index.row());
    }
}

