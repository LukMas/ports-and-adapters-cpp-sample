//
// Created by luca on 21/01/26.
//

#ifndef GRABSTATION_KIOSKSTATE_H
#define GRABSTATION_KIOSKSTATE_H

#pragma once
#include <string>
#include <memory>
#include <optional>

class Kiosk; // Forward declaration

class IKioskState
{
public:
    virtual ~IKioskState() = default;
    virtual std::unique_ptr<IKioskState> updateState(Kiosk& context, std::optional<std::string> cmd) = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
};

#endif //GRABSTATION_KIOSKSTATE_H
