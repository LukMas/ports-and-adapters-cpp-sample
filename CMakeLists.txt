# --- REDIRECT OUTPUT TO BIN FOLDER ---
# This puts the .exe (or Linux binary) in cmake-build-debug/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# (Optional) This puts the .lib or .a files in cmake-build-debug/lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

cmake_minimum_required(VERSION 4.1)
project(GrabStation)

set(CMAKE_CXX_STANDARD 20)

# Ensure headers in the same folder or 'include' folder are found
add_subdirectory(src/main)


# Testing (Optional toggle)
option(BUILD_TESTING "Enable unit tests" OFF)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(src/test)
endif()