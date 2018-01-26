#pragma once

#include "sensor.h"

/**
 * \brief An example sensor that returns constant data
 */
class TempSensor : public Sensor
{
public:
    /**
     * \brief Creates an example sensor that returns constant data
     */
    TempSensor()
        : Sensor("TemperatureSensor", "C", 30, -273.15f, 1000.0f, 10)
    {}

    /**
     * \brief Enables the sensor
     * 
     * \return <i>true</i>
     */
    virtual bool activate() override
    {
        return true;
    }

    /**
     * \brief Disables the sensor
     * 
     * \return <i>true</i>
     */
    virtual bool deactivate() override
    {
        return true;
    }

    /**
     * \brief Gets data from the sensor
     * 
     * \return a map (key value pair/dictionary) of sensor data
     */
    virtual const std::map<const char*, double> getData() override
    {
        return std::map<const char*, double> {{"Temperature", 20}};
    }
};