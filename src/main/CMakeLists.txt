# src/main/CMakeLists.txt

# 1. Enter all subfolders to build the libraries
add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(adapters)

# 2. Define the main executable (the one with int main())
add_executable(GrabStation main.cpp)

find_package(Threads REQUIRED)

# 3. LINK EVERYTHING TOGETHER
# This is where the magic happens. GrabStation "imports" the code from the libs.
target_link_libraries(GrabStation PRIVATE
        kiosks_ports_adapters_lib
        Threads::Threads
)