cmake_minimum_required(VERSION 3.12)
project(SnapKey)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}) --- unnecessary

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    list(APPEND SOURCES src/windows.cpp)
else()
    list(APPEND SOURCES src/linux.cpp)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
set(BUILD_SHARED_LIBS OFF)

add_executable(SnapKey ${SOURCES} resources.rc)

if (WIN32)
    set_target_properties(SnapKey PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
else()
    find_package(X11 REQUIRED)
    find_package(Xtst REQUIRED)
    find_package(Xinput2 REQUIRED)
    target_link_libraries(SnapKey ${X11_LIBRARIES} ${XTST_LIBRARIES})
endif()

