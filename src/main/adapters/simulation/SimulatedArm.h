//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_SIMULATEDARM_H
#define GRABSTATION_SIMULATEDARM_H
#pragma once
#include <shared_mutex>

#include "ports/IArmPort.h"
#include <thread>

/**
 * The class implements the IArmPort and simulates the presence of a real
 * arm that moves to the selected position.
 *
 */
class SimulatedArm : public IArmPort
{
private:
    Coordinate m_destination {0,0}, m_currentPosition{0,0};
    std::mutex m_mutex;

public:
    explicit SimulatedArm()= default;

    void setDestination(const Coordinate& destination) override
    {
        m_mutex.lock();
        m_destination = destination;
    }

    [[nodiscard]] Coordinate getCurrentPosition() const override
    {
        return m_currentPosition;
    }
    [[nodiscard]] bool hasReachedTarget() const override
    {
        return false;
    };

    // void moveTo(int tx, int ty) override
    // {
    //     // create a thread that simulates the arm movement...
    //     std::thread([this, tx, ty]()
    //     {
    //         while ((m_y != ty) || (m_x != tx))
    //         {
    //             std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //             // first I'll align on the x-axis...
    //             if (m_y != ty)
    //             {
    //                 m_y += (ty > m_y) ? 1 : -1;
    //             }
    //             // ... then I'll align on the y-axis
    //             else
    //             {
    //                 m_x += (tx > m_x) ? 1 : -1;
    //             }
    //         }
    //         // ... then detach it when the operation is finish to destroy it
    //     }).detach();
    // }
};

#endif //GRABSTATION_SIMULATEDARM_H
