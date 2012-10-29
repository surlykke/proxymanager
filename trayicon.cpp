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
#include "trayicon.h"
#include <QEvent>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QVariantMap>
#include <QMessageBox>
#include <QDBusInterface>
#include <QVariantList>
#include <QVariantMap>
#include <QProcess>
#include "notifications.h"
#include <QFileSystemWatcher>
#include <QFile>
#include <QSettings>


TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent), notificationId(0) {
    setIcon(QIcon(":icons/proxymanager.png"));

    makeContextMenu();
    //connect(contextMenu(), SIGNAL(triggered(QAction*)), this, SLOT(chooseProfile(QAction*)));

    // Force setup
    resolvconfChanged();

    // Monitor /etc/resolv.conf to discover when network changes
    resolvconfWatcher.addPath("/etc/resolv.conf");
    connect(&resolvconfWatcher, SIGNAL(fileChanged(QString)), this, SLOT(resolvconfChanged()));
}

void TrayIcon::close() {
    cntlmProcess.close();
}

void TrayIcon::makeContextMenu() {
    if (contextMenu() == 0) {
        setContextMenu(new QMenu());
    }
    else {
        contextMenu()->clear();
    }

    QActionGroup* profileGroup = new QActionGroup(this);
    QSettings settings;
    settings.beginGroup("profiles");
    foreach (QString profileName, settings.childGroups()) {
        QAction* profileAction = contextMenu()->addAction(profileName);
        profileAction->setCheckable(true);
        profileAction->setChecked(profileName == currentProfileName);
        profileGroup->addAction(profileAction);
    }
    connect(profileGroup, SIGNAL(triggered(QAction*)), this, SLOT(chooseProfile(QAction*)));

    contextMenu()->addSeparator();

    QAction* manageProfileAction = contextMenu()->addAction("Manage...");
    connect(manageProfileAction, SIGNAL(triggered()), this, SLOT(manageProfiles()));

    contextMenu()->addSeparator();

    QAction* exitAction = contextMenu()->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitProxyManager()));
}

void TrayIcon::chooseProfile(QAction *action) {
    Associations().setAssociation(networkSignature(), action->text());
    activateProfile(action->text());
}

void TrayIcon::activateProfile(QString profileName ) {
    qDebug() << "activateProfile: " << profileName;
    if (cntlmProcess.state() != QProcess::NotRunning) {
        cntlmProcess.close();
    }

    QSettings settings;
    settings.beginGroup("profiles");
    settings.beginGroup(profileName);

    QStringList args;
    args << "-f" << "-v" << "-l" << "7938";
    bool useAuthentication = settings.value(USEAUTHENTICATION).toBool();
    if (useAuthentication) {
        QString userId = settings.value(USERID).toString();
        QString ntDomain = settings.value(NTDOMAIN).toString();
        QString passWord = settings.value(PASSWORD).toString();
        args << "-u" << ( userId + "@" + ntDomain) << "-p" << passWord;
    }

    bool useProxy = settings.value(USEPROXY).toBool();
    if (useProxy) {
        QString exceptions =settings.value(EXCEPTIONS).toString();
        if (exceptions.trimmed().size() > 0) {
            args << "-N" << exceptions;
        }

        QString host = settings.value(HOST).toString();
        QString port = settings.value(PORT).toString();
        args << host << port;
    }
    else {
        args << "-N" << "*";
        args << "dummy" << "1234";
    }
    cntlmProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    cntlmProcess.start("cntlm", args);
    currentProfileName = profileName;
    notify("Proxy settings changed", "Proxy profile '" + currentProfileName + "' activated");
    makeContextMenu();
    setToolTip("Proxy profile '" + currentProfileName + "' active.");
}


void TrayIcon::manageProfiles() {
    qDebug() << "manageProfiles..";
    QString selectedId;
    for (int i = 0; i < contextMenu()->actions().size(); i++) {
        if (contextMenu()->actions().at(i)->isChecked()) {
            selectedId = contextMenu()->actions().at(i)->text();
            break;
        }
    }

    ProfileDialog profileManager(selectedId);
    profileManager.setAttribute(Qt::WA_QuitOnClose, false);
    profileManager.exec();
    makeContextMenu();
    activateProfile(currentProfileName);
}


void TrayIcon::exitProxyManager() {
    QCoreApplication::exit(0);
}


void TrayIcon::notify(QString summary, QString message) {
    Notifications n(QDBusConnection::sessionBus(), this);
    QDBusPendingReply<uint> reply = n.Notify("Proxymanager client", notificationId, "proxymanager", summary, message,  QStringList(), QMap<QString, QVariant>(), 2000);
    notificationId = reply.value();
}

void TrayIcon::resolvconfChanged() {
    currentNetworkSignature = "INVALID";
    QString profileName = Associations().getAssociation(networkSignature());

    QSettings settings;
    settings.beginGroup("profiles");
    QStringList profiles = settings.childGroups();
    settings.endGroup();
    if (profiles.contains(profileName)) {
        activateProfile(profileName);
        makeContextMenu();
    }
    else {
        Associations().removeAssociation(networkSignature());
    }

    // If the file has been removed, QFileSystemWatcher stops watching it...
    if (! resolvconfWatcher.files().contains("/etc/resolv.conf")) {
        resolvconfWatcher.addPath("/etc/resolv.conf");
    }
}

QString TrayIcon::networkSignature() {
    if (currentNetworkSignature == "INVALID") {
        currentNetworkSignature = "";
        QFile resolvConf("/etc/resolv.conf");
        if (resolvConf.exists() && resolvConf.open(QIODevice::ReadOnly)) {
            QTextStream stream(&resolvConf);
            QCryptographicHash hash(QCryptographicHash::Md5);
            bool foundSome = false;
            while (! stream.atEnd()) {
                QString line = stream.readLine();
                if (notWhiteSpace(line)) {
                    hash.addData(line.toAscii());
                    foundSome = true;
                }
            }
            if (foundSome) {
                currentNetworkSignature =  hash.result().toHex();
            }
        }
    }
    return currentNetworkSignature;
}

bool TrayIcon::notWhiteSpace(QString& string){
    static QRegExp reg("^\\s*#.*$|^\\s*$");
    return ! reg.exactMatch(string);
}



Associations::Associations()
{
    beginGroup("associations");
}

QString Associations::getAssociation(QString networkSignature)
{
    return value(networkSignature).toString();
}

void Associations::setAssociation(QString networkSignature, QString profileName)
{
    setValue(networkSignature, profileName);
}

void Associations::removeAssociation(QString networkSignature)
{
    if ("" != networkSignature.trimmed())
    {
        remove(networkSignature);
    }
}
