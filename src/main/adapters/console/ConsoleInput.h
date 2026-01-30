#ifndef GRABSTATION_CONSOLEINPUT_H

#include <stop_token>

#include "CommandQueue.h"
#include "ConsoleState.h"

/**
 * Interface used to catch the user input and push it in
 * the command queue.
 */
class ConsoleInput {
public:
    ConsoleInput(CommandQueue& q, ConsoleState& s) : m_queue(q), m_state(s) {}
    void run( const std::stop_source& stop_source); // This will be our thread function

private:
    CommandQueue& m_queue;
    ConsoleState& m_state;
};

#endif