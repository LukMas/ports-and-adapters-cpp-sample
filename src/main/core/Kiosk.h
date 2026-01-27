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


class Kiosk
{
private:
    CommandQueue& m_queue;
    IViewPort& m_view;
    IArmPort& m_arm;
    std::unique_ptr<IKioskState> m_state;

    std::vector<IStatusListener*> m_statusListeners;

public:
    Kiosk(CommandQueue& q, IViewPort& v, IArmPort& a);

    void addStatusListener(IStatusListener* l)
    {
        m_statusListeners.push_back(l);
    }

    [[nodiscard]] IViewPort& getView() const
    {
        return m_view;
    }

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
