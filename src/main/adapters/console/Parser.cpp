//
// Created by luca on 05/02/26.
//

#include "Parser.h"

#include <sstream>

#include "ports/IArmPort.h"

std::optional<KioskCommand> Console::Parser::parse(const std::string& input)
{
    // for the future: this takes the input and puts it into the ss stream
    std::stringstream ss(input);
    // prepares an empty string to store the action
    std::string action;
    // the >> operator trims the ' ' in front and stops at the first ' ', filling the
    // action with the content it has read
    ss >> action;

    // Normalize to uppercase (**I've copied it, I just know it works**)
    for (auto& c : action)
    {
        // to avoid the noise that the IDE makes about the use of toupper!
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    // I process the actions now

    // It's a list of Ifs, but the console just simulates the interaction, it's not a state machine
    if (action == "START")
    {
        return KioskCommand(CommandType::START);
    }

    if (action == "MOVE")
    {
        // I expect the format MOVE A 2, or MOVE Z 33...
        char col; // 'A', 'B', etc.
        int row; // 1, 2, 3...

        if (ss >> col >> row)
        {
            // Transform 'A'->1, 'B'->2... 'Z'->26
            int x = std::toupper(col) - 'A' + 1;
            int y = row;

            // Check for basic alphabetic validity before sending
            if (x >= 1 && x <= 26)
            {
                return KioskCommand(CommandType::MOVE_TO, Coordinate(x, y));
            }
        }
        // if not, the syntax was incorrect and I generate no command
        return std::nullopt;
    }


    // no other commands, return null
    return std::nullopt;
}
