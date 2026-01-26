#include "ConsoleInput.h"
#include <termios.h>
#include <unistd.h>
#include <iostream>

void ConsoleInput::run() {
    // 1. Terminal Magic: Disable "canonical mode" (waiting for Enter) and "echo"
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch;
    while (m_state.running) {
        // Read 1 byte at a time (non-blocking if you use select, but blocking is fine here)
        if (read(STDIN_FILENO, &ch, 1) > 0) {
            std::lock_guard<std::mutex> lock(m_state.mtx);

            if (ch == '\n' || ch == '\r') { // ENTER
                if (m_state.currentInput == "exit") {
                    m_state.running = false;
                } else if (!m_state.currentInput.empty()) {
                    m_queue.push(m_state.currentInput);
                    m_state.currentInput.clear();
                }
            } else if (ch == 127 || ch == 8) { // BACKSPACE
                if (!m_state.currentInput.empty()) m_state.currentInput.pop_back();
            } else if (ch >= 32 && ch <= 126) { // Printable characters
                m_state.currentInput += ch;
            }
        }
    }

    // 2. Restore the terminal to normal before leaving
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}