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
#ifndef TESTMODEL_H_
#define TESTMODEL_H_
#include <QtCore>
#include "profile.h"

class ProfileListModel : public QAbstractListModel {
  Q_OBJECT
public:
  ProfileListModel(QObject *parent = 0);
  virtual ~ProfileListModel();

  // Implement methods defined in QAbstractListModel
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex & parent = QModelIndex()) const;
  virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
  // -----------------------------------------------

  void newProfile();
  QModelIndex currentIndex();
  QList<Profile> profiles;
  int selectedProfile;

  void selectProfile(QString profileId);
  void selectProfile(int row);

  void loadProfiles();
  void saveProfiles();

public slots:
  void selectionChanged(QItemSelection selected, QItemSelection deselected);
  void nameChanged(QString newName);
  void useProxyChanged(int useProxy);
  void proxyHostChanged(QString newProxy);
  void proxyPortChanged(int newPort);
  void hostExceptionListChanged(QStringList newHostExceptionList);
  void domainExceptionListChanged(QStringList newDomainExceptionList);

signals:
  void selectedProfileChanged(int newSelectedProfile);


private:
  bool pendingChanges;
};

#endif /* TESTMODEL_H_ */
