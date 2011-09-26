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
#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QtGui/QDialog>
#include <QtCore>
#include <QMap>
#include <QtGui>
#include "profilelistmodel.h"
#include "profile.h"

namespace Ui {
    class ProfileManager;
}

class ProfileManager : public QDialog {
  Q_OBJECT

public:
  ProfileManager(ProfileListModel* profileListModel, QWidget * parent = 0);
  ~ProfileManager();

public slots:
  void showProfile(Profile *profile);
  void newProfile();
  void deleteProfile();
  void newHostException();
  void deleteHostException();
  void newDomainException();
  void deleteDomainException(); 

  virtual void accept();
  virtual void reject();


protected:
  void changeEvent(QEvent *e);

private:
  int currentSelection();

  Ui::ProfileManager *m_ui;
  ProfileListModel* profileListModel;
  bool updating_ui;

  QStringList widgetListData(QListWidget *list);

  // Debug
  void showProfiles(QString msg = "");


};

#endif // PROFILEMANAGER_H
