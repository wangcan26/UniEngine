cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(UniEngine)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(UNIENGINE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message("UniEngine root dir is ${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(PLATFORM_WIN32             FALSE CACHE INTERNAL "")
set(PLATFORM_ANDROID           FALSE CACHE INTERNAL "")
set(PLATFORM_MACOS             FALSE CACHE INTERNAL "")

if(WIN32)
else()
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_definitions(-DUNI_PLATFORM_ANDROID=1)
    set(PLATFORM_ANDROID TRUE)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_definitions(-DUNI_PLATFORM_MACOS=1)
    set(PLATFORM_MACOS TRUE)
  else()
    message(FATAL_ERROR "Unsupported platform")
  endif()
endif(WIN32)

set(UNIENGINE_DEPEND_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/)
set(UNIENGINE_EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externs/)
#Dependencies
if(NOT PLATFORM_ANDROID)
  #using glfw : https://www.glfw.org/docs/latest/
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

  add_subdirectory(${UNIENGINE_DEPEND_DIR}/glfw)
endif()

#using Diligent Core: https://github.com/jakeloo/glfw-diligent/blob/master/CMakeLists.txt
add_subdirectory(${UNIENGINE_DEPEND_DIR}/DiligentCore)
set(UNI_COMPILE_OPTIONS -DUNICODE)

set(Diligent_LIBS Diligent-GraphicsEngine)
if(NOT PLATFORM_ANDROID)
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
else()
  if (GLES_SUPPORTED)
    set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsEngineOpenGL-static)
  endif()
endif()

set(Diligent_LIBS ${Diligent_LIBS} Diligent-GraphicsTools Diligent-TargetPlatform)

#using FrameGraph
add_definitions(-D_HAS_CXX20=0)
set(FG_BUILD_TEST OFF)
add_subdirectory(third_party/FrameGraph)

#include headers
if(NOT PLATFORM_ANDROID)
  include_directories(${UNIENGINE_DEPEND_DIR}/glfw/include)
endif()
include_directories(${UNIENGINE_DEPEND_DIR}/NanoLog)
include_directories(${UNIENGINE_EXTERN_DIR}/)
include_directories(${UNIENGINE_DEPEND_DIR}/DiligentCore/Graphics/GraphicsTools/interface)
include_directories(${UNIENGINE_DEPEND_DIR}/FrameGraph/include)

#source code
file(GLOB EXTERN_SOURCES_APP ${UNIENGINE_ROOT_DIR}/externs/app/*.cpp)
file(GLOB DEPEND_LOG_SOURCES ${UNIENGINE_DEPEND_DIR}/NanoLog/*.cpp)


if(NOT PLATFORM_ANDROID)
  set(UNI_MAIN_SOURCES ${UNIENGINE_ROOT_DIR}/externs/main/main.cpp)
else()
endif()

set(UNI_SOURCES ${EXTERN_SOURCES_APP} ${DEPEND_LOG_SOURCES} ${UNI_MAIN_SOURCES})
message("sources: ${SOURCES}")

#targets
if(NOT PLATFORM_ANDROID)
  #sample_triangle 
  set(UNI_TARGET sample_triangle)
  file(GLOB SOURCES_SAMPLE_TRIANGLE ${UNIENGINE_ROOT_DIR}/externs/samples/triangle/*.cpp)
  set(SOURCES ${UNI_SOURCES} ${SOURCES_SAMPLE_TRIANGLE})
  add_executable(${UNI_TARGET} ${SOURCES})
  target_compile_options(${UNI_TARGET} PRIVATE ${UNI_COMPILE_OPTIONS})
  target_link_libraries(${UNI_TARGET} glfw ${Diligent_LIBS} fg::FrameGraph)

  #sample_buffers
  set(UNI_TARGET sample_buffers)
  file(GLOB SOURCES_SAMPLE_BUFFERS ${UNIENGINE_ROOT_DIR}/externs/samples/buffers/*.cpp)
  set(SOURCES ${UNI_SOURCES} ${SOURCES_SAMPLE_BUFFERS})
  add_executable(${UNI_TARGET} ${SOURCES})
  target_compile_options(${UNI_TARGET} PRIVATE ${UNI_COMPILE_OPTIONS})
  target_link_libraries(${UNI_TARGET} glfw ${Diligent_LIBS} fg::FrameGraph)
else()
  set(SOURCES ${UNIENGINE_ROOT_DIR}/externs/main/android_main.cpp)
  add_library(universe SHARED ${SOURCES})
  target_link_libraries(universe PUBLIC ${Diligent_LIBS} fg::FrameGraph android)
endif()
