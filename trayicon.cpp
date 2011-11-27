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

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent) {
    setIcon(QIcon(":icons/icon.png"));

    makeContextMenu();
    connect(contextMenu(), SIGNAL(triggered(QAction*)), this, SLOT(chooseProfile(QAction*)));
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
    }

    contextMenu()->addSeparator();

    QAction* manageProfileAction = contextMenu()->addAction("Manage profiles");
    connect(manageProfileAction, SIGNAL(triggered()), this, SLOT(manageProfiles()));

    contextMenu()->addSeparator();

    QAction* exitAction = contextMenu()->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitProxyManager()));
}

void TrayIcon::chooseProfile(QAction *action) {
    static QUrl proxySettingsUrl;
    if (action->data().type() == QVariant::String) {
        if (proxySettingsUrl.isEmpty()) {
            proxySettingsUrl = findLink("desktopservices:proxysetting");
        }
        if (proxySettingsUrl.isEmpty()) {
            notify("Error activating profile", "Could not retrieve proxysetting link from desktopservices");
            makeContextMenu();
        }
        else {
            JsonResource jsonResource(proxySettingsUrl);
            jsonResource.PUT(profileListModel.id2map(action->data().toString().toUtf8()));
            if (jsonResource.error) {
                notify("Error activating profile",  jsonResource.errorMsg);
                makeContextMenu();
            }
            else {
                notify("Profile activated", "Profile '" + action->text() + "' activated");
            }
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
