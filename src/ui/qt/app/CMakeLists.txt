find_package(Qt5 COMPONENTS Qml Quick Gui Core)

return_if(NOT Qt5Quick_FOUND)
return_if(NOT Qt5Gui_FOUND)
return_if(NOT Qt5Core_FOUND)
return_if(NOT Qt5Qml_FOUND)

add_executable(qmlapp 
	qmlapp.cpp)

target_link_libraries(qmlapp PUBLIC 
	Qt5::Core
	Qt5::Quick
	Qt5::Qml)

