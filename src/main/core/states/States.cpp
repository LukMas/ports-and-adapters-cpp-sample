//
// Created by luca on 22/01/26.
//

#include "States.h"

#include "CoreLogicState.h"
#include "../Kiosk.h"


IKioskState& IdleState::update(Kiosk& context, const std::string& cmd)
{
    // if the command is START...
    if (cmd == "START")
    {
        // ... inform the view with a message
        context.getView().notifyMessage("System Active. Please enter item coordinates.");
        // ... and send the new status to all the listeners
        context.notifyListeners(MachineStatus::WAITING);

        return WaitingState::getInstance();
    }

    return *this;
}

IKioskState& WaitingState::update(Kiosk& context, const std::string& cmd)
{
    auto nextState = CoreLogicState::update(context, cmd);

    if (nextState != nullptr)
    {
        return *nextState;
    }

    // if the core handler doesn't know the command...
    // ... it should be one that the waiting state can manage
    std::optional<Coordinate> coordinate = context.generateCoordinates(cmd);

    if (coordinate.has_value())
    {
        // and start the movement of the arm
        context.getArm().setDestination(coordinate.value());
    }

    return *this;
}
