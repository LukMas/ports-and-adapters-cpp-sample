//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IACTIVITYLISTENER_H
#define GRABSTATION_IACTIVITYLISTENER_H

#pragma once
class IActivityListener {
public:
    virtual ~IActivityListener() = default;
    virtual void onUserActivity() = 0;
};

#endif //GRABSTATION_IACTIVITYLISTENER_H