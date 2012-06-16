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
#include <QtGui/QApplication>
//#include "mainwindow.h"
#include <QtGui>
#include <QObject>
#include "trayicon.h"
#include <QDebug>
#include <QSettings>
#include <QFileInfo>
#include <QSystemTrayIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Surlykke IT");
    QCoreApplication::setOrganizationDomain("surlykke-it.dk");
    QCoreApplication::setApplicationName("ProxyManager");
    
    TrayIcon trayIcon;
    QObject::connect(&a, SIGNAL(aboutToQuit()), &trayIcon, SLOT(close()));

    trayIcon.show();
    return a.exec();
}
