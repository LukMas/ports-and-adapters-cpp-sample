# --- REDIRECT OUTPUT TO BIN FOLDER ---
# This puts the .exe (or Linux binary) in cmake-build-debug/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# (Optional) This puts the .lib or .a files in cmake-build-debug/lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

cmake_minimum_required(VERSION 4.1)
project(GrabStationTest)

set(CMAKE_CXX_STANDARD 20)

# Ensure headers in the same folder or 'include' folder are found
include_directories(.)

# --- 1. FIND THE THREADS LIBRARY ---
# This looks for pthreads on Linux/macOS or the Windows equivalent
find_package(Threads REQUIRED)

# Target 2: The UI Test
# Make sure your file is named ui_test.cpp in your folder!
add_executable(GrabStationTest ui_test.cpp)
target_link_libraries(UITest PRIVATE Threads::Threads)
