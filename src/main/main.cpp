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

    ConsoleInput inputHandler(queue, state);


    ConsoleView console_view;
    SimulatedArm arm(queue);
    // monitoring

    Watchdog watchdog(queue);

    // main logic
    Kiosk kiosk(queue, console_view, arm);

    // allows the watchdog to notify the kiosk
    kiosk.addStatusListener(&watchdog);

    // starting threads
    // Thread 1: Input (Blocks on read())
    std::thread input_thread(&ConsoleInput::run, &inputHandler);

    std::thread watchdog_thread([&]()
    {
        while (state.running)
        {
            watchdog.check();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });


    std::thread ui_thread([&]()
    {
        while (state.running)
        {
            console_view.render(state, kiosk);
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
