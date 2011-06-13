cmake_minimum_required(VERSION 2.8.1)

project(PICASA)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTWEBKIT TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${PICASA_BINARY_DIR} ${QT_INCLUDE_DIR} )
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(PICASA_SRCS
    picasa.cpp
)

set(PICASA_HEADERS
	picasa.h
)

set(PICASA_UIS
    picasa.ui
)

set( PICASA_RCS
       picasa.qrc
  )

QT4_WRAP_CPP(PICASA_MOC_SRCS ${PICASA_HEADERS})
QT4_ADD_RESOURCES( PICASA_RC_SRCS ${PICASA_RCS} )
QT4_WRAP_UI(PICASA_UIS_H ${PICASA_UIS})

add_library(picasa SHARED ${PICASA_SRCS} ${PICASA_MOC_SRCS}
							${PICASA_RC_SRCS} ${PICASA_UIS_H})
target_link_libraries(picasa photosdd ${QT_LIBRARIES})