cmake_minimum_required(VERSION 3.10)
project(RCLibProject LANGUAGES CXX VERSION 1.0.0)

set(Qt6_DIR "C:/Qt/6.6.0/msvc2019_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option for smart pointer implementation
option(USE_CUSTOM_SMART_PTR "Use RCLib::SmartPtr instead of std::shared_ptr" OFF)
if(USE_CUSTOM_SMART_PTR)
    add_definitions(-DUSE_RCLIB_SMART_PTR)
endif()

# Add subdirectories for each component
add_subdirectory(RCLib)
add_subdirectory(RCLibTests)
add_subdirectory(RCLibQt)
add_subdirectory(RunTests)
add_subdirectory(RunTestsQt)
