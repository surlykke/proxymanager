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

set ( proxymanager_SRCS
    profilelistmodel.cpp
    main.cpp
    trayicon.cpp
    profiledialog.cpp
    notifications.cpp
    profile.cpp
    )

set ( proxymanager_UIS
    profilemanager.ui
    )
QT4_WRAP_UI(UIS ${proxymanager_UIS})

set ( proxymanager_RSCS
    proxymanager.qrc
    )
QT4_ADD_RESOURCES(RSCS ${proxymanager_RSCS})

set ( proxymanager_MOCS
    profilelistmodel.h
    trayicon.h
    profiledialog.h
    notifications.h
    )
QT4_WRAP_CPP(MOCS ${proxymanager_MOCS})

add_executable ( proxymanager ${proxymanager_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( proxymanager  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTNETWORK_LIBRARY} )
