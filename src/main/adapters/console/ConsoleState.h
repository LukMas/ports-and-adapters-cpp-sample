//
// Created by luca on 25/01/26.
//

#ifndef GRABSTATION_CONSOLESTATE_H
#define GRABSTATION_CONSOLESTATE_H

#include <string>
#include <mutex>

/**
 * Structure used to carry the current input and
 * the status of the machine.
 */
struct ConsoleState {
    /**
     * Used to regulate the access
     */
    std::mutex mtx;            // Protects the string
    std::string currentInput;  // The characters typed so far
};

#endif