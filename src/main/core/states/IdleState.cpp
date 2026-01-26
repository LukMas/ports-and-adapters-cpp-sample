//
// Created by luca on 22/01/26.
//

#include "IdleState.h"
#include "../Kiosk.h"
#include <iostream>

#include "WaitingState.h"


// We only care about handleInput here because we are waiting for a "START" command
std::unique_ptr<IKioskState> IdleState::updateState(Kiosk& context, std::optional<std::string> cmd)
{
    if (cmd.value() == "START")
    {
        // 1. Prepare the UI for the user
        context.getView().updateDisplay("System Active. Please enter item coordinates.");
        context.getView().displayState(this->getName());

        // 2. Notify the Watchdog to start its 30s countdown
        context.notifyStatus(MachineStatus::WAITING);

        return std::make_unique<WaitingState>();

        // 3. Transition to the next state in your logic chain

        // return std::make_unique<WaitingForCoordsState>();
    }

    return nullptr;
}
