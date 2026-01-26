//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IDLESTATE_H
#define GRABSTATION_IDLESTATE_H
#include "IKioskState.h"

#pragma once

class IdleState : public IKioskState {
public:
    std::unique_ptr<IKioskState> updateState(Kiosk& context, std::optional<std::string> cmd) override;
    [[nodiscard]] std::string getName() const override { return "IDLE"; }
};

#endif //GRABSTATION_IDLESTATE_H