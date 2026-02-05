//
// Created by luca on 04/02/26.
//

#ifndef GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
#define GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
#include <condition_variable>

#include "ports/ICommandQueue.h"

class SynchronizedCommandQueue : public ICommandQueue
{
    std::queue<KioskCommand> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;

public:
    void push(KioskCommand cmd) override
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(cmd);
        }

        m_cv.notify_one();
    }

    KioskCommand pop() override
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return !m_queue.empty(); });

        KioskCommand cmd = m_queue.front();
        m_queue.pop();
        return cmd;
    }
};

#endif //GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
