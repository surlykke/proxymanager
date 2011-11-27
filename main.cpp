#include <QtGui/QApplication>
//#include "mainwindow.h"
#include <QtGui>
#include <QObject>
#include "trayicon.h"
#include <QDebug>
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

    qDebug() << "Dir: " << ProfileListModel::settingsDir.absolutePath();
    QString settingsDirPath =  QDir::homePath() + "/.pm_client/settings";
    ProfileListModel::settingsDir.mkpath(settingsDirPath);
    ProfileListModel::settingsDir.setPath(settingsDirPath);
    TrayIcon trayIcon;

    trayIcon.show();

    return a.exec();
}
