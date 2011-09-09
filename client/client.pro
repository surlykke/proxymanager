# -------------------------------------------------
# Project created by QtCreator 2009-12-14T18:06:54
# -------------------------------------------------
QT += dbus
TARGET = pm_client
TEMPLATE = app
SOURCES += profilelistmodel.cpp \
    main.cpp \
    trayicon.cpp \
    profilemanager.cpp \
    profile.cpp \
    dbus_interface.cpp \
    notifications.cpp
HEADERS += profilelistmodel.h \
    trayicon.h \
    profilemanager.h \
    profile.h \
    dbus_interface.h \
    notifications.h
FORMS += profilemanager.ui
OTHER_FILES += icons/icon.svg
RESOURCES += \
    client.qrc

target.path = /usr/local/bin
INSTALLS += target
