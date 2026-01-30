//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IDLE_STATE_H
#define GRABSTATION_IDLE_STATE_H

#pragma once
#include <memory>

class Kiosk;

class IKioskState
{
public:
    virtual ~IKioskState() = default;
    virtual std::unique_ptr<IKioskState> update(Kiosk& context, const std::string& cmd) = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
};

class IdleState : public IKioskState
{
    virtual std::unique_ptr<IKioskState> update(Kiosk& context, const std::string& cmd) override;
    [[nodiscard]] std::string getName() const override
    {
        return "Idle";
    }
};

class WaitingState : public IKioskState
{
    virtual std::unique_ptr<IKioskState> update(Kiosk& context, const std::string& cmd) override;
    [[nodiscard]] std::string getName() const override
    {
        return "Waiting";
    }
};

#endif //GRABSTATION_IDLESTATE_H
