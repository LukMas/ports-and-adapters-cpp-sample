//
// Created by luca on 21/01/26.
//
#ifndef GRABSTATION_COMMANDQUEUE_H
#define GRABSTATION_COMMANDQUEUE_H

#pragma once

#include <queue>
#include <mutex>
#include <string>
#include <optional>

class CommandQueue
{
private:
    std::queue<std::string> queue;
    std::mutex mtx;

public:
    CommandQueue() = default;

    void push(const std::string& cmd)
    {
        // lock_guard automatically unlocks when the function ends
        std::lock_guard lock(mtx);
        queue.push(cmd);
    }

    std::optional<std::string> try_pop()
    {
        std::lock_guard lock(mtx);
        if (queue.empty())
        {
            return std::nullopt;
        }

        std::string cmd = queue.front();
        queue.pop();
        return cmd;
    }


    // Delete the two functions to avoid any chance to copy objects that should be used by reference
    CommandQueue(const CommandQueue&) = delete;
    CommandQueue& operator=(const CommandQueue&) = delete;
};

#endif
