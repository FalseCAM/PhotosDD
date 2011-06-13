cmake_minimum_required(VERSION 2.8.1)

project(ATOM)

set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTXML TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${ATOM_BINARY_DIR} ${QT_INCLUDE_DIR} )
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(ATOM_SRCS
    atom.cpp
)

set(ATOM_HEADERS
	atom.h
)

set(ATOM_UIS
    atom.ui
)

set( ATOM_RCS
       atom.qrc
  )

QT4_WRAP_CPP(ATOM_MOC_SRCS ${ATOM_HEADERS})
QT4_ADD_RESOURCES( ATOM_RC_SRCS ${ATOM_RCS} )
QT4_WRAP_UI(ATOM_UIS_H ${ATOM_UIS})

add_library(atom SHARED ${ATOM_SRCS} ${ATOM_MOC_SRCS}
							${ATOM_RC_SRCS} ${ATOM_UIS_H})
target_link_libraries(atom photosdd ${QT_LIBRARIES})