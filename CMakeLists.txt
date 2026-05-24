cmake_minimum_required(VERSION 3.15)
project(Chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use bundled SFML from external/
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML-3.1.0/lib/cmake/SFML" CACHE PATH "Path to SFML CMake config")

# Find SFML 3
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Include local headers
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Define executable target
add_executable(Chess ${SOURCES})

# Link SFML and system libraries
target_link_libraries(Chess PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)

# Copy SFML DLLs to the output directory on Windows after build
if(WIN32)
    add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML-3.1.0/bin"
        $<TARGET_FILE_DIR:Chess>
    )
endif()
