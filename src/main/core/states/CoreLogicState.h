//
// Created by luca on 02/02/26.
//

#ifndef GRABSTATION_CORELOGICSTATE_H
#define GRABSTATION_CORELOGICSTATE_H

#include "States.h"

namespace kiosk::core::states
{
    IKioskState* handleCoreLogic(Kiosk& context, const KioskCommand& cmd);
}


#endif //GRABSTATION_CORELOGICSTATE_H
