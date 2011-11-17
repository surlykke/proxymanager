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
    if (action->data().type() == QVariant::String) {
        JsonResource jsonResource("http://localhost:8000/proxysetting/current");
        jsonResource.PUT(action->data().toString().toUtf8());
        if (jsonResource.error) {
            QString summary("Error activating '" + action->text() + "'");
            Notifications(QDBusConnection::sessionBus(), this).Notify("Proxymanager client", 0, "", summary, jsonResource.errorMsg, QStringList(), QMap<QString, QVariant>(), 2000);
            makeContextMenu();
        }
        else {
            Notifications(QDBusConnection::sessionBus(), this).Notify("Proxymanager client", 0, "", "Profile activated", "Profile '" + action->text() + "' activated",  QStringList(), QMap<QString, QVariant>(), 2000);
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


