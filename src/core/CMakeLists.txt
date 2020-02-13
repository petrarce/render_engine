# static library
include_directories(AFTER include/)
add_library(core STATIC 
                src/Mesh.cpp 
                src/Model.cpp 
                src/Shader.cpp 
                src/RenderContext.cpp 
                src/FrameBuffer.cpp
                src/VertexArray.cpp)

# install libs
install(TARGETS core DESTINATION lib/)