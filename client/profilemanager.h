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
  ProfileManager(QList<Profile> profiles, QWidget * parent = 0);
  ~ProfileManager();
  QList<Profile> & getProfiles();


public slots:
  void selectionChanged(QItemSelection selected, QItemSelection deselected);
  void newProfile();
  void deleteProfile();
  void profileDataChanged();
  void newHostException();
  void deleteHostException();
  void newDomainException();
  void deleteDomainException(); 

protected:
  void changeEvent(QEvent *e);

private:
  int currentSelection();

  Ui::ProfileManager *m_ui;
  ProfileListModel profileListModel;
  bool m_doing_selection_change;

  // Debug
  void showProfiles(QString msg = "");


};

#endif // PROFILEMANAGER_H
