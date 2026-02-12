//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_SIMULATEDARM_H
#define GRABSTATION_SIMULATEDARM_H
#pragma once
#include <shared_mutex>

#include "ports/IArmPort.h"
#include <thread>


// I add it here because it's where I use it as atomic. With a different 'arm' I may use a mutex,
// or something else. But here I use an atomic, and I want to be sure that Coordinate could be used
// with atomic.
static_assert(std::atomic<Coordinate>::is_always_lock_free);

/**
 * The class implements the IArmPort and simulates the presence of a real
 * arm that moves to the selected position.
 *
 */
class SimulatedArm : public IArmPort
{
    std::mutex m_mutex;

    Coordinate m_destination{0, 0};

    // It allows to atomically read the values. Moreover, the reader could read not up-to-date values,
    // but being used in a console to show a simulated arm it's not important.
    std::atomic<Coordinate> m_currentPosition{Coordinate{0, 0}};

public:
    explicit SimulatedArm() = default;

    void setDestination(const Coordinate& destination) override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // perform a fast copy of the values, then returns
        m_destination = destination;
    }

    [[nodiscard]] Coordinate getCurrentPosition() const override
    {
        // I extract from the atomic the value
        return m_currentPosition.load();
    }

    [[nodiscard]] bool hasReachedTarget() const override
    {
        return false;
    };

    /**
     * The function continuously checks the current position and the destination, moving the arm
     * over the Y-axis, before, and the X-axis, after, as long as the two are not the same.
     */
    void move()
    {
        if (m_currentPosition.load().y != m_destination.y)
        {
            m_currentPosition.exchange(Coordinate{
                m_currentPosition.load().x,
                m_currentPosition.load().y < m_destination.y
                    ? m_currentPosition.load().y + 1
                    : m_currentPosition.load().y - 1
            });
        }
        else if (m_currentPosition.load().x != m_destination.x)
        {
            m_currentPosition.exchange(Coordinate{
                m_currentPosition.load().x < m_destination.x
                    ? m_currentPosition.load().x + 1
                    : m_currentPosition.load().x - 1,
                m_currentPosition.load().y
            });
        }
    }
};

#endif //GRABSTATION_SIMULATEDARM_H
