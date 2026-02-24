//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_ARMPORT_H
#define GRABSTATION_ARMPORT_H
#pragma once

struct Coordinate
{
    float x;
    float y;
};

/**
 * The IArmPort defines the set of functions that allow the Kiosk to
 * move the arm from the coordinates of the idle position to the ones
 * set in the destination.
 */
class IArmPort
{
public:
    virtual ~IArmPort() = default;

    /**
     * The function allows to set the destination coordinates.
     * @param destination the object that stores the destination coordinates
     */
    virtual void setDestination(const Coordinate& destination) = 0;

    /**
     * The function returns the current coordinates of the arm.
     * @return the object that stores the current coordinates
     */
    [[nodiscard]] virtual Coordinate getCurrentPosition() const = 0;

    /**
     * The function informs the caller if the arm has reached the
     * destination coordinates.
     * @return true if the arm has reached the destination coordinates,
     * false otherwise
     */
    [[nodiscard]] virtual bool hasReachedTarget() const = 0;
};
#endif //GRABSTATION_ARMPORT_H
