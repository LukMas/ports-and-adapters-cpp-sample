//
// Created by luca on 22/01/26.
//

#include "States.h"

#include "CoreLogicState.h"
#include "../Kiosk.h"


// It processes the command BOOT, moving the state machine to the
// IDLE.
IKioskState& InitializingState::handleCommand(Kiosk& context, const KioskCommand& cmd)
{
    auto nextState = kiosk::core::states::handleCoreLogic(context, cmd);
    if (nextState != nullptr)
    {
        return *nextState;
    }

    if (cmd.type == CommandType::READY)
    {
        return IdleState::getInstance();
    };

    return *this;
}

// It processes the command START and eventually changes to WAITING,
// otherwise it does nothing.
IKioskState& IdleState::handleCommand(Kiosk& context, const KioskCommand& cmd)
{
    auto nextState = kiosk::core::states::handleCoreLogic(context, cmd);
    if (nextState != nullptr)
    {
        return *nextState;
    }


    if (cmd.type == CommandType::START)
    {
        return ProcessingSelectionState::getInstance();
    }

    return *this;
}

// It checks the coordinates and if they are correct it pass
// them to the arm, changing the state to PROCESSING, otherwise
// it remains the same state.
IKioskState& ProcessingSelectionState::handleCommand(Kiosk& context, const KioskCommand& cmd)
{
    auto nextState = kiosk::core::states::handleCoreLogic(context, cmd);
    if (nextState != nullptr)
    {
        return *nextState;
    }


    if (cmd.type == CommandType::MOVE_TO)
    {
        std::optional<Coordinate> coordinate = context.validateCoordinates(cmd.payload);

        if (coordinate.has_value())
        {
            // and start the movement of the arm
            context.getArm().setDestination(coordinate.value());

            return ApproachingItemState::getInstance();
        }
    }

    return *this;
}

// It processes the transition from approaching item to selecting it.
IKioskState& ApproachingItemState::handleCommand(Kiosk& context, const KioskCommand& cmd)
{
    if (cmd.type == CommandType::SECURE_ITEM)
    {
        std::optional<Coordinate> coordinate = context.validateCoordinates(cmd.payload);

        if (coordinate.has_value())
        {
            // and start the movement of the arm
            context.getArm().setDestination(coordinate.value());

            // TODO: change to the correct state later
            return IdleState::getInstance();
        }
    }

    return *this;
}
