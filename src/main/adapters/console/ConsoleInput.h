#ifndef GRABSTATION_CONSOLEINPUT_H
#define CONSOLE_INPUT_H

#include "CommandQueue.h"
#include "ConsoleState.h"
z

class ConsoleInput {
public:
    ConsoleInput(CommandQueue& q, ConsoleState& s) : m_queue(q), m_state(s) {}
    void run(); // This will be our thread function

private:
    CommandQueue& m_queue;
    ConsoleState& m_state;
};

#endif