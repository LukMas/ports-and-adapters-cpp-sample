# Ensure headers in the same folder or 'include' folder are found
include_directories(.)

# --- 1. FIND THE THREADS LIBRARY ---
# This looks for pthreads on Linux/macOS or the Windows equivalent
find_package(Threads REQUIRED)

add_library(kiosk_logic_lib STATIC
        Kiosk.h
        Kiosk.cpp
        ports/IViewPort.h
        ports/IArmPort.h
        states/CoreLogicState.h
        states/CoreLogicState.cpp
        states/States.h
        states/States.cpp
)

# Tell anyone who uses this library where the headers are
target_include_directories(kiosk_logic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kiosk_logic_lib PUBLIC utils_lib)