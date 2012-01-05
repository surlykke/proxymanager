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
#include "qjson/parser.h"
#include "notifications.h"
#include <QFileSystemWatcher>
#include <QFile>

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent) {
    setIcon(QIcon(":icons/icon.png"));

    makeContextMenu();
    connect(contextMenu(), SIGNAL(triggered(QAction*)), this, SLOT(chooseProfile(QAction*)));

    // Force setup
    resolvconfChanged();

    // Monitor /etc/resolv.conf to discover when network changes
    resolvconfWatcher.addPath("/etc/resolv.conf");
    connect(&resolvconfWatcher, SIGNAL(fileChanged(QString)), this, SLOT(resolvconfChanged()));
}

void TrayIcon::close() {
    qDebug() << "Så er vi her..";
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
    for(int row = 0; row < profileListModel.rowCount(); row++) {
        QAction* profileAction = contextMenu()->addAction(profileListModel.name(row));
        profileAction->setData(profileListModel.id(row));
        profileAction->setCheckable(true);
        profileGroup->addAction(profileAction);
        if (profileAction->data() == currentProfileId) {
            profileAction->setChecked(true);
        }
    }

    contextMenu()->addSeparator();

    QAction* manageProfileAction = contextMenu()->addAction("Manage...");
    connect(manageProfileAction, SIGNAL(triggered()), this, SLOT(manageProfiles()));

    contextMenu()->addSeparator();

    QAction* exitAction = contextMenu()->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitProxyManager()));
}

void TrayIcon::chooseProfile(QAction *action) {
    if (action->data().type() == QVariant::String) {
        QString profileId = action->data().toString();
        QSettings settings;
        settings.beginGroup("associations");
        settings.setValue(networkSignature(), profileId);
        settings.endGroup();
        activateProfile(profileId);
    }
}

void TrayIcon::activateProfile(QString profileId) {
    qDebug() << cntlmProcess.readAllStandardError();
    if (cntlmProcess.state() != QProcess::NotRunning) {
        cntlmProcess.kill();
    }

    Profile profile = profileListModel.id2profile(profileId);
    QStringList args;
    args << "-f" << "-v" << "-l" << "7938";
    if (profile.useAuthentication) {
        args << "-u" << (profile.userId + "@" + profile.ntDomain) << "-p" << profile.password;
    }
    if (profile.useProxy) {
        if (profile.exceptions.trimmed().size() > 0) {
            args << "-N" << profile.exceptions;
        }
        args << profile.host << QString::number(profile.port);
    }
    else {
        args << "-N" << "*";
        args << "dummy" << "1234";
    }
    cntlmProcess.setReadChannel(QProcess::StandardError);
    cntlmProcess.start("cntlm", args);
}

void TrayIcon::writeForProtocol(QVariantMap& profile, QString protocol, QTextStream& textStream) {
    QString key = protocol + "Proxy";
    qDebug() << "Kigger efter: " << key;
    qDebug() << "profile.contains(\"" << key << "\"): " << profile.contains(key);
    qDebug() << "useProxy: " << profile.value(key).toMap().value("useProxy").toBool();
    if (profile.contains(key) && profile.value(key).toMap().value("useProxy").toBool()) {
            textStream << "ProxyRemote "
                       << protocol << " "
                       << profile.value(key).toMap().value("host").toString()
                       << ":"
                       << profile.value(key).toMap().value("port").toString()
                       << "\n";
    }
}




void TrayIcon::manageProfiles() {
    QString selectedId;
    for (int i = 0; i < contextMenu()->actions().size(); i++) {
        if (contextMenu()->actions().at(i)->isChecked()) {
            selectedId = contextMenu()->actions().at(i)->data().toString();
            break;
        }
    }

    ProfileDialog profileManager(selectedId, &profileListModel);
    profileManager.setAttribute(Qt::WA_QuitOnClose, false);
    profileManager.exec();
    makeContextMenu();
}


void TrayIcon::exitProxyManager() {
    QCoreApplication::exit(0);
}


void TrayIcon::notify(QString summary, QString message) {
    Notifications(QDBusConnection::sessionBus(), this).Notify("Proxymanager client", 0, "", summary, message,  QStringList(), QMap<QString, QVariant>(), 2000);
}

void TrayIcon::resolvconfChanged() {
    currentNetworkSignature = "INVALID";
    QSettings settings;
    settings.beginGroup("associations");
    QString id = settings.value(networkSignature()).toString();
    if (profileListModel.exists(id)) {
        activateProfile(id);
        makeContextMenu();
    }
    else if (id != "") {
        settings.remove(networkSignature());
    }
    settings.endGroup();
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

