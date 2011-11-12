#include "trayicon.h"
#include <QEvent>
#include <QMap>
#include <QVariant>
#include <QString>
#include <QDir>
#include <QFile>
#include <QDBusConnection>
#include <QDBusPendingReply>
#include <QVariantMap>
#include <qjson/serializer.h>
#include <qjson/parser.h>

TrayIcon::TrayIcon(QWidget *parent) : QSystemTrayIcon(parent) {
    setIcon(QIcon(":icons/icon.png"));
    makeContextMenu();
}

void TrayIcon::makeContextMenu() {
    if (contextMenu() != 0) {
        delete(contextMenu());
        setContextMenu(0);
    }

    QMenu *menu = new QMenu();
    QActionGroup* profileGroup = new QActionGroup(this);
    for(int row = 0; row < profileListModel.rowCount(); row++) {
        QAction* profileAction = menu->addAction(profileListModel.name(row));
        profileAction->setData(profileListModel.id(row));
        profileAction->setCheckable(true);
        profileGroup->addAction(profileAction);
    }

    menu->addSeparator();

    QAction* manageProfileAction = menu->addAction("Manage profiles");
    connect(manageProfileAction, SIGNAL(triggered()), this, SLOT(manageProfiles()));

    menu->addSeparator();

    QAction* exitAction = menu->addAction("Exit");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitProxyManager()));
    setContextMenu(menu);
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


