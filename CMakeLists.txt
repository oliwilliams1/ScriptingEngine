cmake_minimum_required(VERSION 3.16)

project(SableScriptEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Build the editor executable
add_executable(SableScriptEngine ${SOURCES})