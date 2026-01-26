//
// Created by luca on 21/01/26.
//
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std::chrono_literals;

// Simple structure to hold mock data for the test
struct MockKiosk
{
    int arm_x = 0;
    int arm_y = 0;
    int grid[5][5]{};

    MockKiosk()
    {
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 5; ++c) grid[r][c] = 5; // Start with 5 items each
    }
};

void render(const MockKiosk& kiosk)
{
    // \033[H moves cursor to top-left. \033[2J clears the whole screen.
    // For a smooth test, we use Home (\033[H) to overwrite.
    std::cout << "\033[H";

    std::cout << "=== UI RENDERING TEST (5 SECONDS) ===\n\n";
    std::cout << "    1   2   3   4   5\n";

    for (int r = 0; r < 5; ++r)
    {
        char rowLabel = 'A' + r;
        std::cout << rowLabel << " | ";
        for (int c = 0; c < 5; ++c)
        {
            if (kiosk.arm_x == c && kiosk.arm_y == r)
            {
                // Green bold asterisk for the arm
                std::cout << "\033[1;32m * \033[0m ";
            }
            else
            {
                std::cout << "[" << kiosk.grid[r][c] << "] ";
            }
        }
        std::cout << "\n\n";
    }
    std::cout << "-------------------------------------\n";
    std::cout << "Testing movement... Pos: [" << (char)('A' + kiosk.arm_y) << (kiosk.arm_x + 1) << "]\n";
    std::cout << std::flush;

    std::this_thread::sleep_for(400ms);
}

int main()
{
    MockKiosk kiosk;
    kiosk.arm_x = 0;
    kiosk.arm_y = 0;

    // Clear screen once at start
    std::cout << "\033[2J";

    auto startTime = std::chrono::steady_clock::now();

    while (kiosk.arm_y < 4)
    {
        render(kiosk);
        kiosk.arm_y++;
    }

    render(kiosk);

    while (kiosk.arm_x < 4)
    {
        render(kiosk);
        kiosk.arm_x++;
    }

    render(kiosk);

    std::cout << "\nTest Complete. If the grid flickered, check CLion terminal settings.\n";
    return 0;
}
