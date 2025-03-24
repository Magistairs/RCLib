cmake_minimum_required(VERSION 3.10)
project(RCLib VERSION 1.0.0)

# Set install prefix before anything else
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation prefix" FORCE)
endif()

set(Qt6_DIR "C:/Qt/6.6.0/msvc2019_64/lib/cmake/Qt6")

# Find Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Core)

# Core library sources
file(GLOB_RECURSE RCLIB_CORE_SRC 
    "src/*.cpp"
)

file(GLOB_RECURSE RCLIB_CORE_HEADERS
    "include/*.h"
)

# Create the core library
add_library(RCLib SHARED 
    ${RCLIB_CORE_SRC}
    ${RCLIB_CORE_HEADERS}
)

add_library(RCLib::RCLib ALIAS RCLib)

target_compile_definitions(RCLib 
    PRIVATE 
        RCLIB_EXPORT
)

target_include_directories(RCLib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Core library only needs minimal Qt dependencies
target_link_libraries(RCLib 
    PUBLIC
        Qt6::Core
)

set_target_properties(RCLib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    DEBUG_POSTFIX "d"
    OUTPUT_NAME "RCLib"
)

# Install targets
install(TARGETS RCLib
    EXPORT RCLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Install CMake config files
install(EXPORT RCLibTargets
    FILE RCLibTargets.cmake
    NAMESPACE RCLib::
    DESTINATION lib/cmake/RCLib
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/RCLibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibConfig.cmake"
    INSTALL_DESTINATION lib/cmake/RCLib
    PATH_VARS CMAKE_INSTALL_PREFIX
)

# Install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibConfigVersion.cmake"
    DESTINATION lib/cmake/RCLib
)

# Export build directory
export(EXPORT RCLibTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/RCLibTargets.cmake"
    NAMESPACE RCLib::
)

# Print installation paths for debugging
message(STATUS "RCLib will be installed to: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "RCLib config files will be installed to: ${CMAKE_INSTALL_PREFIX}/lib/cmake/RCLib") 