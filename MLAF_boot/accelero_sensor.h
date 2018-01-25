#pragma once

#include "sensor.h"

class AcceleroSensor : public Sensor
{
public:
    AcceleroSensor()
        : Sensor("AcceleroSensor",
            "Accelero sensor",
            "G", 10, 10)
    {
        Plan p{PlanType::Both, -100, 100.0f, "Too fast!", "Slackagent", "#general", 5};
        MeasurementMetadata mdata{"accelero", -1000.0f, 1000.0f, std::list<Plan>{p}};
        addMeasurementMetadata(mdata);
    }

    virtual bool activate() override
    {
        return true;
    }

    virtual bool deactivate() override
    {
        return true;
    }

    virtual const std::map<const char*, double> getData() override
    {
        return std::map<const char*, double> {{"X", 0}, {"Y", 1}, {"Z", 2}};
    }
};