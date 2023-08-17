cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(UniEngine)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(UNIENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message("UniEngine root dir is ${CMAKE_CURRENT_SOURCE_DIR}")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

#Dependencies
set(UNIENGINE_DEPEND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/)
#using glfw : https://www.glfw.org/docs/latest/
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${UNIENGINE_DEPEND_DIR}/glfw)

#source code
set(SOURCES main.cpp)

#target
add_executable(hello_uniengine ${SOURCES})
target_link_libraries(hello_uniengine glfw)