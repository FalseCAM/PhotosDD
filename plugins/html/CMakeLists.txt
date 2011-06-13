cmake_minimum_required(VERSION 2.8.4)

project(HTML)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTWEBKIT TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${HTML_BINARY_DIR} ${QT_INCLUDE_DIR} )
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(HTML_SRCS
    html.cpp
)

set(HTML_HEADERS
	html.h
)

set(HTML_UIS
    html.ui
)

set( HTML_RCS
       html.qrc
  )

QT4_WRAP_CPP(HTML_MOC_SRCS ${HTML_HEADERS})
QT4_ADD_RESOURCES( HTML_RC_SRCS ${HTML_RCS} )
QT4_WRAP_UI(HTML_UIS_H ${HTML_UIS})

add_library(html SHARED ${HTML_SRCS} ${HTML_MOC_SRCS}
							${HTML_RC_SRCS} ${HTML_UIS_H})
target_link_libraries(html photosdd ${QT_LIBRARIES})