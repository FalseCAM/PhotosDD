	CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)
	
 	########### Project name ###########
 	PROJECT(PhotosDD)
 	SET(PhotosDD_MAJOR "0")
 	SET(PhotosDD_MINOR "2")
 	SET(PhotosDD_PATCH "1")
 	SET(VERSION "${PhotosDD_MAJOR}.${PhotosDD_MINOR}.${PhotosDD_PATCH}")
 	SET(PACKAGE photosdd)
   
  ########### Subdirectories ###############

	set(CMAKE_SKIP_RPATH true)
	set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )
	set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" )
	set( CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/bin" )
	include_directories("${PROJECT_SOURCE_DIR}/photosdd/include")
	
	IF(NOT CMAKE_BUILD_TYPE)
		SET(CMAKE_BUILD_TYPE Release CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
	ENDIF(NOT CMAKE_BUILD_TYPE)

	ADD_SUBDIRECTORY( photosdd )
	ADD_SUBDIRECTORY( photosdd_gui )
	ADD_SUBDIRECTORY( plugins )
	
	
	install(FILES "bin/libphotosdd.so"
			DESTINATION lib)
	install(FILES "bin/photosdd-gui"
			DESTINATION bin
			PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
	install(FILES "bin/plugins/libatom.so"
			DESTINATION "lib/photosdd/plugins")
	install(FILES "bin/plugins/libfile.so"
			DESTINATION "lib/photosdd/plugins")
	install(FILES "bin/plugins/libflickr.so"
			DESTINATION "lib/photosdd/plugins")
	install(FILES "bin/plugins/libfotocommunity.so"
			DESTINATION "lib/photosdd/plugins")
	install(FILES "bin/plugins/libhtml.so"
			DESTINATION "lib/photosdd/plugins")
	install(FILES "bin/plugins/libpicasa.so"
			DESTINATION "lib/photosdd/plugins")
	install(FILES "bin/plugins/librss.so"
			DESTINATION "lib/photosdd/plugins")
	install(DIRECTORY "photosdd/include/photosdd"
			DESTINATION "include"
			FILES_MATCHING PATTERN "*.h")
	install (FILES "data/photosdd.xpm"
			DESTINATION "share/pixmaps")
	install (FILES "data/photosdd.desktop"
			DESTINATION "share/applications")