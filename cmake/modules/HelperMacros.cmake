macro(return_if)
	if( ${ARGV} )
		string(REPLACE ";" " " OUTMSG "${ARGV}")
		message("${OUTMSG}")
		return()
	endif()
endmacro()

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
	set(multiValArgs SOURCES LINK_LIBS QMLFILES)
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
		${QMLMOD_LINK_LIBS})

	set(QML_MODULE_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${QMLMOD_NAME})
	set_target_properties(${QMLMOD_NAME}-qmlmodule
		PROPERTIES
			LIBRARY_OUTPUT_DIRECTORY ${QML_MODULE_DIR}
		)

	foreach(ASSET ${QMLMOD_QMLFILES})
		list(APPEND SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}")
		list(APPEND DEST_LIST "${QML_MODULE_DIR}/${ASSET}")
	endforeach()
	list(APPEND SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/qmldir")
	list(APPEND DEST_LIST "${QMLMOD_QMLFILES}/qmldir")

	# Move qml and other resource files to modules directory
	add_custom_target(${QMLMOD_NAME}-qmlmodule-files
		COMMAND ${CMAKE_COMMAND} -E copy ${SRC_LIST} qmldir ${QML_MODULE_DIR}
		BYPRODUCTS ${DEST_LIST}
		COMMENT "Deploing assets for${QMLMOD_NAME}-qmlmodule-files"
		DEPENDS ${SRC_LIST}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		SOURCES ${SRC_LIST})
	add_dependencies(${QMLMOD_NAME}-qmlmodule
		${QMLMOD_NAME}-qmlmodule-files)

	# generate qml types information file
	#TODO: Segfault???
#	get_filename_component(QT_QMLPLUGINDUMP_EXECUTABLE "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
#	set(QT_QMLPLUGINDUMP_EXECUTABLE "${QT_QMLPLUGINDUMP_EXECUTABLE}/qmlplugindump")
#	if(EXISTS ${QT_QMLPLUGINDUMP_EXECUTABLE})
#		add_custom_command(TARGET ${QMLMOD_NAME}-qmlmodule POST_BUILD
#			COMMAND ${QT_QMLPLUGINDUMP_EXECUTABLE} ${QMLMOD_NAME} ${QMLMOD_VERSION} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} > ${CMAKE_CURRENT_SOURCE_DIR}/${QMLMOD_NAME}.qmltypes
#			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${QMLMOD_NAME}.qmltypes ${QML_MODULE_DIR}
#			)
#	else()
#		message("Cannot generate qmltypes file: ${QT_QMLPLUGINDUMP_EXECUTABLE} doesn't exists")
#	endif()
endfunction()
