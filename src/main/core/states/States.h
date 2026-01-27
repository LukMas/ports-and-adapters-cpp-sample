//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IDLESTATE_H
#define GRABSTATION_IDLESTATE_H
#include "IKioskState.h"

#pragma once

class IKioskState
{
public:
    virtual ~IKioskState() = default;
    virtual std::unique_ptr<IKioskState> update(Kiosk& context, std::optional<std::string> cmd) = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
};

class IdleState : public IKioskState
{
    std::unique_ptr<IKioskState> update(Kiosk& context, std::optional<std::string> cmd) override;
    [[nodiscard]] std::string getName() const override;
};

class WaitingState : public IKioskState
{
    std::unique_ptr<IKioskState> update(Kiosk& context, std::optional<std::string> cmd) override;
    [[nodiscard]] std::string getName() const override;
};

#endif //GRABSTATION_IDLESTATE_H
