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

    TrayIcon trayIcon;
    trayIcon.show();

    return a.exec();
}
