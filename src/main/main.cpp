#include <cmath>
#include <iostream>

#include "Kiosk.h"
#include "console/ConsoleState.h"
#include "monitors/WatchDog.h"
#include "simulation/SimulatedArm.h"
#include "console/ConsoleController.h"
#include "logger/AsyncLogger.h"
#include "messaging/SynchronizedCommandQueue.h"


int main()
{
    std::stop_source system_shutdown;
    std::stop_token master_token = system_shutdown.get_token();

    Logger::AsyncLogger async_logger("kiosk.log");

    ConsoleState state;

    SynchronizedCommandQueue queue(master_token);

    // The arm that simulates the movement to grab the object
    SimulatedArm arm;

    // Now I create the console.
    ConsoleController console_controller(queue, state, arm);


    // The monitoring service
    Watchdog watchdog(queue);

    // I create the controller now, I've all the items for it
    Kiosk kiosk(queue, console_controller, arm, async_logger, 5, 5);


    // allows the kiosk to notify the watchdog
    kiosk.addStatusListener(&watchdog);
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


    std::jthread arm_simulator_thread([&simulator = arm, &master_token]()
    {
        while (!master_token.stop_requested())
        {
            simulator.move();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::jthread watchdog_thread([&monitor = watchdog, &master_token]()
    {
        while (!master_token.stop_requested())
        {
            monitor.check();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    // this initializes the Kiosk
    kiosk.start();
    // 7. Main Logic Loop (The Heartbeat)
    // This runs in the main thread
    try
    {
        while (!master_token.stop_requested())
        {
            kiosk.step();
        }
    }

    catch (const std::exception& e)
    {
        std::cerr << "FATAL ERROR in Kiosk: " << e.what() << std::endl;
    }

    return 0;
}
