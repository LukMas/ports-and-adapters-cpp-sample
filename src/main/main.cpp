#include <cmath>
#include <iostream>

#include "Kiosk.h"
#include "monitors/UserInactiveWatchdog.h"
#include "simulation/SimulatedArm.h"
#include "console/ConsoleController.h"
#include "logger/AsyncLogger.h"
#include "messaging/SynchronizedCommandQueue.h"


int main()
{
    std::stop_source system_shutdown;
    std::stop_token master_token = system_shutdown.get_token();

    Logger::AsyncLogger async_logger("kiosk.log");

    SynchronizedCommandQueue queue(master_token);

    // The arm that simulates the movement to grab the object
    SimulatedArm arm;

    // Now I create the console.
    ConsoleController console_controller(queue, arm);


    // The monitoring service
    UserInactiveWatchdog user_inactive_watchdog(queue);

    // I create the controller now, I've all the items for it
    Kiosk kiosk(queue, console_controller, arm, async_logger, 5, 5);


    // allows the kiosk to notify the watchdog
    kiosk.addStatusListener(&user_inactive_watchdog);
    // allows the kiosk to notify the view
    kiosk.addStatusListener(&console_controller);


    // starting threads
    std::jthread ui_thread([&console_controller, &system_shutdown]()
    {
        console_controller.run(system_shutdown);
    });


    std::jthread log_thread([&async_logger, &master_token]()
    {
        async_logger.flush(master_token);
    });

    // this initializes the Kiosk
    kiosk.start();
    // 7. Main Logic Loop (The Heartbeat)
    // This runs in the main thread
    try
    {
        while (!master_token.stop_requested())
        {
            arm.move();
            user_inactive_watchdog.check();
            kiosk.step();
        }
    }

    catch (const std::exception& e)
    {
        std::cerr << "FATAL ERROR in Kiosk: " << e.what() << std::endl;
    }

    return 0;
}
