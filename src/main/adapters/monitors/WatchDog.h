//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_WATCHDOG_H
#define GRABSTATION_WATCHDOG_H
#pragma once
#include "IStatusListener.h"
#include "CommandQueue.h"
#include <chrono>

/**
 * The watchdog class defines a monitor, implementing the IStatusListener,
 * that changes back the status of the machine to IDLE when the actual status
 * is WAITING and the user hasn't interacted for more than 30 seconds. 
 */
class Watchdog : public IStatusListener
{
private:
    CommandQueue& m_queue;

    std::mutex m_waitingMutex;

    bool m_isWaiting = false;
    std::chrono::steady_clock::time_point m_lastActivity;

public:
    Watchdog(CommandQueue& q) : m_queue(q), m_lastActivity(std::chrono::steady_clock::now())
    {
    }

    void onStatusChanged(const MachineStatus s) override
    {
        // I need it to avoid incongruences by a call to check just after the IF
        m_waitingMutex.lock();
        // as soon as the user started the machine ...
        if (s == MachineStatus::WAITING)
        {
            // ... I start the countdown
            m_isWaiting = true;
            m_lastActivity = std::chrono::steady_clock::now();
        }
        else
        {
            // ... whatever is the status, the monitor should not be active
            m_isWaiting = false;
        }
    }

    void check()
    {
        m_waitingMutex.lock();
        if (m_isWaiting)
        {
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - m_lastActivity);

            if (elapsed.count() > 30)
            {
                m_queue.push("IDLE");
            }
        }
    }
};
#endif //GRABSTATION_WATCHDOG_H
