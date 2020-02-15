# static library
include_directories(AFTER include/)
include_directories(AFTER ../DearImGui/imgui/)
include_directories(AFTER ../DearImGui/imgui/examples/)

add_library(core STATIC 
                src/Mesh.cpp 
                src/Model.cpp 
                src/Shader.cpp 
                src/RenderContext.cpp 
                src/FrameBuffer.cpp
                src/VertexArray.cpp)
add_dependencies(core imgui)
target_link_libraries(core imgui)
# install libs
install(TARGETS core DESTINATION lib/)