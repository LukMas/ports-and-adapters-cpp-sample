#include <iostream>

#include "Kiosk.h"
#include "console/ConsoleInput.h"
#include "console/ConsoleView.h"
#include "monitors/WatchDog.h"
#include "simulation/SimulatedArm.h"

#include <atomic>

int main()
{
    std::stop_source system_shutdown;
    std::stop_token master_token = system_shutdown.get_token();


    CommandQueue queue;
    ConsoleState state;

    // I define the handler of the input using the console
    ConsoleInput inputHandler(queue, state);

    // I define the output using the console
    ConsoleView console_view;

    // The arm that simulates the movement to grab the object
    SimulatedArm arm;

    // The monitoring service
    Watchdog watchdog(queue);

    // main logic
    Kiosk kiosk(queue, console_view, arm, 5, 5);

    // allows the watchdog to notify the kiosk
    kiosk.addStatusListener(&watchdog);

    // starting threads
    //
    // Input thread (Blocks on read())
    std::jthread input_thread([&input = inputHandler, &system_shutdown, &master_token]()
    {
        while (!master_token.stop_requested())
        {
            input.run(system_shutdown);
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

    // Ui thread used to show the status of the kiosk
    std::jthread ui_thread([&console_view, &state, &master_token]()
    {
        while (!master_token.stop_requested())
        {
            console_view.render(state);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
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
