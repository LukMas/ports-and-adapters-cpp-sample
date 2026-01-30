//
// Created by luca on 21/01/26.
#include "Kiosk.h"
#include "CommandQueue.h"

#include "states/States.h"

// void Kiosk::initializeGrid(int rows, int cols) {
//     grid.resize(rows, std::vector<Slot>(cols, {"Empty", 0}));
// }

/**
 * Starting state is IDLE
 * @param q
 * @param v
 * @param a
 */
Kiosk::Kiosk(CommandQueue& q, IViewPort& v, IArmPort& a, int rows, int cols) :
    m_queue(q),
    m_view(v),
    m_arm(a),
    m_state(std::make_unique<IdleState>()),
    m_rows(rows),
    m_cols(cols)
{
}

void Kiosk::step()
{
    auto cmd = m_queue.try_pop();

    // I don't pass an empty commands
    if (cmd.has_value())
    {
        auto next = m_state->update(*this, cmd.value());
        if (next)
        {
            auto oldState =
                std::exchange(m_state, std::move(next));
            std::cout << "Transitioned from " << oldState->getName() << std::endl;
        }
    }
}


Kiosk::~Kiosk() = default;
