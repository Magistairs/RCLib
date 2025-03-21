cmake_minimum_required(VERSION 3.10)
project(RCLibQt VERSION 1.0.0)

# Set install prefix before anything else
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation prefix" FORCE)
endif()

# Qt library sources
file(GLOB_RECURSE RCLIB_QT_SRC 
    "src/*.cpp"
)

file(GLOB_RECURSE RCLIB_QT_HEADERS
    "include/*.h"
)

# Create the Qt widgets library
add_library(RCLibQt SHARED 
    ${RCLIB_QT_SRC}
    ${RCLIB_QT_HEADERS}
)

target_compile_definitions(RCLibQt 
    PRIVATE 
        RCLIB_QT_EXPORT
)

target_include_directories(RCLibQt 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/RCLib/include
)

# Link against core library and Qt
target_link_libraries(RCLibQt 
    PUBLIC 
        RCLib
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

set_target_properties(RCLibQt PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install targets
install(TARGETS RCLibQt
    EXPORT RCLibQtTargets
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
install(EXPORT RCLibQtTargets
    FILE RCLibQtTargets.cmake
    NAMESPACE RCLibQt::
    DESTINATION lib/cmake/RCLibQt
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibQtConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/RCLibQtConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibQtConfig.cmake"
    INSTALL_DESTINATION lib/cmake/RCLibQt
)

# Install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibQtConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/RCLibQtConfigVersion.cmake"
    DESTINATION lib/cmake/RCLibQt
)

# Print installation paths for debugging
message(STATUS "RCLibQt will be installed to: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "RCLibQt config files will be installed to: ${CMAKE_INSTALL_PREFIX}/lib/cmake/RCLibQt") 