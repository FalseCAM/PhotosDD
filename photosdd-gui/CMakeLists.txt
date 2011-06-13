cmake_minimum_required(VERSION 2.8.1)

project(PHOTOSDD_GUI)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})

include_directories( ${PHOTOSDD_GUI_BINARY_DIR} ${QT_INCLUDE_DIR} )

set(PHOTOSDD_GUI_SRCS
    main.cpp
    mainwindow.cpp
	tipdialog.cpp
)

set(PHOTOSDD_GUI_HEADERS
	mainwindow.h
	tipdialog.h
)

set(PHOTOSDD_GUI_UIS
    mainwindow.ui
	tipdialog.ui
)

set( PHOTOSDD_GUI_RCS
       photosdd-gui.qrc
  )

# enable warnings
add_definitions( -Wall )

QT4_WRAP_CPP(PHOTOSDD_GUI_MOC_SRCS ${PHOTOSDD_GUI_HEADERS})
QT4_ADD_RESOURCES( PHOTOSDD_GUI_RC_SRCS ${PHOTOSDD_GUI_RCS} )
QT4_WRAP_UI(PHOTOSDD_GUI_UIS_H ${PHOTOSDD_GUI_UIS})

# Set some Win32 Specific Settings
IF(WIN32)
SET(GUI_TYPE WIN32)
ENDIF(WIN32)
# Set some Apple MacOS Specific settings
IF (APPLE)
SET(GUI_TYPE MACOSX_BUNDLE)
ENDIF (APPLE)

add_executable(photosdd-gui ${GUI_TYPE} ${PHOTOSDD_GUI_SRCS} ${PHOTOSDD_GUI_MOC_SRCS}
							${PHOTOSDD_GUI_RC_SRCS} ${PHOTOSDD_GUI_UIS_H})
target_link_libraries(photosdd-gui photosdd ${QT_LIBRARIES})