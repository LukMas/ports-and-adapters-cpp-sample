//
// Created by luca on 23/01/26.
//

#ifndef GRABSTATION_WAITINGSTATE_H
#define GRABSTATION_WAITINGSTATE_H
#include "IKioskState.h"


class WaitingState : public IKioskState
{
public:
    std::unique_ptr<IKioskState> updateState(Kiosk& context, std::optional<std::string> cmd) override;
    [[nodiscard]] std::string getName() const override { return "WAITING"; }
};


#endif //GRABSTATION_WAITINGSTATE_H
