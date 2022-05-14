set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGL_ENABLE_THROW_ON_ERROR -fsanitize=address" CACHE STRING "Additional flags for CXX debug build" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DGL_ENABLE_THROW_ON_ERROR -fsanitize=address" CACHE STRING "Additional flags for CXX RelWIdhDebInfo" FORCE)

set(CMAKE_CXX_FLAGS_RELEASELSAN
	"-fsanitize=leak -g -O2" CACHE STRING "Releas with debug information with leak sanatizer" FORCE)
set(CMAKE_CXX_FLAGS_RELEASETSAN
	"-fsanitize=thread -g -O2" CACHE STRING "Releas with debug information with thread sanatizer" FORCE)
