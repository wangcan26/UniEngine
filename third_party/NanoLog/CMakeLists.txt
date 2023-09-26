cmake_minimum_required(VERSION 3.20)

project(NanoLog)

add_library(
    NanoLog
    "NanoLog.cpp"
    "NanoLog.hpp"
)

target_include_directories(
    NanoLog PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
                    
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )