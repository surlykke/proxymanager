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
  void showProfile(int row);
  void newProfile();
  void deleteProfile();
  void profileDataChanged();
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

  // Debug
  void showProfiles(QString msg = "");


};

#endif // PROFILEMANAGER_H
