//
// Created by luca on 02/02/26.
//

#ifndef GRABSTATION_CORELOGICSTATE_H
#define GRABSTATION_CORELOGICSTATE_H

#include "States.h"


class CoreLogicState
{
    CoreLogicState() = default;

public:
    static IKioskState* update(Kiosk& context, const KioskCommand& cmd);

    CoreLogicState(const CoreLogicState&) = delete;
    CoreLogicState& operator=(const CoreLogicState&) = delete;
};


#endif //GRABSTATION_CORELOGICSTATE_H
