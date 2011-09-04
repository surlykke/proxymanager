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
    dbus_interface.cpp
HEADERS += profilelistmodel.h \
    trayicon.h \
    profilemanager.h \
    profile.h \
    dbus_interface.h
FORMS += profilemanager.ui
OTHER_FILES += icons/icon.svg
RESOURCES += \
    client.qrc
