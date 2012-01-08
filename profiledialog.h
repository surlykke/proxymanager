/*
  Copyright 2011, 2012 Christian Surlykke

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
#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QtGui/QDialog>
#include <QtCore>
#include <QDataWidgetMapper>
#include <QMap>
#include <QtGui>
#include "profilelistmodel.h"

namespace Ui {
    class ProfileManager;
}

class ProfileDialog : public QDialog {
  Q_OBJECT

public:
  ProfileDialog(QString selectedProfileId, ProfileListModel* profileListModel, QWidget * parent = 0);
  ~ProfileDialog();

public slots:
  void newProfile();
  void deleteProfile();
  void selectionChanged(QItemSelection start, QItemSelection end);
  virtual void accept();
  virtual void reject();


private:
  int currentSelection();
  void select(int row, bool edit = false);

  Ui::ProfileManager *m_ui;
  ProfileListModel* profileListModel;
  QDataWidgetMapper *mapper;

};

#endif // PROFILEMANAGER_H
