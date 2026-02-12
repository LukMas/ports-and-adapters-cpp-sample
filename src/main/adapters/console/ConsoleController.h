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
#include "ports/ICommandQueue.h"

class ConsoleController : public IViewPort, public IStatusListener
{
    ICommandQueue& m_queue;
    IArmPort& m_arm;

    std::mutex m_viewMtx{};
    std::mutex m_inputMtx{};

    std::string m_input;

    // View Data (Protected by internal mutex)
    std::string m_kioskStatus = "INITIALIZING";
    int m_armX = 0;
    int m_armY = 0;

    static void setupTerminal(termios& oldt);
    static void restoreTerminal(const termios& oldt);

public:
    ConsoleController(ICommandQueue& q,IArmPort& arm) : m_queue(q), m_arm(arm)
    {
    }

    // Ports/Listener implementations
    void notifyMessage(std::string status) override
    {
        // I lock and copy the status into the local variable
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
