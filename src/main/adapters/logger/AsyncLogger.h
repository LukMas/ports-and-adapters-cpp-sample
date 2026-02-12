//
// Created by luca on 10/02/26.
//

#ifndef GRABSTATION_ASYNCLOGGER_H
#define GRABSTATION_ASYNCLOGGER_H

#include <condition_variable>
#include <fstream>
#include <queue>

#include "ports/ILogger.h"

namespace Logger
{
    class AsyncLogger : public ILogger
    {
        std::mutex m_mutex;
        std::condition_variable_any m_cv;

        std::queue<std::string> m_queue;

        std::ofstream m_file;

    public:
        explicit AsyncLogger(const std::string& filename);

        void log(const std::string& message) override;

        void flush(std::stop_token& st);

        AsyncLogger& operator=(const AsyncLogger&) = delete;

        ~AsyncLogger() override = default;
    };
}


#endif //GRABSTATION_ASYNCLOGGER_H
