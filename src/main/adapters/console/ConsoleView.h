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
    void updateDisplay(const std::optional<std::string>::value_type& basic_string) override
    {
        std::cout << "\033[H\033[J" << "Message: " << basic_string << "\n";

    };

    void displayState(const std::string& status) override
    {
        std::lock_guard<std::mutex> l(m_mtx);
        this->m_state = status;
    };

    void onStatusChanged(MachineStatus s) override
    {
        std::lock_guard<std::mutex> l(m_mtx);
        m_status = s;
    }

    void render(const ConsoleState& state, const Kiosk& kiosk) {
        // Clear screen and Move cursor to top-left
        std::cout << "\033[2J\033[H";

        // Draw Status Area
        std::cout << "--- GRABSTATION MONITOR ---" << std::endl;
        // std::cout << "Kiosk State: " << kiosk.getCurrentStateName() << std::endl;
        // std::cout << "Arm Position: " << kiosk.getPos() << std::endl;
        std::cout << "---------------------------" << std::endl;

        // Draw Input Area (Re-drawing what the user typed)
        {
            std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(state.mtx));
            std::cout << "> " << state.currentInput << "_" << std::flush;
        }
    }
};

#endif //GRABSTATION_CONSOLEVIEW_H
