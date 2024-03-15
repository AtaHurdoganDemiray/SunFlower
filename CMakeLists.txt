cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name and language
project(SunFlower LANGUAGES CXX)

# Add source files
set(SOURCE_FILES
    GeoUnited2024/src/AABB.cpp
    GeoUnited2024/src/AddTrimLoop2SurfPointsPack_.cpp
    
)

# Create the main target
add_executable(SunFlower ${SOURCE_FILES})

# Specify include directories for the target
target_include_directories(SunFlower PRIVATE GeoUnited2024/include
                                             GeoUnited2024/include/gl                             
                                             GeoUnited2024/include/NonMFCDefinitions
                                             GeoUnited2024/include/UI                )

# Specify dependencies for the target
target_link_libraries(SunFlower PRIVATE
    # Specify external libraries if used
)

# Compilation options
target_compile_options(SunFlower PRIVATE
    -Wall
    -Wextra
    -pedantic
    # Specify other compilation options here
)
