#include <iostream>

#include "Kiosk.h"
#include "console/ConsoleState.h"
#include "monitors/WatchDog.h"
#include "simulation/SimulatedArm.h"
#include "console/ConsoleController.h"

#include <atomic>


int main()
{
    std::stop_source system_shutdown;
    std::stop_token master_token = system_shutdown.get_token();

    CommandQueue queue;
    ConsoleState state;

    ConsoleController console_controller(queue, state);

    // The arm that simulates the movement to grab the object
    SimulatedArm arm;

    // The monitoring service
    Watchdog watchdog(queue);

    // main logic
    Kiosk kiosk(queue, console_controller, arm, 5, 5);

    // allows the watchdog to notify the kiosk
    kiosk.addStatusListener(&console_controller);

    // starting threads
    std::jthread watchdog_thread([&monitor = watchdog, &master_token]()
    {
        while (!master_token.stop_requested())
        {
            monitor.check();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::jthread ui_thread([&console_controller, &system_shutdown]()
    {
        console_controller.run(system_shutdown);
    });

    // 7. Main Logic Loop (The Heartbeat)
    // This runs in the main thread
    try
    {
        while (!master_token.stop_requested())
        {
            kiosk.step();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "FATAL ERROR in Kiosk: " << e.what() << std::endl;
    }

    return 0;
}
