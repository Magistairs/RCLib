cmake_minimum_required(VERSION 3.10)
project(RunTests VERSION 1.0.0)

# Set install prefix before anything else
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation prefix" FORCE)
endif()

# Set paths for RCLib and RCLibTests
set(RCLib_DIR "${CMAKE_SOURCE_DIR}/../build/RCLib/CMakeFiles")
set(RCLibTests_DIR "${CMAKE_SOURCE_DIR}/../build/RCLibTests/CMakeFiles")

# Console test runner sources
add_executable(RunTests 
    "src/main.cpp"
)

target_include_directories(RunTests 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against core and test libraries
target_link_libraries(RunTests 
    PRIVATE 
        RCLib
        RCLibTests
        Qt6::Core
)

set_target_properties(RunTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install targets
install(TARGETS RunTests
    RUNTIME DESTINATION bin
)

# Print installation paths for debugging
message(STATUS "RunTests will be installed to: ${CMAKE_INSTALL_PREFIX}/bin") 