//
// Created by luca on 02/02/26.
//

#include "CoreLogicState.h"

IKioskState* kiosk::core::states::handleCoreLogic(Kiosk& context, const KioskCommand& cmd)
{
    if (cmd.type == CommandType::IDLE)
    {
        return &IdleState::getInstance();
    }

    return nullptr;
}