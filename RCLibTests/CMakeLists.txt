cmake_minimum_required(VERSION 3.10)
project(RCLibTests VERSION 1.0.0)

# Set install prefix before anything else
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation prefix" FORCE)
endif()

# Test library sources
file(GLOB_RECURSE RCLIB_TESTS_SRC 
    "src/*.cpp"
)

file(GLOB_RECURSE RCLIB_TESTS_HEADERS
    "include/*.h"
)

# Create the test library
add_library(RCLibTests SHARED 
    ${RCLIB_TESTS_SRC}
    ${RCLIB_TESTS_HEADERS}
)

target_compile_definitions(RCLibTests 
    PRIVATE 
        RCLIB_TESTS_EXPORT
)

target_include_directories(RCLibTests 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/RCLib/include
)

# Link against core library
target_link_libraries(RCLibTests 
    PUBLIC 
        RCLib
)

set_target_properties(RCLibTests PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install targets
install(TARGETS RCLibTests
    EXPORT RCLibTestsTargets
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
install(EXPORT RCLibTestsTargets
    FILE RCLibTestsTargets.cmake
    NAMESPACE RCLibTests::
    DESTINATION lib/cmake/RCLibTests
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibTestsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/RCLibTestsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibTestsConfig.cmake"
    INSTALL_DESTINATION lib/cmake/RCLibTests
)

# Install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibTestsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibTestsConfigVersion.cmake"
    DESTINATION lib/cmake/RCLibTests
)

# Print installation paths for debugging
message(STATUS "RCLibTests will be installed to: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "RCLibTests config files will be installed to: ${CMAKE_INSTALL_PREFIX}/lib/cmake/RCLibTests") 