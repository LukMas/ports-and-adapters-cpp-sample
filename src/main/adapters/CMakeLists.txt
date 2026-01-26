# Ensure headers in the same folder or 'include' folder are found
include_directories(.)

add_library(kiosks_ports_adapters_lib STATIC
        console/ConsoleInput.h
        console/ConsoleView.h
        simulation/SimulatedArm.h
        monitors/WatchDog.h
        console/ConsoleState.h
        console/ConsoleInput.cpp
)

target_link_libraries(kiosks_ports_adapters_lib PUBLIC kiosk_logic_lib utils_lib)

target_include_directories(kiosks_ports_adapters_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
