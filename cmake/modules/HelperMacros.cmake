function(return_if)
	if( ${ARGV} )
		string(REPLACE ";" " " OUTMSG "${ARGV}")
		message("${OUTMSG}")
		return()
	endif()
endfunction()

function(target_deploy_assets)
	set(options "")
	set(singleValArgs "TARGET")
	set(multiValArgs "ASSETFILES")
	cmake_parse_arguments(ASSETS
		"${options}"
		"${singleValArgs}"
		"${multiValArgs}"
		${ARGN})
	if(NOT ASSETS_TARGET)
		message("target_deploy_assets: target was not provided")
		return()
	endif()

	if(NOT TARGET ${ASSETS_TARGET})
		message("target_deploy_assets: ${ASSETS_TARGET} is not a target")
		return()
	endif()

	foreach(ASSET ${ASSETS_ASSETFILES})
		list(APPEND SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}")
		list(APPEND DEST_LIST "${CMAKE_ASSETS_OUTPUT_DIRECTORY}/${ASSET}")
	endforeach()

	add_custom_target(${ASSETS_TARGET}-assets
		COMMAND ${CMAKE_COMMAND} -E copy ${SRC_LIST} ${CMAKE_ASSETS_OUTPUT_DIRECTORY}
		BYPRODUCTS ${DEST_LIST}
		COMMENT "Deploing assets for ${ASSETS_TARGET}"
		DEPENDS ${SRC_LIST}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		SOURCES ${SRC_LIST})
	add_dependencies(${ASSETS_TARGET}
		${ASSETS_TARGET}-assets)

endfunction()

function(qt_add_qml_module)
	set(options "")
	set(singleValArgs NAME VERSION)
	set(multiValArgs SOURCES LINK_LIBS)
	cmake_parse_arguments(QMLMOD
		"${options}"
		"${singleValArgs}"
		"${multiValArgs}"
		${ARGN})
	find_package(Qt5 COMPONENTS Quick Qml Core)
	return_if(NOT Qt5Quick_FOUND)
	return_if(NOT Qt5Qml_FOUND)
	return_if(NOT Qt5Core_FOUND)
	
	add_library(${QMLMOD_NAME}-qmlmodule SHARED
		${QMLMOD_SOURCES})
	
	target_link_libraries(${QMLMOD_NAME}-qmlmodule PUBLIC 
		Qt5::Quick
		Qt5::Qml
		Qt5::Core
		${LINK_LIBS})
	
	
#	set_target_properties(${QMLMOD_NAME}-qmlmodule
#		PROPERTIES
#			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${QMLMOD_NAME}
#		)
endfunction()
