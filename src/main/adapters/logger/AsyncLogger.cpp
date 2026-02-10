//
// Created by luca on 10/02/26.
//

#include "AsyncLogger.h"

Logger::AsyncLogger::AsyncLogger(const std::string& filename)
{
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

    while (!st.stop_requested())
    {
        // I need a unique_lock because the wait needs to unlock
        std::unique_lock<std::mutex> lock(m_mutex);

        {
            m_cv.wait(lock, st, [this]
            {
                return !m_queue.empty();
            });

            if (m_queue.empty() && st.stop_requested())
            {
                return;
            }

            if (!m_queue.empty())
            {
                message = std::move(m_queue.front());
                m_queue.pop();
            }
        }

        if (m_file.is_open() && !message.empty())
        {
            m_file << message << std::endl;
        }
    }
}


Logger::AsyncLogger::~AsyncLogger()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}
