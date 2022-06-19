set(CMAKE_CXX_FLAGS_RELEASEASAN "${CMAKE_CXX_FLAGS_RELEASE} -g -DGL_ENABLE_THROW_ON_ERROR -fsanitize=address"
	CACHE STRING "Releas with debug information with leak sanatizer" FORCE)
set(CMAKE_CXX_FLAGS_RELEASELSAN "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=leak -g -O2 -DGL_ENABLE_THROW_ON_ERROR"
	CACHE STRING "Releas with debug information with leak sanatizer" FORCE)
set(CMAKE_CXX_FLAGS_RELEASETSAN "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=thread -g -O2 -DGL_ENABLE_THROW_ON_ERROR"
	CACHE STRING "Releas with debug information with thread sanatizer" FORCE)
set(CMAKE_CXX_FLAGS_DEBUGCOVERAGE "-g -O0 -DGL_ENABLE_THROW_ON_ERROR -fprofile-arcs -ftest-coverage"
	CACHE STRING "Code coverage build configuration" FORCE)
set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -g -fno-omit-frame-pointer" CACHE STRING "Compiler flags for profiling")

set(CMAKE_CXX_STANDARD 17 CACHE INT "C++ standard version")

set(CMAKE_INSTALL_ASSETSDIR "assets" CACHE STRING "directory where all assets are deployed")
