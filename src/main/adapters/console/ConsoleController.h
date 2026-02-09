//
// Created by luca on 01/02/26.
//

#ifndef GRABSTATION_CONSOLECONTROLLER_H
#define GRABSTATION_CONSOLECONTROLLER_H

#include <string>
#include <mutex>
#include <termios.h>
#include <stop_token>
#include "ports/IViewPort.h"
#include "IStatusListener.h"
#include "ConsoleState.h" // Assuming this holds your currentInput and mtx
#include "ports/ICommandQueue.h"

class ConsoleController : public IViewPort, public IStatusListener
{
private:
    ICommandQueue& m_queue;
    ConsoleState& m_state;
    IArmPort& m_arm;

    // View Data (Protected by internal mutex)
    std::mutex m_viewMtx{};
    std::string m_kioskStatus = "INITIALIZING";
    int m_armX = 0;
    int m_armY = 0;

    static void setupTerminal(termios& oldt);
    static void restoreTerminal(const termios& oldt);

public:
    ConsoleController(ICommandQueue& q, ConsoleState& s, IArmPort& arm) : m_queue(q), m_state(s), m_arm(arm)
    {
    }

    // Ports/Listener implementations
    void notifyMessage(std::string status) override
    {
        std::lock_guard<std::mutex> l(m_viewMtx);
        m_kioskStatus = std::move(status);
    }

    void onStatusChanged(MachineStatus s) override
    {
        /* update local status if needed */
    }

    void notifyError(std::string msg) override
    {
        /* handle error */
    }

    // THE MAIN LOOP
    void run(const std::stop_source& stop_source);
};

#endif
