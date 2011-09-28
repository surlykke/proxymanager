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
    makeContextMenu();
    notifications = new Notifications(QDBusConnection::sessionBus(), this);
    if (signatureReply.isValid()) {
        networkChanged(signatureReply.value());
    }
}

void TrayIcon::makeContextMenu() {
  contextMenu()->clear();
  QActionGroup* profileGroup = new QActionGroup(this);
  qDebug() << "makeContextMenu, selectedProfile: " << profileListModel.selectedProfile;
  for (int i = 0; i < profileListModel.profiles.size(); i++) {
    QAction* profileAction = menu.addAction(profileListModel.profiles[i].name);
    profileAction->setData(i);
    profileAction->setCheckable(true);
    profileGroup->addAction(profileAction);
    if (profileListModel.selectedProfile != 0 &&
        profileListModel.profiles[i].id == profileListModel.selectedProfile->id) {
        profileAction->setChecked(true);
    }
  }

  contextMenu()->addSeparator();

  QAction* manageProfileAction = contextMenu()->addAction("Manage profiles");
  connect(manageProfileAction, SIGNAL(triggered()), this, SLOT(manageProfiles()));

  contextMenu()->addSeparator();

  QAction* exitAction = contextMenu()->addAction("Exit");
  connect(exitAction, SIGNAL(triggered()), this, SLOT(exitProxyManager()));
}


void TrayIcon::activateProfile(QAction* action) {
    if (action->data().type() == QVariant::Int) {
        profileListModel.selectProfile(action->data().toInt());
        if (profileListModel.selectedProfile != 0) {
            activateProfile(*(profileListModel.selectedProfile));
            if (currentSignature != "") {
                rememberAssociation(currentSignature, profileListModel.selectedProfile->id);
            }
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
    if (!reply.isError()) {
        qDebug() << "Valid";
        QString summary("Proxyconfiguration changed");
        QString body = "Proxyprofile '" + profile.name + "' " + (profile.useProxy ? ("(" + profile.proxyHost + ":" + QString::number(profile.proxyPort) + ") ") : "") + "activated";
        notifications->Notify("Proxymanager client",
                              0,
                              "",
                              summary,
                              body,
                              QStringList(),
                              QMap<QString, QVariant>(),
                              100);
    }
    else {
        qDebug() << "Problem configuring proxy: " << QDBusError::errorString(reply.error().type());
    }

}

void TrayIcon::rememberAssociation(QString networkSignature, QString profileId) {
    QSettings settings;
    settings.beginGroup("associations");
    settings.setValue(networkSignature, profileId);
    settings.endGroup();
}

QString TrayIcon::profileId4signature(QString networkSignature) {
    QSettings settings;
    settings.beginGroup("associations");
    QString profileId = settings.value(networkSignature).toString();
    settings.endGroup();
    return profileId;
}

void TrayIcon::manageProfiles() {
    qDebug() << "Manage profiles..";
    qDebug() << "Profiles: " << profileListModel.profiles;
    ProfileManager profileManager(&profileListModel);
    profileManager.setAttribute(Qt::WA_QuitOnClose, false);
    if (profileManager.exec() == QDialog::Accepted) {
        profileListModel.selectProfile(currentProfileId);
        if (profileListModel.selectedProfile != 0) { // FIXME check if it actually changed
            activateProfile(*(profileListModel.selectedProfile));
        }
        makeContextMenu();
    }
}


void TrayIcon::exitProxyManager() {
    exit(0);
}

void TrayIcon::networkChanged(QString newNetworkSignature) {
    qDebug() << "Ind i client networkChanged.., signature: " << newNetworkSignature;
    currentSignature = newNetworkSignature;
    if (currentSignature == "") return;

    currentProfileId = profileId4signature(currentSignature);
    profileListModel.selectProfile(currentProfileId);
    if (profileListModel.selectedProfile != 0) {
        activateProfile(*(profileListModel.selectedProfile));
        makeContextMenu();
    }
}


