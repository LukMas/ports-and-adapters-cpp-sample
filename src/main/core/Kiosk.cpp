//
// Created by luca on 21/01/26.
#include "Kiosk.h"
#include "states/IKioskState.h"
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
Kiosk::Kiosk(CommandQueue& q, IViewPort& v, IArmPort& a) :
    m_queue(q),
    m_view(v),
    m_arm(a),
    m_state(std::make_unique<IdleState>())
{
}

Kiosk::~Kiosk()
= default;
