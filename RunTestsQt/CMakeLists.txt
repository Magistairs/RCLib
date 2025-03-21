cmake_minimum_required(VERSION 3.15)
project(RunTestsQt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Define RCLIB_API for Qt project
add_definitions(-DRCLIB_API=__declspec\(dllexport\))

# Add the build directory to the package search path
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/install")

# Set paths for RCLib and RCLibQt
set(RCLib_DIR "${CMAKE_SOURCE_DIR}/out/build/x64-Debug/RCLib")
set(RCLibQt_DIR "${CMAKE_SOURCE_DIR}/out/build/x64-Debug/RCLibQt")
set(RCLibTests_DIR "${CMAKE_SOURCE_DIR}/out/build/x64-Debug/RCLibTests")

message(STATUS "${PROJECT_NAME} will look for RCLibConfig.cmake in ${RCLib_DIR}") 

# Add Qt6 path
set(Qt6_DIR "C:/Qt/6.6.0/msvc2019_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(RCLib REQUIRED)
find_package(RCLibQt REQUIRED)
find_package(RCLibTests REQUIRED)

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/Application.cpp
    src/LogWidget.cpp
    src/LogControlWidget.cpp
    src/QtFactory.cpp
)

set(HEADERS
    include/MainWindow.h
    include/Application.h
    include/LogWidget.h
    include/LogControlWidget.h
    include/QtFactory.h
    include/QtLogger.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    RCLib
    RCLibTests
    RCLibQt
    Qt6::Core
    Qt6::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Print installation paths for debugging
message(STATUS "${PROJECT_NAME} will be installed to: ${CMAKE_INSTALL_PREFIX}/bin") 