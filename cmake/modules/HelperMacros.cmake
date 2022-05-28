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
	message("ASSETS_TARGET=${ASSETS_TARGET}, ASSETS_ASSETFILES=${ASSETS_ASSETFILES}")
	if(NOT ASSETS_TARGET)
		message("target_deploy_assets: target was not provided")
		return()
	endif()

	if(NOT TARGET ${ASSETS_TARGET})
		message("target_deploy_assets: ${ASSETS_TARGET} is not a target")
		return()
	endif()

	foreach(ASSET ${ASSETS_ASSETFILES})
		message("ASSET=${ASSET}")
		add_custom_command(
			OUTPUT ${CMAKE_ASSETS_OUTPUT_DIRECTORY}/${ASSET}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET} ${CMAKE_ASSETS_OUTPUT_DIRECTORY})
		target_sources(${ASSETS_TARGET}
			PRIVATE ${CMAKE_ASSETS_OUTPUT_DIRECTORY}/${ASSET})
	endforeach()

endfunction()
