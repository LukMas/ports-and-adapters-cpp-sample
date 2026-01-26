//
// Created by luca on 23/01/26.
//

#include "WaitingState.h"

std::unique_ptr<IKioskState> WaitingState::updateState(Kiosk& context, std::optional<std::string> cmd)
{
    if (cmd.value() == "IDLE")
    {
        // for now I do nothing, so the watchdog kiks in
        return std::unique_ptr<IKioskState>(this);
    }

    return nullptr;
}
