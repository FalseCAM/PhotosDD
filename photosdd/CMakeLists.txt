cmake_minimum_required(VERSION 2.8.1)

project(PHOTOSDD_LIB)

find_package("Qt4" REQUIRED)
set(QT_USE_QTNETWORK TRUE)
include(${QT_USE_FILE})

include_directories( ${PHOTOSDD_LIB_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS( -D PHOTOSDD_LIBRARY )
# enable warnings
add_definitions( -Wall )

file(GLOB PHOTOSDD_LIB_SRCS "src/*.cpp")
file(GLOB PHOTOSDD_LIB_HEADERS "include/photosdd/*.h")

set(PHOTOSDD_LIB_UIS
    pluginsdialog.ui
)

set( PHOTOSDD_LIB_RCS
       photosdd.qrc
  )
  
QT4_WRAP_UI(PHOTOSDD_LIB_UIS_H ${PHOTOSDD_LIB_UIS})
QT4_ADD_RESOURCES( PHOTOSDD_LIB_RC_SRCS ${PHOTOSDD_LIB_RCS} )
QT4_WRAP_CPP(PHOTOSDD_LIB_MOC_SRCS ${PHOTOSDD_LIB_HEADERS})

add_library(photosdd SHARED ${PHOTOSDD_LIB_SRCS}
							${PHOTOSDD_LIB_MOC_SRCS}
							${PHOTOSDD_LIB_RC_SRCS}
							${PHOTOSDD_LIB_UIS_H})
target_link_libraries(photosdd ${QT_LIBRARIES})
