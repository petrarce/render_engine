add_library(qt-helpers INTERFACE )
target_include_directories(qt-helpers INTERFACE .)
target_sources(qt-helpers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/EigenConversions.hpp)
