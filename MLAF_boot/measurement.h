#pragma once

#include <list>
#include "plan.h"

struct MeasurementMetadata{
    const char* Id;
    double Min;
    double Max;
    std::list<Plan> Plans;
    
    MeasurementMetadata(const char* id, double minVal, double maxVal, std::list<Plan> plans) {
        Id = id;
        Min = minVal;
        Max = maxVal;
        Plans = plans;
    }
};