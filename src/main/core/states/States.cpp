//
// Created by luca on 22/01/26.
//

#include "States.h"
#include "../Kiosk.h"

// class MovingState : public IKioskState
// {
//     std::unique_ptr<IKioskState> update(Kiosk& context, std::optional<std::string> cmd) override;
// };

std::unique_ptr<IKioskState> IdleState::update(Kiosk& context, std::optional<std::string> cmd)
{
    if (cmd.value() == "START")
    {
        context.getView().notifyState("System Active. Please enter item coordinates.");
        context.getView().notifyState(this->getName());

        context.notifyStatus(MachineStatus::WAITING);

        return std::make_unique<WaitingState>();
    }

    return nullptr;
}

[[nodiscard]] std::string IdleState::getName() const
{
    return "IDLE";
}

std::unique_ptr<IKioskState> WaitingState::update(Kiosk& context, std::optional<std::string> cmd)
{
    if (cmd.value() == "IDLE")
    {
        // for now I do nothing, so the watchdog kiks in
        return std::make_unique<IdleState>();
    }

    return nullptr;
}


[[nodiscard]] std::string WaitingState::getName() const
{
    return "WAITING";
}
