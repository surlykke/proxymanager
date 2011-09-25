/*
 * profilelistmodel.h
 *
 *  Created on: 12/03/2010
 *      Author: christian
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

  int id2pos(QString id);
  // -------------------

public slots:
  void selectionChanged(QItemSelection selected, QItemSelection deselected);

signals:
  void selectedProfileChanged(int newSelectedProfile);


private:

};

#endif /* TESTMODEL_H_ */
