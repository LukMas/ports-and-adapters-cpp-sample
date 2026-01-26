//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_VIEWPORT_H
#define GRABSTATION_VIEWPORT_H
#pragma once
#include <string>

class IViewPort
{
public:
    virtual ~IViewPort() = default;
    virtual void displayState(const std::string& status) = 0;
    virtual void notifyError(const std::string& message) = 0;
    virtual void updateDisplay(const std::optional<std::string>::value_type& basic_string) = 0;
};

#endif //GRABSTATION_COMMANDSOURCE_H
