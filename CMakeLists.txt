#  Copyright 2011, 2012, 2012 Christian Surlykke
#
#  This file is part of ProxyManager
#
#  ProxyManager is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, version 3
#
#  ProxyManager is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with ProxyManager.  If not, see <http://www.gnu.org/licenses/>.
project( proxymanager )
cmake_minimum_required( VERSION 2.6 )
add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
include_directories (
    ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTDBUS_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR}
    )
set ( proxymanager_HDRS
    profile.h
    )

set (
    proxymanager_SRCS
    main.cpp
    trayicon.cpp
    profiledialog.cpp
    notifications.cpp
    )

set (
    proxymanager_UIS
    profilemanager.ui
    )
QT4_WRAP_UI(UIS ${proxymanager_UIS})

set (
    proxymanager_RSCS
    proxymanager.qrc
    )
QT4_ADD_RESOURCES(RSCS ${proxymanager_RSCS})

set (
    proxymanager_MOCS
    trayicon.h
    profiledialog.h
    notifications.h
    )
QT4_WRAP_CPP(MOCS ${proxymanager_MOCS})

add_executable ( proxymanager ${proxymanager_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( proxymanager  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTNETWORK_LIBRARY} )

install(TARGETS proxymanager DESTINATION bin CONFIGURATIONS Release)
install(FILES   icons/proxymanager.svg DESTINATION share/icons/hicolor/scalable/apps)
install(FILES   desktop/proxymanager.desktop DESTINATION share/applications)
