//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_ARMPORT_H
#define GRABSTATION_ARMPORT_H
#pragma once

struct Coordinate {
    int x;
    int y;
};

class IArmPort {
public:
    virtual ~IArmPort() = default;

    // Commands
    virtual void setDestination(const Coordinate& destination) = 0;
    virtual void emergencyStop() = 0;

    // Queries
    [[nodiscard]] virtual Coordinate getCurrentPosition() const = 0;
    [[nodiscard]] virtual bool hasReachedTarget() const = 0;
};
#endif //GRABSTATION_ARMPORT_H
