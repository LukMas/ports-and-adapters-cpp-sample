//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_ISTATUSLISTENER_H
#define GRABSTATION_ISTATUSLISTENER_H

#pragma once
/**
 * The enum that is used to share the status of the
 * machine.
 */
enum class MachineStatus { INITIALIZING, IDLE, PROCESSING, APPROACHING_ITEM, SECURING_ITEM, DELIVERING_ITEM,
    RELEASING_ITEM, STALLED};

inline std::string to_string(MachineStatus machineStatus)
{
    std::string result;

#pragma warning( push )
#pragma warning( default : 4061)
    switch (machineStatus)
    {
    case MachineStatus::INITIALIZING: result = "INITIALIZING";
        break;
    case MachineStatus::IDLE: result = "IDLE";
        break;
    case MachineStatus::PROCESSING: result = "PROCESSING";
        break;
    case MachineStatus::APPROACHING_ITEM: result = "APPROACHING_ITEM";
        break;
    case MachineStatus::SECURING_ITEM: result = "SECURING_ITEM";
        break;
    case MachineStatus::DELIVERING_ITEM: result = "DELIVERING_ITEM";
        break;
    case MachineStatus::RELEASING_ITEM: result = "RELEASING_ITEM";
        break;
    case MachineStatus::STALLED: result = "STALLED";
        break;
    }
#pragma warning ( pop )

    return result;
}

/**
 * The interface that defines a status listener. It is used
 * to inform any
 */
class IStatusListener
{
public:
    virtual ~IStatusListener() = default;
    virtual void onStatusChanged(MachineStatus newStatus) = 0;
};

#endif //GRABSTATION_ISTATUSLISTENER_H
