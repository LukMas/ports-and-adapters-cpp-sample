//
// Created by luca on 25/01/26.
//

#ifndef GRABSTATION_CONSOLESTATE_H
#define GRABSTATION_CONSOLESTATE_H

#include <string>
#include <mutex>
#include <atomic>

struct ConsoleState {
    std::string currentInput;  // The characters typed so far
    std::mutex mtx;            // Protects the string
    std::atomic<bool> running{true}; // Controls the loops
};

#endif