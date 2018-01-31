#pragma once

#include <list>
#include "plan.h"

struct MeasurementMetadata{
    String Id;
    double Min;
    double Max;
    std::list<Plan> Plans;
    
    MeasurementMetadata(String id, double minVal, double maxVal, std::list<Plan> plans) {
        Id = id;
        Min = minVal;
        Max = maxVal;
        Plans = plans;
    }
};