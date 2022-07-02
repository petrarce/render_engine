find_package(Qt5 COMPONENTS Core Gui)

return_if(NOT Qt5Core_FOUND)
return_if(NOT Qt5Gui_FOUND)

add_library(qt-helpers STATIC 
	EigenConversions.hpp)

target_link_libraries(qt-helpers PUBLIC 
	Qt5::Core
	Qt5::Gui)

target_include_directories(qt-helpers PUBLIC .)

