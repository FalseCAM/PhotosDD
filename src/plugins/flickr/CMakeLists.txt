cmake_minimum_required(VERSION 2.8.4)

project(FLICKR)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTWEBKIT TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(FLICKR_SRCS
    flickr.cpp
)

set(FLICKR_HEADERS
	flickr.h
)

set(FLICKR_UIS
    flickr.ui
)

set( FLICKR_RCS
       flickr.qrc
  )

# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

QT4_WRAP_CPP(FLICKR_MOC_SRCS ${FLICKR_HEADERS})
QT4_ADD_RESOURCES( FLICKR_RC_SRCS ${FLICKR_RCS} )
QT4_WRAP_UI(FLICKR_UIS_H ${FLICKR_UIS})

add_library(flickr SHARED ${FLICKR_SRCS} ${FLICKR_MOC_SRCS}
							${FLICKR_RC_SRCS} ${FLICKR_UIS_H})
target_link_libraries(flickr photosdd ${QT_LIBRARIES})