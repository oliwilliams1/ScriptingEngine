cmake_minimum_required(VERSION 3.16)

project(SableScriptEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

# Enable /bigobj for MSVC
if(MSVC)
    add_compile_options(/bigobj)
endif()

# Include directories
include_directories("vendor")

# Add the exprtk library
file(GLOB exprtk_source "vendor/exprtk.hpp")
add_library(exprtk INTERFACE)
target_include_directories(exprtk INTERFACE "vendor")

# Build the executable
add_executable(SableScriptEngine ${SOURCES})

# Link the exprtk library to the executable
target_link_libraries(SableScriptEngine PRIVATE exprtk)