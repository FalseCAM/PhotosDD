cmake_minimum_required(VERSION 2.8.4)

project(RSS)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(RSS_SRCS
    rss.cpp
)

set(RSS_HEADERS
	rss.h
)

set(RSS_UIS
    rss.ui
)

set( RSS_RCS
       rss.qrc
  )

# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

QT4_WRAP_CPP(RSS_MOC_SRCS ${RSS_HEADERS})
QT4_ADD_RESOURCES( RSS_RC_SRCS ${RSS_RCS} )
QT4_WRAP_UI(RSS_UIS_H ${RSS_UIS})

add_library(rss SHARED ${RSS_SRCS} ${RSS_MOC_SRCS}
							${RSS_RC_SRCS} ${RSS_UIS_H})
target_link_libraries(rss photosdd ${QT_LIBRARIES})