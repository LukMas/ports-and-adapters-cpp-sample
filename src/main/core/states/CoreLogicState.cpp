//
// Created by luca on 02/02/26.
//

#include "CoreLogicState.h"

IKioskState* CoreLogicState::update(Kiosk& context, const std::string& cmd)
{
    if (cmd == "IDLE")
    {
        return &IdleState::getInstance();
    }

    return nullptr;
}