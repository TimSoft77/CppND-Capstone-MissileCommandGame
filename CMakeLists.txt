cmake_minimum_required(VERSION 3.7)

project(SDL2Test)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

set(SDL2_DIR "C:\\Program Files (x86)\\SDL2-2.24.1\\x86_64-w64-mingw32\\*") # Remove for Linux
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(MissileCommandGame src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/airSpace.cpp)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES) # Remove quotes for Linux
target_link_libraries(MissileCommandGame ${SDL2_LIBRARIES})
