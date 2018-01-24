#pragma once

#include "sensor.h"

class TempSensor : public Sensor
{
public:
    TempSensor()
        : Sensor("TemperatureSensor", "C", 30, -273.15f, 1000.0f, 10)
    {}

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