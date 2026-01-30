//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_VIEWPORT_H
#define GRABSTATION_VIEWPORT_H
#pragma once
#include <string>

/**
 * The IArmPort is the adapter with the functions that are used
 * to publish the current state of the Kiosk.
 */
class IViewPort
{
public:
    virtual ~IViewPort() = default;

    /**
     * The function used by the Kiosk to notify a message to
     * the view port.
     * @param status the string with the name of the state
     */
    virtual void notifyMessage(const std::string& status) = 0;

    /**
     * The function used to notify eventual errors of the Kiosk.
     * @param message the error message
     */
    virtual void notifyError(const std::string& message) = 0;
};

#endif //GRABSTATION_COMMANDSOURCE_H
