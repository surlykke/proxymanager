/*
 * profilelistmodel.cpp
 *
 *  Created on: 12/03/2010j
 *      Author: christian
 */

#include "profilelistmodel.h"

ProfileListModel::ProfileListModel(QList<Profile> & profileList, QObject *parent) : QAbstractListModel(parent), m_profileList(profileList) {
}

ProfileListModel::~ProfileListModel() {
}


QVariant ProfileListModel::data(const QModelIndex & index, int role) const {
  if (index.isValid() &&
      (index.row() >= 0) &&
      (index.row() < m_profileList.size()) &&
      (index.column() == 0) &&
      role == Qt::DisplayRole) {
    return QVariant(m_profileList[index.row()].name);
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
    return m_profileList.size();
  }
}

bool ProfileListModel::removeRows(int row, int count, const QModelIndex & parent) {
  if ((!parent.isValid()) &&
      row >= 0 &&
      (row + count) <= m_profileList.size()) {
    beginRemoveRows(parent, row, row + count - 1);
    for (int i =  0; i < count; i++) {
      m_profileList.removeAt(row);
    }
    endRemoveRows();
    return true;
  }
  else {
    return false;
  }
}

Profile & ProfileListModel::operator[](int row) {
  return m_profileList[row];
}

void ProfileListModel::profileUpdated(int row) {
  QModelIndex index = createIndex(row, row);
  dataChanged(index, index);
}

QModelIndex ProfileListModel::addProfile() {
  Profile profile;
  profile.name = "Choose a profile name..";
  profile.useProxy = false;
  profile.proxyHost = "";
  profile.proxyPort = 0;
  profile.hostExceptions = QStringList();
  profile.domainExceptions = QStringList();

  beginInsertRows(QModelIndex(), m_profileList.size(), m_profileList.size());
  m_profileList.append(profile);
  endInsertRows();
  return createIndex(m_profileList.size() - 1, 0);
}

QList<Profile> & ProfileListModel::getProfiles() {
  return m_profileList;
}
