//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_SIMULATEDARM_H
#define GRABSTATION_SIMULATEDARM_H
#pragma once
#include "ports/IArmPort.h"
#include "IArmTelemetry.h"
#include <vector>
#include <thread>
#include <atomic>
#include <format>

class SimulatedArm : public IArmPort
{
private:
    std::vector<IArmTelemetry*> m_listeners;
    std::atomic<int> m_x{0}, m_y{0};

public:
    SimulatedArm() = default;

    void addTelemetryListener(IArmTelemetry* l) { m_listeners.push_back(l); }

    void moveTo(int tx, int ty) override
    {
        // create a thread that simulates the arm movement...
        std::thread([this, tx, ty]()
        {
            while ((m_y != ty) || (m_x != tx))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                // first I'll align on the x-axis...
                if (m_y != ty)
                {
                    m_y += (ty > m_y) ? 1 : -1;
                }
                // ... then I'll align on the y-axis
                else
                {
                    m_x += (tx > m_x) ? 1 : -1;
                }
                // I notify each listener that the position has changed
                for (auto* l : m_listeners) l->onPositionChanged(m_x, m_y);
            }
            // ... then detach it when the operation is finish to destroy it
        }).detach();
    }
};

#endif //GRABSTATION_SIMULATEDARM_H
