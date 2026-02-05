//
// Created by luca on 04/02/26.
//

#ifndef GRABSTATION_KIOSKQUEUE_H
#define GRABSTATION_KIOSKQUEUE_H
#include <mutex>
#include <queue>

#include "commands/KioskCommand.h"

class ICommandQueue
{
protected:
    std::queue<KioskCommand> m_queue;

public:
    virtual ~ICommandQueue() = default;

    virtual void push(KioskCommand cmd) = 0;

    virtual KioskCommand pop() = 0;
};

#endif //GRABSTATION_KIOSKQUEUE_H
