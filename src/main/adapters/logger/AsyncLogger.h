//
// Created by luca on 10/02/26.
//

#ifndef GRABSTATION_ASYNCLOGGER_H
#define GRABSTATION_ASYNCLOGGER_H

#include <condition_variable>
#include <fstream>
#include <memory>
#include <queue>

#include "ports/ILogger.h"

namespace Logger
{
    class AsyncLogger : public ILogger
    {
        std::queue<std::string> m_queue;

        std::mutex m_mutex;

        std::condition_variable_any m_cv;

        std::ofstream m_file; // The file handle

    public:
        AsyncLogger(const std::string& filename);

        ~AsyncLogger();

        void log(const std::string& message) override;

        void flush(std::stop_token& st);

        AsyncLogger& operator=(const AsyncLogger&) = delete;
    };
}


#endif //GRABSTATION_ASYNCLOGGER_H
