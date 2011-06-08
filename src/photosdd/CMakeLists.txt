cmake_minimum_required(VERSION 2.8.4)

project(PHOTOSDD_LIB)

set( QT_USE_QTNETWORK TRUE )
find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )

set(PHOTOSDD_LIB_SRCS
    downloader.cpp
    fileutils.cpp
	photosdd.cpp
	pluginloader.cpp
	pluginsdialog.cpp
)

set(PHOTOSDD_LIB_HEADERS
	downloader.h
    fileutils.h
	photosdd.h
	photosdd_global.h
	photosddplugin.h
	pluginloader.h
	pluginsdialog.h
)

set(PHOTOSDD_LIB_UIS
    pluginsdialog.ui
)

set( PHOTOSDD_LIB_RCS
       photosdd.qrc
  )

# enable warnings
add_definitions( -Wall )

QT4_WRAP_UI(PHOTOSDD_LIB_UIS_H ${PHOTOSDD_LIB_UIS})
QT4_ADD_RESOURCES( PHOTOSDD_LIB_RC_SRCS ${PHOTOSDD_LIB_RCS} )
QT4_WRAP_CPP(PHOTOSDD_LIB_MOC_SRCS ${PHOTOSDD_LIB_HEADERS})

add_library(photosdd SHARED ${PHOTOSDD_LIB_SRCS}
							${PHOTOSDD_LIB_MOC_SRCS}
							${PHOTOSDD_LIB_RC_SRCS}
							${PHOTOSDD_LIB_UIS_H})
target_link_libraries(photosdd ${QT_LIBRARIES})
#install(TARGETS photosdd DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin")
install(DIRECTORY "include/photosdd" DESTINATION include
		FILES_MATCHING PATTERN "*.h")
