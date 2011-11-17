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
QT += dbus xml network
TARGET = pm_client
TEMPLATE = app
SOURCES += profilelistmodel.cpp \
    main.cpp \
    trayicon.cpp \
    profiledialog.cpp \
    jsonresource.cpp \
    notifications.cpp
HEADERS += profilelistmodel.h \
    trayicon.h \
    profiledialog.h \
    jsonresource.h \
    notifications.h
FORMS += profilemanager.ui
OTHER_FILES += icons/icon.png
RESOURCES += \
    pm_client.qrc
LIBS += -L/usr/lib/ -lqjson
target.path = /usr/local/bin
INSTALLS += target


