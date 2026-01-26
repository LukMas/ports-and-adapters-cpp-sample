//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_WATCHDOG_H
#define GRABSTATION_WATCHDOG_H
#pragma once
#include "IActivityListener.h"
#include "IStatusListener.h"
#include "CommandQueue.h"
#include <chrono>
#include <atomic>
class Watchdog : public IActivityListener, public IStatusListener
{
private:
    CommandQueue& m_queue;
    std::atomic<std::chrono::steady_clock::time_point> m_lastActivity;
    std::atomic<MachineStatus> m_status{MachineStatus::IDLE};

public:
    Watchdog(CommandQueue& q) : m_queue(q), m_lastActivity(std::chrono::steady_clock::now())
    {
    }

    void onUserActivity() override { m_lastActivity = std::chrono::steady_clock::now(); }
    void onStatusChanged(const MachineStatus s) override { m_status = s; }

    void check()
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - m_lastActivity.load());

        if (m_status == MachineStatus::IDLE && elapsed.count() > 30)
        {
            m_queue.push("IDLE");
            onUserActivity();
        }
    }
};
#endif //GRABSTATION_WATCHDOG_H
