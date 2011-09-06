#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QList>
#include <QObject>
#include <QProcess>
#include <QTemporaryFile>
#include "profilemanager.h"
#include "profile.h"
#include "dbus_interface.h"

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT

public:
  TrayIcon(QWidget* parent = 0);
  void makeContextMenu();

private:
  QList<Profile> loadProfiles();
  void saveProfiles(QList<Profile> &profiles);
  QMenu menu;
  QSettings settings;

public slots:
  void activateProfile(QAction* action);
  void manageProfiles();
  void exitProxyManager();
  void networkChanged(QString key);

  private:

  DBusInterface *dbusInterface;
  QString currentSignature;

};

#endif // TRAYICON_H
