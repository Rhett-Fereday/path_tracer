CMAKE_MINIMUM_REQUIRED(VERSION 3.8 FATAL_ERROR)

project(path_tracer)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME})

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC src lib)