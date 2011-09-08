#include "trayicon.h"
#include <QEvent>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QDir>
#include <QFile>
#include <QDBusConnection>
#include <QDBusPendingReply>
#include "profile.h"

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent), menu(parent), currentSignature("") {
    setIcon(QIcon(":icons/icon.svg"));
    setContextMenu(&menu);
    connect(contextMenu(), SIGNAL(triggered(QAction*)), this, SLOT(activateProfile(QAction*)));;
    qDebug() << "Create dbusInterface";
    dbusInterface = new DBusInterface(QDBusConnection::systemBus(), this);
    qDebug() << "dbusInterface created...";
    connect(dbusInterface, SIGNAL(NetworkChanged(QString)), this, SLOT(networkChanged(QString)));
    QDBusReply<QString> signatureReply = dbusInterface->NetworkSignature();
    if (signatureReply.isValid()) {
        currentSignature = signatureReply.value();
    }
    makeContextMenu();
}

void TrayIcon::makeContextMenu() {
  contextMenu()->clear();
  QList<Profile> profiles = loadProfiles();
  QActionGroup* profileGroup = new QActionGroup(this);
  for (int i = 0; i < profiles.size(); i++) {
    QAction* profileAction = menu.addAction(profiles[i].name);
    profileAction->setData(i);
    profileAction->setCheckable(true);
    profileGroup->addAction(profileAction);
  }

  contextMenu()->addSeparator();

  QAction* manageProfileAction = contextMenu()->addAction("Manage profiles");
  connect(manageProfileAction, SIGNAL(triggered()), this, SLOT(manageProfiles()));

  contextMenu()->addSeparator();

  QAction* exitAction = contextMenu()->addAction("Exit");
  connect(exitAction, SIGNAL(triggered()), this, SLOT(exitProxyManager()));
}

QList<Profile> TrayIcon::loadProfiles() {
  QVariantList list = settings.value("profiles", QVariantList()).toList();
  QList<Profile> profiles;
  for (int i = 0; i < list.size(); i++) {
      QMap<QString, QVariant> serialized = list[i].toMap();
      Profile profile(serialized);
      profiles.append(profile);
  }
  return profiles;
}

void TrayIcon::saveProfiles(QList<Profile> &profiles) {
  QVariantList list;
  for (int i = 0; i < profiles.size(); i++) {
    list.append(profiles[i].serialize());
  }
  settings.setValue("profiles", list);
}

void TrayIcon::activateProfile(QAction* action) {
    if (action->data().type() == QVariant::Int) {
        QList<Profile> profiles = loadProfiles();
        Profile profile =  profiles[action->data().toInt()];
        activateProfile(profile);
        if (currentSignature != "") {
            rememberAssociation(currentSignature, profile.id);
        }
   }
}

void TrayIcon::activateProfile(const Profile& profile) {
    qDebug() << "activating: " << profile.name;
    qDebug() << "id: " << profile.id;
    qDebug() << "Host: " << profile.proxyHost;
    qDebug() << "Port: " << profile.proxyPort;
    qDebug() << "HostExceptions: " << profile.hostExceptions;
    qDebug() << "DomainExceptions: " << profile.domainExceptions;
    QDBusPendingReply<> reply = dbusInterface->ConfigureProxy(profile.useProxy, profile.proxyHost, profile.proxyPort, profile.hostExceptions, profile.domainExceptions);
    if (reply.isValid()) {
        qDebug() << "Valid";
    }
    else {
        qDebug() << QDBusError::errorString(reply.error().type());
    }

}

void TrayIcon::rememberAssociation(QString networkSignature, QString profileId) {
   QVariantMap associations = settings.value("associations", QVariantMap()).toMap();
   associations[networkSignature] = profileId;
   settings.setValue("associations", associations);
}


void TrayIcon::manageProfiles() {
  ProfileManager profileManager(loadProfiles());
  profileManager.setAttribute(Qt::WA_QuitOnClose, false);
  if (profileManager.exec() == QDialog::Accepted) {
      qDebug() << "Accepted: ";
      foreach (const Profile profile, profileManager.getProfiles()) {
          qDebug() << "profile: " << profile.name << "\n"
                   << "hostExceptions: " << profile.hostExceptions << "\n"
                   << "domainExceptions: " << profile.domainExceptions;
      }
    saveProfiles(profileManager.getProfiles());
  }
  makeContextMenu();
}


void TrayIcon::exitProxyManager() {
  exit(0);
}

void TrayIcon::networkChanged(QString newNetworkSignature) {
    qDebug() << "Ind i client networkChanged.., signature: " << newNetworkSignature;
    currentSignature = newNetworkSignature;
    if (newNetworkSignature == "") {
        return;
    }

    QVariantMap associations = settings.value("associations", QVariantMap()).toMap();

    QString profileId = associations.value(currentSignature).toString();
    QList<Profile> profiles = loadProfiles();
    for (int i = 0; i < profiles.size(); i++) {
        qDebug() << "Comparing " << profileId << " with " << profiles.at(i).id;
        if (profileId == profiles.at(i).id) {
            qDebug() << "Switching to " << profiles.at(i).name;
            activateProfile(profiles.at(i));
            menu.actions()[i]->setChecked(true);
            return;
        }
    }
}


