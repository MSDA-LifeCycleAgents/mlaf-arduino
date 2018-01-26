#pragma once

#include "sensor.h"

/**
 * \brief An example sensor that measures voltage on a given pin
 */
class ADCSensor : public Sensor
{
public:
    /**
     * \brief Creates an ADC-sensor
     * 
     * \param pin the pin to measure
     */
    ADCSensor(uint8_t pin=A0)
        : Sensor("VoltageSensor", "V", 30, -0.0f, 3.3f, 10), pin(pin)
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
        return std::map<const char*, double> {{"Voltage", getVoltage()}};
    }

private:
    double getVoltage(){
      int adc_val = analogRead(pin);
      double voltage = adc_val * ( 3.3 / 1024.0);
      return voltage;
    }
    uint8_t pin;
};
