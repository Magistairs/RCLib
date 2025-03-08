cmake_minimum_required(VERSION 3.10)
project(RCLib VERSION 1.0.0)

set(Qt6_DIR "C:/Qt/6.6.0/msvc2019_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)

set(BUILD_STATIC OFF)
set(BUILD_EXAMPLE ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/Qt-Advanced-Docking-System ${CMAKE_BINARY_DIR}/qtadvanceddocking_build)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE RCLIB_SRC "src/*.cpp")
file(GLOB_RECURSE RCLIB_INCLUDE "include/*.h")

add_library(RCLib STATIC ${RCLIB_SRC} ${RCLIB_INCLUDE})

target_include_directories(RCLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs/Qt-Advanced-Docking-System/src
)

target_link_libraries(RCLib PRIVATE Qt6::Core Qt6::Gui Qt6::Test Qt6::Widgets qtadvanceddocking-qt6)

set_target_properties(RCLib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
