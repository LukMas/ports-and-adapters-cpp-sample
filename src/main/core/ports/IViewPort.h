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
     * The function used by the Kiosk to notify a message to the view port. The implementation should copy
     * the content into its own stack.
     * @param status the string with the name of the state, it should be copied in the handler
     */
    virtual void notifyMessage(std::string status) = 0;

    /**
     * The function used to notify eventual errors of the Kiosk. The implementation should copy the content into
     * its own stack.
     * @param message the error message
     */
    virtual void notifyError(std::string message) = 0;
};

#endif //GRABSTATION_COMMANDSOURCE_H
