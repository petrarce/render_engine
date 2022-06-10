set(TMP_CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DGL_ENABLE_THROW_ON_ERROR -fsanitize=address"
	CACHE STRING "temp string for TMP_CMAKE_CXX_FLAGS_RELWITHDEBINFO")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${TMP_CMAKE_CXX_FLAGS_RELWITHDEBINFO}"
	CACHE STRING "Additional flags for CXX RelWIdhDebInfo" FORCE)

set(CMAKE_CXX_FLAGS_RELEASELSAN "-fsanitize=leak -g -O2 -DGL_ENABLE_THROW_ON_ERROR"
	CACHE STRING "Releas with debug information with leak sanatizer" FORCE)
set(CMAKE_CXX_FLAGS_RELEASETSAN "-fsanitize=thread -g -O2 -DGL_ENABLE_THROW_ON_ERROR"
	CACHE STRING "Releas with debug information with thread sanatizer" FORCE)
set(CMAKE_CXX_FLAGS_DEBUGCOVERAGE "-g -O0 -DGL_ENABLE_THROW_ON_ERROR -fprofile-arcs -ftest-coverage"
	CACHE STRING "Code coverage build configuration" FORCE)
set(CMAKE_INSTALL_ASSETSDIR "assets" CACHE STRING "directory where all assets are deployed")
