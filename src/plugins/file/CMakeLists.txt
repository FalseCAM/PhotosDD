cmake_minimum_required(VERSION 2.8.4)

project(FILE)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(FILE_SRCS
    file.cpp
)

set(FILE_HEADERS
	file.h
)

set(FILE_UIS
    file.ui
)

set( FILE_RCS
       file.qrc
  )

# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

QT4_WRAP_CPP(FILE_MOC_SRCS ${FILE_HEADERS})
QT4_ADD_RESOURCES( FILE_RC_SRCS ${FILE_RCS} )
QT4_WRAP_UI(FILE_UIS_H ${FILE_UIS})

add_library("file" SHARED ${FILE_SRCS} ${FILE_MOC_SRCS}
							${FILE_RC_SRCS} ${FILE_UIS_H})
target_link_libraries("file" photosdd ${QT_LIBRARIES})