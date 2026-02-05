//
// Created by luca on 05/02/26.
//

#ifndef GRABSTATION_PARSER_H
#define GRABSTATION_PARSER_H
#include <optional>

#include "commands/KioskCommand.h"


namespace Console::Parser
{
    std::optional<KioskCommand> parse(const std::string& input);
}


#endif //GRABSTATION_PARSER_H
