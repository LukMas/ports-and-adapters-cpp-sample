# Ensure headers in the same folder or 'include' folder are found
include_directories(.)

add_library(kiosks_ports_adapters_lib STATIC
        console/ConsoleState.h
        console/ConsoleController.h
        console/ConsoleController.cpp
        simulation/SimulatedArm.h
        monitors/UserInactiveWatchdog.h
        messaging/SynchronizedCommandQueue.h
        console/Parser.cpp
        console/Parser.h
        logger/AsyncLogger.cpp
        logger/AsyncLogger.h
)

target_link_libraries(kiosks_ports_adapters_lib PUBLIC kiosk_logic_lib utils_lib)

target_include_directories(kiosks_ports_adapters_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
