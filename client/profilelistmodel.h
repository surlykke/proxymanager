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
  ProfileListModel(QList<Profile> & profiles, QObject *parent = 0);
  virtual ~ProfileListModel();

  // Implement methods defined in QAbstractListModel
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex & parent = QModelIndex()) const;
  virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
  // -----------------------------------------------

  // Some access methods
  Profile & operator[](int row);
  void profileUpdated(int row);
  QModelIndex addProfile();
  QList<Profile> & getProfiles();
  // -------------------

private:
  QList<Profile> m_profileList;

};

#endif /* TESTMODEL_H_ */
