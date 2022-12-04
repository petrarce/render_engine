find_package(GLFW3)
find_package(Vulkan)

return_if(NOT GLFW3_FOUND)
return_if(NOT Vulkan_FOUND)

add_executable(vulkan-initial
	vulkan_initial.cpp)

target_link_libraries(vulkan-initial
	PRIVATE
		${GLFW3_LIBRARY}
		Vulkan::Vulkan)
target_compile_options(vulkan-initial PRIVATE -g -O0)
