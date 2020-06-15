# static library
include_directories(AFTER .)
include_directories(AFTER ../DearImGui/imgui/)
include_directories(AFTER ../DearImGui/imgui/examples/)

add_library(core STATIC 
                Mesh.cpp 
                Model.cpp 
                Shader.cpp 
                RenderContext.cpp 
                FrameBuffer.cpp
                TextureManager.cpp
                Texture.cpp
                InputHandler.hpp
                Texture.hpp
                Mesh.hpp
                Singleton.hpp
                Buffer.hpp
                MeshManager.hpp
                FrameBuffer.hpp
                RenderContext.hpp
                RenderAbstraction.hpp
                DrowableObject.hpp
                DrowableObjManager.hpp
                Renderer.hpp
                Manager.hpp
                Camera.hpp
                ProgramState.hpp
                TextureManager.hpp
                types.hpp
                Model.hpp
                Shader.hpp
                ModelManager.hpp
)
add_dependencies(core imgui)
target_link_libraries(core imgui Qt5::Core)
# install libs
install(TARGETS core DESTINATION lib/)
