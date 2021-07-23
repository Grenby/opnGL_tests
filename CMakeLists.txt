cmake_minimum_required(VERSION 3.17)
project(opnGL_tests)

set(CMAKE_CXX_STANDARD 14)

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

if(NOT glm_FOUND)
    message(Error "GLM not found")
endif(NOT glm_FOUND)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE INCLUDE RELATIVE ${CMAKE_SOURCE_DIR} "includes/*.h")
file(GLOB_RECURSE CORE RELATIVE ${CMAKE_SOURCE_DIR} "core/*.cpp")

add_executable(openGLTest main.cpp ${INCLUDE} ${CORE})
add_executable(lighting light2.cpp ${INCLUDE} ${CORE})


target_link_libraries(openGLTest ${GLM_LIBRARIES} GL SDL2 SOIL)
target_link_libraries(lighting ${GLM_LIBRARIES} GL SDL2 SOIL)
