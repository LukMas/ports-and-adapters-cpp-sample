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
 * put into pause as long as no command has been pushed.
 *
 * The queue still uses a mutex to lock the access because the console runs on
 * another thread than the ones that handles the rest of the system.
 */
class SynchronizedCommandQueue : public ICommandQueue
{
    std::stop_token& stop_token;

    std::mutex m_mutex;
    std::condition_variable_any m_cv;

    std::queue<KioskCommand> m_queue;

public:
    /**
     * It creates an instance of the queue to be used to send commands to the Kiosk.
     * The queue offers 2 thread safe operations, push and pop. The push stores something in the queue,
     * while the pop allows to obtain it.
     * @param stop_token the stop token is required to wake up any thread that has been
     * paused in the pop function.
     */
    SynchronizedCommandQueue(std::stop_token& stop_token) : stop_token(stop_token)
    {
    };

    void push(KioskCommand cmd) override
    {
        // I activate the lock...
        std::lock_guard<std::mutex> lock(m_mutex);
        // ... and push into the queue
        m_queue.push(cmd);
    }

    std::optional<KioskCommand> pop() override
    {
        // to avoid an unnecessary long lock ...
        if (m_queue.empty())
        {
            return std::nullopt;
        }

        // Is not empty, so I lock for really short ...
        std::unique_lock<std::mutex> lock(m_mutex);

        // and return the command
        KioskCommand cmd = m_queue.front();
        m_queue.pop();
        
        return cmd;
    }
};

#endif //GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
