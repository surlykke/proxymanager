#include <QtGui/QApplication>
//#include "mainwindow.h"
#include <QtGui>
#include <QObject>
#include "trayicon.h"
#include <QDebug>
#include <QSettings>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0,
                              QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }
    QCoreApplication::setOrganizationName("Surlykke IT");
    QCoreApplication::setOrganizationDomain("surlykke-it.dk");
    QCoreApplication::setApplicationName("ProxyManager");

    QString settingsDir =  QFileInfo(QSettings().fileName()).dir().absolutePath();
    TrayIcon::settingsDir.mkpath(settingsDir);
    TrayIcon::settingsDir.cd(settingsDir);

    QString profilesDir = settingsDir + "/proxymanager_profiles";
    ProfileListModel::profilesDir.mkpath(profilesDir);
    ProfileListModel::profilesDir.cd(profilesDir);

    qDebug() << "settingsDir: " << TrayIcon::settingsDir.absolutePath();
    qDebug() << "profilesdir: " << ProfileListModel::profilesDir.absolutePath();

    TrayIcon trayIcon;
    trayIcon.show();

    return a.exec();
}
