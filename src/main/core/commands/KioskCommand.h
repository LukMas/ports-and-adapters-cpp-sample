//
// Created by luca on 04/02/26.
//

#ifndef GRABSTATION_KIOSKCOMMANDS_H
#define GRABSTATION_KIOSKCOMMANDS_H
#include <string>
#include <utility>

#include "ports/IArmPort.h"


enum class CommandType
{
    READY,
    IDLE,
    START,
    MOVE_TO,
    SECURE_ITEM,
    STOP
};

struct KioskCommand
{
    CommandType type;
    Coordinate payload;

    explicit KioskCommand(CommandType commandType) : type(commandType), payload()
    {
    }

    KioskCommand(CommandType commandType, Coordinate payloadContent) : type(commandType),
                                                                       payload(payloadContent)
    {
    }
};

#endif //GRABSTATION_KIOSKCOMMANDS_H
