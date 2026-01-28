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
enum class MachineStatus { IDLE, PROCESSING, WAITING };

inline std::string to_string(MachineStatus machineStatus)
{
    std::string result;

#pragma warning( push )
#pragma warning( default : 4061)
    switch (machineStatus)
    {
    case MachineStatus::IDLE: result = "IDLE";
        break;
    case MachineStatus::WAITING: result = "WAITING";
        break;
    case MachineStatus::PROCESSING: result = "PROCESSING";
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
