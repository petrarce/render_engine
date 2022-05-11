set_source_files_properties(glad.c PROPERTIES LANGUAGE CXX )

add_library(glad STATIC glad.c)

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
