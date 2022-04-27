add_library(opengl-wrapper-core STATIC
	GLObjectBinder.hpp
	GLObjectBinder.cpp
	GLObject.hpp
	GLBuffer.hpp
	GLBuffer.cpp
	GLArrayBuffer.hpp
	GLVertexArray.hpp
	GLElementArrayBuffer.hpp
)

target_link_libraries(opengl-wrapper-core PUBLIC glad)
target_include_directories(opengl-wrapper-core PUBLIC .)