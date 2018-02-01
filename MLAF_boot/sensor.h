#pragma once

#include <map>
#include <list>

#include "measurement.h"

/**
 * Base class for sensors
*/
class Sensor{
public:
    /**
     * Activates the sensor
     * 
     * <b>NOTE: Must be implemented in a subclass</b>
     * 
     * \return <b>true</b> if succesful, <b>false</b> otherwise
    */
    virtual bool activate() = 0;
    
    /**
     * Activates the sensor
     * 
     * <b>NOTE: Must be implemented in a subclass</b>
     * 
     * \return <b>true</b> if succesful, <b>false</b> otherwise
    */
    virtual bool deactivate() = 0;

    /**
     * Gets data from the sensor
     * 
     * <b>NOTE: Must be implemented in a subclass</b>
     * 
     * \return a map (key value pair/dictionary) of sensor data
    */
    //template<typename T>
    virtual const std::map<const char*, double> getData() = 0;


    /**
     * Gets the unit associated with the sensor data
     *
     * \return the name of the used unit
     */
    const char* getUnit()
    {
        return _unit;
    }

    /**
     * Gets the id associated with the sensor
     *
     * \return the id of the sensor
     */
    const char* getId()
    {
        return _id;
    }

    /**
     * Gets the name associated with the sensor
     *
     * \return the name of the sensor
     */
    const char* getName()
    {
        return _name;
    }

    /**
     * Gets the interval of the sensor
     *
     * \return the interval of the sensor
     */
    size_t getInterval()
    {
        return _interval;
    }

    std::list<MeasurementMetadata> getMeasurementsMetadata()
    {
        return _measurements;
    }

    /**
     * Gets the amount of backup measurements to keep
     *
     * \return the amount of backup measurements
     */
    size_t getNrBackupMeasurements()
    {
        return _nrBackupMeasurements;
    }

    bool needsUpdate(unsigned long t)
    {
        if (t > _lastTimeUpdated + _interval)
        {
            _lastTimeUpdated = t;
            return true;
        }
        return false;
    }

protected:
    /**
     * 
     * \param name the name of the sensor
     * \param unit the unit used by the sensor
     * \param interval the interval of the sensor
     * \param minval the minimum value expected from the sensor
     * \param maxval the maximum value expected from the sensor
    */
    Sensor(const char* id,
        const char* name,
        const char* unit,
        size_t interval,
        size_t nrBackupMeasurements)
        : _id{id},
        _name{name},
        _unit{unit},
        _interval{interval},
        _nrBackupMeasurements{nrBackupMeasurements}
    {}
    
    bool active = false;

    void addMeasurementMetadata(MeasurementMetadata measurementMetadata)
    {
        _measurements.push_back(measurementMetadata);
    }

private:
    const char* _id;
    const char* _name;
    const char* _unit;
    const size_t _interval;
    const size_t _nrBackupMeasurements;
    std::list<MeasurementMetadata> _measurements;

    unsigned long _lastTimeUpdated = 0;
};