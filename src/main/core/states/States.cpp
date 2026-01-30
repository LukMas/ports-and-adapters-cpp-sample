//
// Created by luca on 22/01/26.
//

#include "States.h"
#include "../Kiosk.h"

// class MovingState : public IKioskState
// {
//     std::unique_ptr<IKioskState> update(Kiosk& context, std::optional<std::string> cmd) override;
// };

std::unique_ptr<IKioskState> IdleState::update(Kiosk& context, const std::string& cmd)
{
    // if the command is START...
    if (cmd == "START")
    {
        // ... inform the view with a message
        context.getView().notifyMessage("System Active. Please enter item coordinates.");
        // ... and send the new status to all the listeners
        context.notifyListeners(MachineStatus::WAITING);

        return std::make_unique<WaitingState>();
    }

    return nullptr;
}

std::unique_ptr<IKioskState> WaitingState::update(Kiosk& context, const std::string& cmd)
{
    std::optional<Coordinate> coordinate = context.generateCoordinates(cmd);

    if (coordinate.has_value())
    {
        // ... inform the view with a message,
        context.getView().notifyMessage("System Moving. The arm is going to the selected position.");
        // send the new status to all the listeners,
        context.notifyListeners(MachineStatus::PROCESSING);
        // and start the movement of the arm
        context.getArm().setDestination(coordinate.value());

    }

    return nullptr;
}
