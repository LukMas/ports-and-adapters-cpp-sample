//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IDLE_STATE_H
#define GRABSTATION_IDLE_STATE_H

#pragma once
#include <memory>

#include "IStatusListener.h"
#include "commands/KioskCommand.h"

class Kiosk;

/**
 * Base interface, defines the state machine and the related transactions
 * that control it.
 */
class IKioskState
{
public:
    IKioskState() = default;

    virtual ~IKioskState() = default;

    /**
     * The function takes the context and the command and returns the next
     * state, if the current state can handle it, or the same state if the
     * command cannot be handled.
     * The first call is to the core logic state, to check if the default
     * routes should be selected. If not, the update, that each state must
     * override, is called.
     * @param context used to process the command (as reference being the
     * handler of the result of the function)
     * @param cmd the kiosk command (as reference because it's extracted in
     * the Kiosk and completely processed there)
     * @return the next state, or the current one
     */
    [[nodiscard]] virtual IKioskState& handleCommand(Kiosk& context, KioskCommand& cmd);

    /**
     * The function returns a descriptive message related to the state.
     * @return description of the state
     */
    [[nodiscard]] virtual std::string getMessage() = 0;

    /**
     * The function returns the enum that defines the state of the machine.
     * @return a value of the MachineStatus enum
     */
    [[nodiscard]] virtual MachineStatus getStatus() const = 0;

    // This makes it impossible to accidentally create a 'temp' copy
    IKioskState(const IKioskState&) = delete;
    IKioskState& operator=(const IKioskState&) = delete;

protected:
    /**
     * The function takes the context and the command and returns the next
     * state, if the current state can handle it, or the same state if the
     * command cannot be handled.
     * The default implementation of the function returns the current state.
     * @param context used to process the command
     * @param cmd the command, with the payload if any
     * @return the next state, or the current one
     */
    virtual IKioskState& update(Kiosk& context, KioskCommand& cmd)
    {
        return *this;
    };
};

/**
 * It defines the first state, the one assigned to the Kiosk at the start.
 * It manages the transitions:
 * BOOT -> IDLE
 * Global commands IDLE, STOP are handled by CoreLogic.
 */
class BootState : public IKioskState
{
    BootState() = default;

    IKioskState& update(Kiosk& context, KioskCommand& cmd) override;

public:
    // This makes it impossible to accidentally create a 'temp' copy
    BootState(const BootState&) = delete;
    BootState& operator=(const BootState&) = delete;


    [[nodiscard]] std::string getMessage() override
    {
        return "System booting...";
    }

    [[nodiscard]] MachineStatus getStatus() const override
    {
        return MachineStatus::BOOTING;
    }

    static BootState& getInstance()
    {
        static BootState state;
        return state;
    }
};

/**
 * It defines the idle state, the one the Kiosk has when it does
 * nothing and waits for the user input.
 * It manages the transitions:
 * IDLE -> WAITING
 * Global commands IDLE, STOP are handled by CoreLogic.
 */
class IdleState : public IKioskState
{
    IdleState() = default;

protected:
    IKioskState& update(Kiosk& context, KioskCommand& cmd) override;

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


/**
 * It defines the waiting state, that's the state the Kiosk has when
 * the user has started the interaction and waits for the coordinates
 * of the item.
 * It manages the transitions:
 * WAITING -> PROCESSING
 * Global commands IDLE, STOP are handled by CoreLogic.
 */
class WaitingState : public IKioskState
{
    WaitingState() = default;

protected:
    IKioskState& update(Kiosk& context, KioskCommand& cmd) override;

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
