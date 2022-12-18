cmake_minimum_required(VERSION 3.24)
project(bright_star_catalog_cpp LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
        bright_star_catalog_cpp
        src/bright_star_catalog_parser.cpp
        src/binary_reader.cpp)

target_include_directories(bright_star_catalog_cpp PUBLIC include/)
# A CMake pattern to have headers that are not seen by the consumers of this library.
# So headers which not have to be visible for consumers should be placed in src/
target_include_directories(bright_star_catalog_cpp PRIVATE src ${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
    # PUBLIC = use this compile options also while compiling the project simulation-engine
    # warning level 4 and all warnings as errors
    target_compile_options(bright_star_catalog_cpp PUBLIC /W4 /WX)
else()
    # PUBLIC = use this compile options also while compiling the project simulation-engine
    # lots of warnings and all warnings as errors
    target_compile_options(bright_star_catalog_cpp PUBLIC -Wall -Wextra -pedantic -Werror)
endif()

set(RESOURCES_FOLDER_PATH "${PROJECT_SOURCE_DIR}/res/")
configure_file(config.h.in config.h @ONLY)

# Enable testing
enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit tests as an executable of this library
add_executable(bright_star_catalog_cpp_tests test/bright_star_catalog_parser_test.cpp)
target_link_libraries(bright_star_catalog_cpp_tests gtest_main bright_star_catalog_cpp)

add_test(NAME bright_star_catalog_cpp_tests COMMAND bright_star_catalog_cpp_tests WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/test/")
