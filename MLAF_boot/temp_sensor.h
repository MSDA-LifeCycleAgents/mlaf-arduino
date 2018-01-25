#pragma once

#include "sensor.h"

class TempSensor : public Sensor
{
public:
    TempSensor()
        : Sensor("TemperatureSensor",
            "Temperature sensor",
            "C", 10, 10)
    {
        Plan p{PlanType::Both, -50, 100.0f, "Are you sure this is alright?", "Slackagent", "#general", 5};
        MeasurementMetadata mdata{"temp", -273.15f, 1000.0f, std::list<Plan>{p}};
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
        return std::map<const char*, double> {{"Temperature", 20}};
    }
};