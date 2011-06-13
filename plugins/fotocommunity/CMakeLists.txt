cmake_minimum_required(VERSION 2.8.4)

project(FOTOCOMMUNITY)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTWEBKIT TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${FOTOCOMMUNITY_BINARY_DIR} ${QT_INCLUDE_DIR} )
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(FOTOCOMMUNITY_SRCS
    fotocommunity.cpp
)

set(FOTOCOMMUNITY_HEADERS
	fotocommunity.h
)

set(FOTOCOMMUNITY_UIS
    fotocommunity.ui
)

set( FOTOCOMMUNITY_RCS
       fotocommunity.qrc
  )

QT4_WRAP_CPP(FOTOCOMMUNITY_MOC_SRCS ${FOTOCOMMUNITY_HEADERS})
QT4_ADD_RESOURCES( FOTOCOMMUNITY_RC_SRCS ${FOTOCOMMUNITY_RCS} )
QT4_WRAP_UI(FOTOCOMMUNITY_UIS_H ${FOTOCOMMUNITY_UIS})

add_library(fotocommunity SHARED ${FOTOCOMMUNITY_SRCS} ${FOTOCOMMUNITY_MOC_SRCS}
							${FOTOCOMMUNITY_RC_SRCS} ${FOTOCOMMUNITY_UIS_H})
target_link_libraries(fotocommunity photosdd ${QT_LIBRARIES})