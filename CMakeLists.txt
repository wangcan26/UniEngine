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

#using Diligent Core: https://github.com/jakeloo/glfw-diligent/blob/master/CMakeLists.txt
add_subdirectory(${UNIENGINE_DEPEND_DIR}/DiligentCore)
set(UNI_COMPILE_OPTIONS -DUNICODE)

set(Diligent_LIBS Diligent-GraphicsEngine)
if(D3D11_SUPPORTED)
  set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsEngineD3D11-static)
endif()

if (D3D12_SUPPORTED)
  set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsEngineD3D12-static)
endif()

if (GL_SUPPORTED)
  message("Diligent support gl!")
  set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsEngineOpenGL-static)
endif()

if (VULKAN_SUPPORTED)
  set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsEngineVk-static)
endif()

if (METAL_SUPPORTED)
  set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsEngineMetal-static)
endif()

include_directories(${UNIENGINE_DEPEND_DIR}/glfw/include)
include_directories(${UNIENGINE_DEPEND_DIR}/NanoLog)

#source code
file(GLOB EXTERN_SOURCES ${UNIENGINE_ROOT_DIR}/externs/*.cpp)
file(GLOB DEPEND_LOG_SOURCES ${UNIENGINE_DEPEND_DIR}/NanoLog/*.cpp)
set(SOURCES ${EXTERN_SOURCES} ${DEPEND_LOG_SOURCES} main.cpp)
message("sources: ${SOURCES}")

#targetcd 
set(UNI_TARGET hello_uniengine)
add_executable(${UNI_TARGET} ${SOURCES})
target_compile_options(${UNI_TARGET} PRIVATE ${UNI_COMPILE_OPTIONS})
target_include_directories(${UNI_TARGET} PRIVATE ${UNIENGINE_DEPEND_DIR}/glfw/include ${UNIENGINE_DEPEND_DIR}/DiligentCore)
target_link_libraries(${UNI_TARGET} glfw ${Diligent_LIBS})
