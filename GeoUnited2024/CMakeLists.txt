cmake_minimum_required(VERSION 3.10)

# Project name and language
project(MyProject LANGUAGES CXX)

# Add source files
set(SOURCE_FILES
    src/main.cpp
    src/myclass.cpp
)

# Create the main target
add_executable(MyProject ${SOURCE_FILES})

# Specify include directories for the target
target_include_directories(MyProject PRIVATE include)

# Specify dependencies for the target
target_link_libraries(MyProject PRIVATE
    # Specify external libraries if used
)

# Compilation options
target_compile_options(MyProject PRIVATE
    -Wall
    -Wextra
    -pedantic
    # Specify other compilation options here
)
