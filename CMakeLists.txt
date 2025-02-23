cmake_minimum_required(VERSION 3.16)

project(SerraSript VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include source files from src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "src/*.hpp")

add_subdirectory("vendor/angelscript/angelscript/projects/cmake")
include_directories("vendor/angelscript/angelscript/include")

add_executable(SerraSript ${SOURCES})

target_link_libraries(SerraSript angelscript)