cmake_minimum_required(VERSION 3.27)
project(isenchef C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags for better code quality
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")

# Source files
set(SOURCES
    src/main.c
    src/args.c
    src/file_io.c
    src/encoders.c
    src/actions.c
    src/processor.c
)

# Include directories
include_directories(include)

# Create executable
add_executable(isenchef ${SOURCES})

# Set output name
set_target_properties(isenchef PROPERTIES OUTPUT_NAME "isenchef")
