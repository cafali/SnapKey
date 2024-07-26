cmake_minimum_required(VERSION 3.12)
project(SnapKey)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}) --- unnecessary

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
set(BUILD_SHARED_LIBS OFF)

add_executable(SnapKey SnapKey.cpp resources.rc)

if (WIN32)
    set_target_properties(SnapKey PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()

