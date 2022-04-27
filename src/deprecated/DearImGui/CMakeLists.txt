# static library
include_directories(AFTER imgui/ imgui/examples/ /usr/include/SDL2/)
add_library(imgui STATIC 
                imgui/imgui.cpp 
                imgui/imgui_demo.cpp 
                imgui/imgui_draw.cpp 
                imgui/imgui_widgets.cpp
                imgui/examples/imgui_impl_sdl.cpp
                imgui/examples/imgui_impl_opengl3.cpp)

# install libs
install(TARGETS imgui DESTINATION lib/)