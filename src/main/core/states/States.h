//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IDLE_STATE_H
#define GRABSTATION_IDLE_STATE_H

#pragma once
#include <memory>

#include "IStatusListener.h"

class Kiosk;


class IKioskState
{
public:
    IKioskState() = default;

    virtual ~IKioskState() = default;
    virtual IKioskState& update(Kiosk& context, const std::string& cmd) = 0;

    [[nodiscard]] virtual std::string getMessage() = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
    [[nodiscard]] virtual MachineStatus getStatus() const = 0;

    // This makes it impossible to accidentally create a 'temp' copy
    IKioskState(const IKioskState&) = delete;
    IKioskState& operator=(const IKioskState&) = delete;
};

class IdleState : public IKioskState
{
    IdleState() = default;

    IKioskState& update(Kiosk& context, const std::string& cmd) override;

    [[nodiscard]] std::string getName() const override
    {
        return "Idle";
    }

public:
    // This makes it impossible to accidentally create a 'temp' copy
    IdleState(const IdleState&) = delete;
    IdleState& operator=(const IdleState&) = delete;


    [[nodiscard]] std::string getMessage() override
    {
        return "System idle.";
    }

    [[nodiscard]] MachineStatus getStatus() const override
    {
        return MachineStatus::IDLE;
    }

    static IdleState& getInstance()
    {
        static IdleState state;
        return state;
    }
};

class WaitingState : public IKioskState
{
    WaitingState() = default;

    IKioskState& update(Kiosk& context, const std::string& cmd) override;

    [[nodiscard]] std::string getName() const override
    {
        return "Waiting";
    }

public:
    // This makes it impossible to accidentally create a 'temp' copy
    WaitingState(const WaitingState&) = delete;
    WaitingState& operator=(const WaitingState&) = delete;

    [[nodiscard]] std::string getMessage() override
    {
        return "System activated, waiting for coordinates.";
    }

    [[nodiscard]] MachineStatus getStatus() const override
    {
        return MachineStatus::WAITING;
    }

    static WaitingState& getInstance()
    {
        static WaitingState state;
        return state;
    }
};

#endif //GRABSTATION_IDLESTATE_H
