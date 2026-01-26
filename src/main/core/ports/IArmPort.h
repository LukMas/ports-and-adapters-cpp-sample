//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_ARMPORT_H
#define GRABSTATION_ARMPORT_H
#pragma once

class IArmPort
{
public:
    virtual ~IArmPort() = default;

    // The Core calls this to start the journey
    virtual void moveTo(int targetX, int targetY) = 0;
};
#endif //GRABSTATION_ARMPORT_H
