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
        states/IKioskState.h
        states/IdleState.h
        states/IdleState.cpp
        states/WaitingState.h
        states/WaitingState.cpp
        #        ports/WaitingForCoordsState.cpp
#        ports/WaitingForCoordsState.h
)

# Tell anyone who uses this library where the headers are
target_include_directories(kiosk_logic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kiosk_logic_lib PUBLIC utils_lib)