//
// Created by luca on 22/01/26.
//

#include "States.h"

#include "CoreLogicState.h"
#include "../Kiosk.h"

IKioskState& IKioskState::handleCommand(Kiosk& context, const KioskCommand& cmd)
{
    auto nextState = CoreLogicState::update(context, cmd);

    if (nextState != nullptr)
    {
        return *nextState;
    }

    return update(context, cmd);
}

// It processes the command BOOT, moving the state machine to the
// IDLE.
IKioskState& BootState::update(Kiosk& context, const KioskCommand& cmd)
{
    if (cmd.type == CommandType::READY)
    {
        return IdleState::getInstance();
    };

    return *this;
}

// It processes the command START and eventually changes to WAITING,
// otherwise it does nothing.
IKioskState& IdleState::update(Kiosk& context, const KioskCommand& cmd)
{
    if (cmd.type == CommandType::START)
    {
        return WaitingState::getInstance();
    }

    return *this;
}

// It checks the coordinates and if they are correct it pass
// them to the arm, changing the state to PROCESSING, otherwise
// it remains the same state.
IKioskState& WaitingState::update(Kiosk& context, const KioskCommand& cmd)
{
    if (cmd.type == CommandType::MOVE_TO)
    {
        std::optional<Coordinate> coordinate = context.validateCoordinates(cmd.payload);

        if (coordinate.has_value())
        {
            // and start the movement of the arm
            context.getArm().setDestination(coordinate.value());
        }
    }

    return *this;
}
