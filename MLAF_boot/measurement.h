#pragma once

#include <list>
#include "plan.h"

/**
 * @brief      Contains (sub)sensor definitions
 */
struct MeasurementMetadata{
    /**
     * @brief      The id of the (sub)sensor
     */
    String Id;
    /**
     * @brief      The lowest value the (sub)sensor is able to measure
     */
    double Min;
    /**
     * @brief      The highest value the (sub)sensor is able to measure
     */
    double Max;
    /**
     * @brief      The plans associated with the (sub)sensor
     */
    std::list<Plan> Plans;

    /**
     * @brief      The type of threshold to use
     * @param[in]  id      the id of the (sub)sensor
     * @param[in]  minVal  the lowest value the (sub)sensor is able to measure 
     * @param[in]  maxVal  the highest value the (sub)sensor is able to measure
     * @param[in]  plans   the plans associated with the (sub)sensor
     */
    MeasurementMetadata(String id, double minVal, double maxVal, std::list<Plan> plans) {
        Id = id;
        Min = minVal;
        Max = maxVal;
        Plans = plans;
    }
};