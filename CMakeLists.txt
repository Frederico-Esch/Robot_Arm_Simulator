cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(braco)

add_executable(braco src/main.cpp src/Space.cpp src/Camera.cpp src/RotationalJoint.cpp src/Robot.cpp)
target_include_directories(braco PUBLIC ./include)
target_link_libraries(braco PUBLIC raylib)
set(proj braco)
