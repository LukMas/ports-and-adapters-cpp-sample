//
// Created by luca on 10/02/26.
//

#include "AsyncLogger.h"

Logger::AsyncLogger::AsyncLogger(const std::string& filename)
{
    // I open the file as soon the object is created. I don't need to close it, it does it automatically
    m_file.open(filename, std::ios::app); // Open in "Append" mode
    if (!m_file.is_open())
    {
        throw std::runtime_error("Could not open log file!");
    }
}

void Logger::AsyncLogger::log(const std::string& message)
{
    // I don't need a unique_lock, I unlock once out
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(message);

    m_cv.notify_one();
}


void Logger::AsyncLogger::flush(std::stop_token& st)
{
    std::string message;

    // It runs all the time, without conditions, because...
    while (true)
    {
        // ... I'm going to put it sleeping as long nothing happens, but for this I need...
        {
            // ... a unique_lock, because the wait needs to unlock, and the stop token, that I get from the call
            std::unique_lock<std::mutex> lock(m_mutex);
            // now I wait either the awake from the notify or the stop token to change its state...
            m_cv.wait(lock, st, [this]
            {
                // I just check if the queue is empty, the stop token 'bypass' the normal behavior and automatically
                // wakes up the thread
                return !m_queue.empty();
            });

            // since I have a while true, I return as soon as the stop token changed and the queue is empty
            if (m_queue.empty() && st.stop_requested())
            {
                return;
            }

            // If the loop is still running, I check the queue...
            if (!m_queue.empty())
            {
                // ... and eventually I take the message out
                message = std::move(m_queue.front());
                m_queue.pop();
            }

            // the lock unlocks, reducing the time the two threads are 'interleaved'
        }

        // Here I perform any other action (normally something slow), that keeps this thread busy for a while, without
        // blocking any other thread
        if (m_file.is_open() && !message.empty())
        {
            m_file << message << std::endl;
        }
    }
}
