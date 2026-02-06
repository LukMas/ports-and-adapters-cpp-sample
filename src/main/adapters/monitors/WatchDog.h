//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_WATCHDOG_H
#define GRABSTATION_WATCHDOG_H
#pragma once
#include "IStatusListener.h"
#include <chrono>

/**
 * The watchdog class defines a monitor, implementing the IStatusListener,
 * that changes back the status of the machine to IDLE when the actual status
 * is WAITING and the user hasn't interacted for more than 30 seconds. 
 */
class Watchdog : public IStatusListener
{
private:
    ICommandQueue& m_queue;

    std::mutex m_mutex{};

    bool m_isProcessing = false;
    std::chrono::steady_clock::time_point m_lastActivity{};

    const int TIMEOUT = 30;

public:
    Watchdog(ICommandQueue& q) : m_queue(q)
    {
    }

    void onStatusChanged(const MachineStatus s) override
    {
        // I need to avoid incongruences in the check
        std::lock_guard<std::mutex> lock(m_mutex); // Locks here
        // now I update all the values
        m_isProcessing = (s == MachineStatus::PROCESSING);
        m_lastActivity = m_isProcessing
                             ? std::chrono::steady_clock::now() // store the value
                             : std::chrono::steady_clock::time_point{}; // reset to something
    }

    void check()
    {
        std::lock_guard<std::mutex> lock(m_mutex); // Locks here

        // waiting?
        if (!m_isProcessing)
        {
            // if not, do nothing
            return;
        }

        // otherwise find the elapsed time
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - m_lastActivity);

        // if the timeout has expired ...
        if (elapsed.count() > TIMEOUT)
        {
            //... reset the flag and push the status to idle
            m_isProcessing = false;
            m_queue.push(KioskCommand(CommandType::IDLE));
        }
    }
};
#endif //GRABSTATION_WATCHDOG_H
