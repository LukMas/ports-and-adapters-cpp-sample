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

    std::mutex m_mutex;
    std::condition_variable_any m_cv;

    std::queue<KioskCommand> m_queue;

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
            // I activate the lock...
            std::lock_guard<std::mutex> lock(m_mutex);
            // ... and push into the queue
            m_queue.push(cmd);
        }

        // I notify any sleeping thread that the queue has something
        m_cv.notify_one();
    }

    KioskCommand pop() override
    {
        // I just need it for later...
        KioskCommand cmd = KioskCommand{CommandType::IDLE};

        {
            // I activate the lock...
            std::unique_lock<std::mutex> lock(m_mutex);

            // and I put the thread into idle using the condition variable.
            // Here I wait for notify, and then I check the queue, or for a change in the stop token. When one of the two
            // happens, I lock the mutex and go to the next instruction
            m_cv.wait(lock, stop_token, [this]
            {
                // if  notify comes and the queue is empty I simply sleep again
                return !m_queue.empty();
            });

            // If the stop token has been requested...
            if (stop_token.stop_requested())
            {
                // ... I shut down the system sending one last message
                return KioskCommand{CommandType::STOP};
            }


            // I take the command and pop the queue...
            cmd = m_queue.front();
            m_queue.pop();
        }

        // Outside the lock, reducing the 'lock' size, I return the command
        return cmd;
    }
};

#endif //GRABSTATION_SYNCHRONIZEDCOMMANDQUEUE_H
