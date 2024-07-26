cmake_minimum_required(VERSION 3.12)
project(SnapKey)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SnapKey SnapKey.cpp resources.rc)

if (WIN32)
    set_target_properties(SnapKey PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()

