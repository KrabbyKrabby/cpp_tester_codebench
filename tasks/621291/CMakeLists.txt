cmake_minimum_required(VERSION 3.14)
project(subset_sum_tests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GoogleTest via FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Collect all .cpp files from src/
file(GLOB IMPLEMENTATIONS "alternate_responses/*.cpp")

# Iterate through each implementation and generate tests for each
foreach(IMPLEMENTATION ${IMPLEMENTATIONS})
    # Extract the implementation name (e.g., A, B, ideal, etc.)
    get_filename_component(IMPL_NAME ${IMPLEMENTATION} NAME_WE)

    # Generate a unique wrapper file for each implementation
    set(WRAPPER_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/template_wrapper.h.in")
    set(WRAPPER_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/wrapper_${IMPL_NAME}.h")
    configure_file(${WRAPPER_TEMPLATE} ${WRAPPER_OUTPUT} @ONLY)

    # Add a test executable for each implementation
    add_executable(${IMPL_NAME}_tests test.cpp)

    # Link the implementation indirectly through the wrapper
    target_include_directories(${IMPL_NAME}_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_compile_definitions(${IMPL_NAME}_tests PRIVATE WRAPPER_FILE="${WRAPPER_OUTPUT}")

    # Link GoogleTest
    target_link_libraries(${IMPL_NAME}_tests GTest::gtest_main)

    # Register the test
    add_test(NAME ${IMPL_NAME}_tests COMMAND ${IMPL_NAME}_tests)
endforeach()
