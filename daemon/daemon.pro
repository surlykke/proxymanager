# Copyright 2010, 2011 Christian Surlykke

# This file is part of ProxyManager.

# PitchTrainer is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3

# PitchTrainer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with PitchTrainer.  If not, see <http://www.gnu.org/licenses/>.
QT       += core

QT       -= gui

QT       += dbus

TARGET = proxymanagerd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    proxy_manager_daemon.cpp \
    dbus_adaptor.cpp

HEADERS += \
    proxy_manager_daemon.h \
    dbus_adaptor.h \
    proxy_manager_consts.h

OTHER_FILES += \
    proxy-manager \
    proxy_manager_interface.xml

target.path = /usr/local/bin

init.path = /etc/init.d
init.files = proxy-manager

INSTALLS += target init
