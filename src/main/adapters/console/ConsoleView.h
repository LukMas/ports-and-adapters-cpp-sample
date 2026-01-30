//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_CONSOLEVIEW_H
#define GRABSTATION_CONSOLEVIEW_H

#pragma once
#include <iostream>
#include <mutex>
#include <optional>

#include "ports/IViewPort.h"
#include "IStatusListener.h"

enum class MachineStatus;

class KioskSnapshot
{
    int armX = 0;
    int armY = 0;
    std::string name;
};

class ConsoleView : public IViewPort, public IStatusListener
{
private:
    std::mutex m_mtx;
    MachineStatus m_status = MachineStatus::IDLE;
    std::string m_state;
    int m_x = 0, m_y = 0;

public:
    ConsoleView() = default;

    void notifyError(const std::string& message) override
    {
    };

    void notifyMessage(const std::string& status) override
    {
        // I lock and just copy the reference, quite fast
        std::lock_guard<std::mutex> l(m_mtx);
        this->m_state = status;
    };

    void onStatusChanged(MachineStatus s) override
    {
        // I lock and copy the while status, it's an enum
        std::lock_guard<std::mutex> l(m_mtx);
        m_status = s;
    }

    void render(const ConsoleState& inputState)
    {
        // 1. Move cursor to top-left (doesn't flicker as much as clearing)
        std::cout << "\033[H";

        // 2. Lock the view's own mutex to get a consistent snapshot of the status
        std::string currentState;
        int x, y;
        {
            // the lock and copy is not useless!
            std::lock_guard<std::mutex> l(m_mtx);
            // copying the value here reduces the time the lock is active ...
            // ... while preserving the integrity of the data that is going ...
            // ... to be shown.
            currentState = m_state;
            x = m_x;
            y = m_y;
        }

        // 3. Draw the UI
        std::cout << "--- GRABSTATION MONITOR ---" << std::endl;
        std::cout << "Kiosk State: " << currentState << std::endl;
        std::cout << "Arm Position: (" << x << ", " << y << ")" << std::endl;
        std::cout << "---------------------------" << std::endl;

        // 4. Draw Input Area (what the user is currently typing)
        {
            // I use the same lock that I use in the input, so I can't create
            // race conditions
            std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(inputState.mtx));
            // Clear the line and print the buffer
            std::cout << "\033[K> " << inputState.currentInput << "_" << std::flush;
        }
    }
};

#endif //GRABSTATION_CONSOLEVIEW_H
