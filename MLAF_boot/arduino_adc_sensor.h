#pragma once

#include "sensor.h"
#include <Arduino.h>

class ADCSensor : public Sensor
{
public:
    ADCSensor(uint8_t pin=A0)
        : Sensor("VoltageSensor", "V", 30, -0.0f, 3.3f, 10), pin(pin)
    {}

    virtual bool activate() override
    {
        //not implemented
        return true;
    }

    virtual bool deactivate() override
    {
        //not implemented
        return true;
    }
    
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
