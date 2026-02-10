//
// Created by luca on 10/02/26.
//

#ifndef GRABSTATION_ILOGGER_H
#define GRABSTATION_ILOGGER_H
#include <string>

/**
 * The interface is responsible for logging the operations of the kiosk.
 */

class ILogger
{
public:
    /**
     * The function logs the message sent from the kiosk.
     * @param message message to store in the log
     */
    virtual void log(const std::string& message) = 0;

    virtual ~ILogger() = default;
};

#endif //GRABSTATION_ILOGGER_H
