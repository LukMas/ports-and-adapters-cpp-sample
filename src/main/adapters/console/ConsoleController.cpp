#include "ConsoleController.h"
#include <iostream>
#include <stop_token>

void ConsoleController::setupTerminal(termios& oldt)
{
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    // Non-blocking: wait at most 100ms for input
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void ConsoleController::restoreTerminal(const termios& oldt)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


void ConsoleController::run(const std::stop_source& stop_source)
{
    termios oldt;
    setupTerminal(oldt);

    while (!stop_source.stop_requested())
    {
        bool inputChanged = false;
        char ch;

        // 1. READ INPUT (Non-blocking)
        if (read(STDIN_FILENO, &ch, 1) > 0)
        {
            std::lock_guard<std::mutex> lock(m_state.mtx);
            if (ch == '\n' || ch == '\r')
            {
                if (m_state.currentInput == "exit")
                {
                    stop_source.request_stop();
                    break;
                }
                else if (!m_state.currentInput.empty())
                {
                    m_queue.push(m_state.currentInput);
                    m_state.currentInput.clear();
                }
            }
            else if (ch == 127 || ch == 8)
            {
                if (!m_state.currentInput.empty()) m_state.currentInput.pop_back();
            }
            else if (ch >= 32 && ch <= 126)
            {
                m_state.currentInput += ch;
            }
            inputChanged = true;
        }

        // 2. RENDER (Refresh UI)
        // Move cursor to top left
        std::cout << "\033[H";

        {
            std::lock_guard<std::mutex> l(m_viewMtx);
            std::cout << "--- GRABSTATION MONITOR ---" << "\n";
            std::cout << "Kiosk State: " << m_kioskStatus << "          " << "\n"; // Spaces clear old text
            std::cout << "Arm Position: (" << m_armX << ", " << m_armY << ")    " << "\n";
            std::cout << "---------------------------" << "\n";
        }

        {
            std::lock_guard<std::mutex> lock(m_state.mtx);
            // \r returns to start of line, \033[K clears the line
            std::cout << "\r\033[K> " << m_state.currentInput << "_" << std::flush;
        }

        // 3. SLEEP (To avoid 100% CPU, but keep it responsive)
        if (!inputChanged)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    restoreTerminal(oldt);
    std::cout << "\nExiting System..." << std::endl;
}
