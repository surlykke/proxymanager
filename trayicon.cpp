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
#include "jsonresource.h"
#include "notifications.h"
#include <QFileSystemWatcher>

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
    qDebug() << "activating: " + profileId;
    static QUrl proxySettingsUrl;
    if (proxySettingsUrl.isEmpty()) {
        proxySettingsUrl = findLink("desktopservices:proxysetting");
    }
    if (proxySettingsUrl.isEmpty()) {
        notify("Error activating profile", "Could not retrieve proxysetting link from desktopservices");
        makeContextMenu();
    }
    else {
        JsonResource jsonResource(proxySettingsUrl);
        QVariantMap map = profileListModel.id2map(profileId);
        jsonResource.PUT(map);
        if (jsonResource.error) {
            notify("Error activating profile",  jsonResource.errorMsg);
            makeContextMenu();
        }
        else {
            currentProfileId = profileId;
            notify("Proxy settings changed", "Proxy setting '" + map.value("name").toString() + "' activated");
        }
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
    exit(0);
}


QUrl TrayIcon::findLink(QString relation) {
    static QUrl baseUrl("http://localhost:8000/desktopservices");
    JsonResource jsonResource(baseUrl);
    jsonResource.GET();
    if (!jsonResource.error) {
        QJson::Parser parser;
        QVariantList links =  jsonResource.response.toMap()["links"].toList();
        qDebug() << "findLink modtog: " << links;
        for (int i = 0; i < links.size(); i++) {
            QVariantMap link = links.at(i).toMap();
            qDebug() << "Undersøger: " << link;
            qDebug() << "Sammenligner " << relation << " med " << link["rel"].toString();
            if (relation == link["rel"].toString()) {
                QUrl resolvedUrl = baseUrl.resolved(QUrl(link["href"].toString()));
                qDebug() << "resolvedUrl: " << resolvedUrl.toString();
                return resolvedUrl;
            }
        }
    }
    return QUrl();
}


void TrayIcon::notify(QString summary, QString message) {
    Notifications(QDBusConnection::sessionBus(), this).Notify("Proxymanager client", 0, "", summary, message,  QStringList(), QMap<QString, QVariant>(), 2000);
}

void TrayIcon::resolvconfChanged() {
    currentNetworkSignature = "INVALID";
    QSettings settings;
    settings.beginGroup("associations");
    qDebug() << "I resolvconfChanged, networkSignature: " << networkSignature();
    QString id = settings.value(networkSignature()).toString();
    qDebug() << "id: " << id;
    if (profileListModel.exists(id)) {
        activateProfile(id);
        makeContextMenu();
    }
    else if (id != "") {
        qDebug() << "Fjerner: " << networkSignature();
        settings.remove(networkSignature());
    }
    settings.endGroup();
    qDebug() << "resolvconf changed";
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
                qDebug() << "Reading " << line;
                if (notWhiteSpace(line)) {
                    qDebug() << "Adding";
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
