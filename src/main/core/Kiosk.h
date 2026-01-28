//
// Created by luca on 21/01/26.
//

#ifndef GRABSTATION_KIOSK_H
#define GRABSTATION_KIOSK_H

#pragma once
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>

#include <vector>

#include "CommandQueue.h"
#include "IStatusListener.h"
#include "states/IKioskState.h"
#include "ports/IArmPort.h"
#include "ports/IViewPort.h"
#include "states/States.h"

/**
 * The class that handles the logic of the kiosk machine.
 * It checks the content of the CommandQueue, and when a
 * command is found it passes it to the actual state.
 *
 * The states are modeled in a way that each one is able to
 * manage only a given set of commands, avoiding any type of
 * unwanted transaction.
 *
 * The Kiosk class uses two ports:
 * - the IViewPort is used to inform about the internal status
 * of the Kiosk (to say a message or show an error)
 * - the IArmPort is used for the interaction with the Arm
 * when the state requires its movement
 *
 * The registred observers are used to notify any external service
 * about the changes in the states of the Kiosk.
 */
class Kiosk
{
private:
    CommandQueue& m_queue;
    IViewPort& m_view;
    IArmPort& m_arm;
    std::unique_ptr<IKioskState> m_state;

    std::vector<IStatusListener*> m_statusListeners;

public:
    /**
     * Constructor, it requires the queue for the commands,
     * the view port to output the status and the arm port
     * to move the arm.
     * @param q the instance of the queue
     * @param v the view port
     * @param a the arm port
     */
    Kiosk(CommandQueue& q, IViewPort& v, IArmPort& a);

    void addStatusListener(IStatusListener* l)
    {
        m_statusListeners.push_back(l);
    }

    /**
     * Accessor to obtain the view port
     * @return the instance of the view port
     */
    [[nodiscard]] IViewPort& getView() const
    {
        return m_view;
    }

    /**
     * The function that is called at every cycle, it
     * runs the state machine obtained the next command
     * from the queue.
     */
    void step()
    {
        auto cmd = m_queue.try_pop();

        // I don't pass an empty commands
        if (cmd.has_value())
        {
            auto next = m_state->update(*this, cmd);
            if (next)
            {
                auto oldState =
                    std::exchange(m_state, std::move(next));
                std::cout << "Transitioned from " << oldState->getName() << std::endl;
            }
        }
    }

    /**
     * The function that notifies the status of the machine
     * to every registered observer.
     * @param s the status of the machine
     */
    void notifyStatus(MachineStatus s) const
    {
        for (auto* l : m_statusListeners)
        {
            l->onStatusChanged(s);
        }
    }

    ~Kiosk();
};

#endif
