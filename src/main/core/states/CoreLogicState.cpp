//
// Created by luca on 02/02/26.
//

#include "CoreLogicState.h"

IKioskState* CoreLogicState::update(Kiosk& context, KioskCommand& cmd)
{
    if (cmd.type == CommandType::IDLE)
    {
        return &IdleState::getInstance();
    }

    return nullptr;
}