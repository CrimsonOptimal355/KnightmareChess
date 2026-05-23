cmake_minimum_required(VERSION 3.15)
project(Chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Direct CMake to the local SFML installation in the project directory
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/lib/cmake/SFML" CACHE PATH "Path to SFML CMake config")

# Find SFML 3
# find_package(SFML 3.0 COMPONENTS graphics window system REQUIRED)

# Include local headers
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Define executable target
add_executable(Chess ${SOURCES})

# Link SFML and system libraries
target_link_libraries(Chess PRIVATE sfml-graphics sfml-window sfml-system opengl32)

# Copy SFML DLLs to the output directory on Windows after build
if(WIN32)
    add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-graphics-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-graphics-d-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-window-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-window-d-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-system-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-system-d-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-audio-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-audio-d-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-network-3.dll"
        "${CMAKE_CURRENT_SOURCE_DIR}/New/Practice/External/SFML-MinGW/SFML-3.0.0/bin/sfml-network-d-3.dll"
        $<TARGET_FILE_DIR:Chess>
        NO_SOURCE_PERMISSIONS
    )
endif()
