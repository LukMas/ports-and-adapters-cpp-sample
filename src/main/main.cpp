#include <iostream>

#include "Kiosk.h"
#include "console/ConsoleInput.h"
#include "console/ConsoleView.h"
#include "monitors/WatchDog.h"
#include "simulation/SimulatedArm.h"

#include <atomic>

int main()
{
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
    Kiosk kiosk(queue, console_view, arm);

    // allows the watchdog to notify the kiosk
    kiosk.addStatusListener(&watchdog);

    // starting threads
    //
    // Input thread (Blocks on read())
    std::thread input_thread(&ConsoleInput::run, &inputHandler);

    std::thread watchdog_thread([&]()
    {
        while (state.running)
        {
            watchdog.check();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    // Ui thread used to show the status of the kiosk
    std::thread ui_thread([&]()
    {
        while (state.running)
        {
            console_view.render(state);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    });

    // 7. Main Logic Loop (The Heartbeat)
    // This runs in the main thread
    try
    {
        while (state.running)
        {
            kiosk.step();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "FATAL ERROR in Kiosk: " << e.what() << std::endl;
    }

    // Cleanup (If you ever break the loop)
    if (watchdog_thread.joinable())
    {
        watchdog_thread.join();
    }

    if (ui_thread.joinable())
    {
        ui_thread.join();
    }

    if (input_thread.joinable())
    {
        input_thread.join();
    }

    return 0;
}
