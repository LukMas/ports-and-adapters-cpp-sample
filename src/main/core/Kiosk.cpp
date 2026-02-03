//
// Created by luca on 21/01/26.
#include "Kiosk.h"
#include "CommandQueue.h"

#include "states/States.h"

Kiosk::Kiosk(CommandQueue& q, IViewPort& v, IArmPort& a, int rows, int cols) :
    m_queue(q),
    m_view(v),
    m_arm(a),
    m_currentState(&BootState::getInstance()),
    m_rows(rows),
    m_cols(cols)
{
    m_view.notifyMessage(m_currentState->getMessage());
}

void Kiosk::step()
{
    auto cmd = m_queue.try_pop();

    // I don't pass an empty commands
    if (cmd.has_value())
    {
        IKioskState& nextState = m_currentState->handleCommand(*this, cmd.value());
        if (&nextState != m_currentState)
        {
            // there's a transaction...
            std::cout << "Transitioned from " << to_string(m_currentState->getStatus()) <<
                " to "<< to_string(nextState.getStatus()) << std::endl;

            // state changes...
            m_currentState = &nextState;

            // Information broadcasting
            m_view.notifyMessage(m_currentState->getMessage());
            // send the new status to all the listeners,
            this->notifyListeners(m_currentState -> getStatus());
        }
    }
}


Kiosk::~Kiosk() = default;
