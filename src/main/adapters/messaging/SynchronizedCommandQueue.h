//
// Created by luca on 04/02/26.
//

#ifndef GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
#define GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
#include <condition_variable>

#include "ports/ICommandQueue.h"

/**
 * An implementation of the ICommandQueue that is thread safe. The implementation
 * of the push and pop is made in a way that every thread that tries to pop is
 * put into pause as long as no command has been pushed. Once a commen is pushed
 * the queue wakes up the thread that could immediately pop the command.
 */
class SynchronizedCommandQueue : public ICommandQueue
{
    std::stop_token& stop_token;

    std::queue<KioskCommand> m_queue;
    std::mutex m_mutex;
    std::condition_variable_any m_cv;

public:
    /**
     * It creates an instance of the queue to be used to send commands to the Kiosk.
     * The queue offers 2 thread safe operations, push and pop. The pop puts the caller
     * in idle using the wait, while the push wakes it up using the notify_one.
     * @param stop_token the stop token is required to wake up any thread that has been
     * paused in the pop function.
     */
    SynchronizedCommandQueue(std::stop_token& stop_token) : stop_token(stop_token)
    {
    };

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

        m_cv.wait(lock, stop_token, [this]
        {
            return !m_queue.empty();
        });

        if (stop_token.stop_requested())
        {
            return KioskCommand{CommandType::STOP};
        }

        KioskCommand cmd = m_queue.front();
        m_queue.pop();
        return cmd;
    }
};

#endif //GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
