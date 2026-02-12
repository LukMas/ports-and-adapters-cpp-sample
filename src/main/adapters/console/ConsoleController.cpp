#include "ConsoleController.h"
#include <iostream>
#include <stop_token>

#include "Parser.h"

void ConsoleController::setupTerminal(termios& oldt)
{
    // It works, I've to check what's going on exactly
    // but, checking the way the code is written, I suppose this saves the actual terminal configuration into the oldt
    tcgetattr(STDIN_FILENO, &oldt);

    // copies it into the newt
    termios newt = oldt;

    // this I don't know
    newt.c_lflag &= ~(ICANON | ECHO);

    // it allows the read to return with 0 chars, hence it's not a blocking process anymore
    newt.c_cc[VMIN] = 0;
    // it waits 1/10 second (100ms) returning 0 char, or it returns immediately with the char
    newt.c_cc[VTIME] = 1;

    // it updates the terminal with the new configuration
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void ConsoleController::restoreTerminal(const termios& oldt)
{
    // it restores the old terminal configuration
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


void ConsoleController::run(const std::stop_source& stop_source)
{
    // It creates an object that stores the terminal configuration
    termios oldt{};
    // and calls the function that updates it
    setupTerminal(oldt);

    // I use the flag to control the loop
    bool isStopped = false;
    // while the system is running ...
    while (!isStopped)
    {
        bool inputChanged = false;
        char ch;

        // first handle the input ...
        if (read(STDIN_FILENO, &ch, 1) > 0)
        {
            // I process the chars in a logical way: first the meaningful ones...
            if (ch >= 32 && ch <= 126)
            {
                m_input += ch;
            }
            // than the 'undo' ones...
            else if (ch == 127 || ch == 8)
            {
                if (!m_input.empty())
                {
                    m_input.pop_back();
                }
            }
            // ... and finally the ones that would generate the command
            else if (ch == '\n' || ch == '\r')
            {
                // now, with exit I stop the system
                if (m_input == "exit")
                {
                    // The call does all the work to send the stop signal to all the other threads
                    // I use the result to gracefully shut down also this loop
                    isStopped = stop_source.request_stop();
                }

                // the solution with the flag allows to process, if it exits, a shutdown command
                // if the input is not empty...
                if (!m_input.empty())
                {
                    // I use the parser to generate a command...
                    std::optional<KioskCommand> command = Console::Parser::parse(m_input);
                    if (command.has_value())
                    {
                        // and if the command exists, I lock the queue and push the command
                        std::lock_guard<std::mutex> lock(m_inputMtx);
                        m_queue.push(command.value());
                    }

                    // I clear the string
                    m_input.clear();
                }
            }

            inputChanged = true;
        }

        // ... then render the status with the written input ...
        // Move cursor to top left, and 1 row down -> this will allow some logging to show, even if the solution should
        // absolutely be changed with a centralized logging
        std::cout << "\033[3H";
        {
            Coordinate armPosition = m_arm.getCurrentPosition();
            std::lock_guard<std::mutex> l(m_viewMtx);
            std::cout << "--- GRABSTATION MONITOR ---                                                     " << "\n";
            std::cout << "Kiosk State: " << m_kioskStatus << "                                            " << "\n";
            std::cout << "Arm Position: (" << armPosition.x << ", " << armPosition.y << ")                " << "\n";
            std::cout << "---------------------------                                                     " << "\n";
        }

        {
            // I lock for a short moment to print the result
            std::lock_guard<std::mutex> lock(m_inputMtx);
            // \r returns to start of line, \033[K clears the line
            std::cout << "\r\033[K> " << m_input << "_" << std::flush;
        }

        // ... finally checks the input (it the input changed I immediately process the next char) and the flag,
        // pausing if both are false.
        if (!inputChanged && !isStopped)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    // I restore the terminal and write one last message.
    restoreTerminal(oldt);
    std::cout << "\nExiting System..." << std::endl;
}
