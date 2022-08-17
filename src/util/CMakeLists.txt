
add_library(math-helpers INTERFACE)
target_include_directories(math-helpers INTERFACE .)
if(BUILD_TESTING)
	add_executable(test-util-helpers
		tests/TestUtilHelpers.cpp
	)

	target_link_libraries(test-util-helpers PUBLIC
		math-helpers
	)

	add_test(NAME test-util-helpers COMMAND test-util-helpers)
endif()
