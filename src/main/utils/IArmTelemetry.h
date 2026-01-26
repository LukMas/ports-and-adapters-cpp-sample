//
// Created by luca on 22/01/26.
//

#ifndef GRABSTATION_IARMTELEMETRY_H
#define GRABSTATION_IARMTELEMETRY_H

#pragma once
class IArmTelemetry {
public:
    virtual ~IArmTelemetry() = default;
    virtual void onPositionChanged(int x, int y) = 0;
};

#endif //GRABSTATION_IARMTELEMETRY_H