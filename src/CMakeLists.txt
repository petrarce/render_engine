set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall")

add_subdirectory(core)
add_subdirectory(objects)
add_subdirectory(context-creators)
add_subdirectory(components)
add_subdirectory(renderer)
add_subdirectory(ui)
