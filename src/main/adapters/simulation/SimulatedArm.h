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

    Coordinate m_end{0.0f, 0.0f};
    Coordinate m_start{0.0f, 0.0f};

    // It allows to atomically read the values. Moreover, the reader could read not up-to-date values,
    // but being used in a console to show a simulated arm it's not important.
    std::atomic<Coordinate> m_currentPosition{Coordinate{0.0f, 0.0f}};

    std::chrono::steady_clock::time_point startTime;
    float durationSeconds = 0.0f;
    bool isMoving = false;

public:
    static inline float MAX_SPEED = 1.0f;

    explicit SimulatedArm() = default;

    void setDestination(const Coordinate& destination) override
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            // perform a fast copy of the values, then returns
            m_end = destination;
        }

        // I keep a copy of the start position for the calculations
        m_start = m_currentPosition.load();

        const float distance = std::hypot(m_end.x - m_currentPosition.load().x,
                                          m_end.y - m_currentPosition.load().y);

        this->durationSeconds = distance / MAX_SPEED;

        this->startTime = std::chrono::steady_clock::now();
        this->isMoving = true;
    }

    [[nodiscard]] Coordinate getCurrentPosition() const override
    {
        // I extract from the atomic the value
        return m_currentPosition.load();
    }

    [[nodiscard]] bool hasReachedTarget() const override
    {
        return !isMoving;
    };

    /**
     * The function simulates a linear movement of the arm using a constant velocity.
     */
    void move()
    {
        if (!isMoving) return;

        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> elapsed = now - startTime;
        const float t = elapsed.count() / durationSeconds;

        if (t >= 1.0f)
        {
            m_currentPosition.store(m_end);
            isMoving = false;
        }
        else
        {
            const float nextX = m_start.x + (m_end.x - m_start.x) * t;
            const float nextY = m_start.y + (m_end.y - m_start.y) * t;

            m_currentPosition.store(Coordinate{nextX, nextY});
        }
    }
};

#endif //GRABSTATION_SIMULATEDARM_H
