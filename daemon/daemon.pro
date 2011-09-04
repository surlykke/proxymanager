#-------------------------------------------------
#
# Project created by QtCreator 2011-08-25T20:46:11
#
#-------------------------------------------------

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
    dbus_adaptor.h

OTHER_FILES += \
    init_script
