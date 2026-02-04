//
// Created by luca on 04/02/26.
//

#ifndef GRABSTATION_STATECOMMANDS_H
#define GRABSTATION_STATECOMMANDS_H
#include <map>
#include <string>

// THIS SHOULD BE ELABORATED BETTER!
enum StateCommands
{
    READY,
    START,
    IDLE,
    UNKNOWN
};


inline std::map<std::string, int> tocode = {{"READY", 1}, {"START", 2}, {"IDLE", 3}};

inline StateCommands getStateCommand(const std::string& cmd)
{
    switch (tocode[cmd])
    {
    case 1:
        return START;
        break;
    case 2:
        return IDLE;
        break;
    case 3:
        return READY;
        break;
    default:
        return UNKNOWN;
        break;
    }
}

#endif //GRABSTATION_STATECOMMANDS_H
