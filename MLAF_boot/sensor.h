#pragma once

#include <map>
#include <list>

/**
 * \brief Base class for sensors
*/
class Sensor{
public:
    /**
     * \brief Activates the sensor
     * 
     * <b>NOTE: Must be implemented in a subclass</b>
     * 
     * \return <b>true</b> if succesful, <b>false</b> otherwise
    */
    virtual bool activate() = 0;
    
    /**
     * \brief Deactivates the sensor
     * 
     * <b>NOTE: Must be implemented in a subclass</b>
     * 
     * \return <b>true</b> if succesful, <b>false</b> otherwise
    */
    virtual bool deactivate() = 0;

    /**
     * \brief Gets data from the sensor
     * 
     * <b>NOTE: Must be implemented in a subclass</b>
     * 
     * \return a map (key value pair/dictionary) of sensor data
    */
    //template<typename T>
    virtual const std::map<const char*, double> getData() = 0;


    /**
     * \brief Gets the unit associated with the sensor data
     *
     * \return the name of the used unit
     */
    const char* getUnit()
    {
        return _unit;
    }

    /**
     * \brief Gets the name associated with the sensor
     *
     * \return the name of the sensor
     */
    const char* getName()
    {
        return _name;
    }

    /**
     * \brief Gets the interval of the sensor
     *
     * \return the interval of the sensor
     */
    size_t getInterval()
    {
        return _interval;
    }

    /**
     * \brief Gets the minimum value supported by the sensor
     *
     * \return the minimum value supported by the sensor
     */
    double getMinVal()
    {
        return _minVal;
    }

    /**
     * \brief Gets the maximum value supported by the sensor
     *
     * \return the maximum value supported by the sensor
     */
    double getMaxVal()
    {
        return _maxVal;
    }

    /**
     * \brief Gets the amount of backup measurements to keep
     *
     * \return the amount of backup measurements
     */
    size_t getNrBackupMeasurements()
    {
        return _nrBackupMeasurements;
    }

    /**
     * \brief Determines if a sensor is ready to update its values
     * 
     * \param t the time to compare against the sensor interval
     */
    bool needsUpdate(unsigned long t)
    {
        if (t > lastTimeUpdated + _interval)
        {
            lastTimeUpdated = t;
            return true;
        }
        return false;
    }

protected:
    /**
     * \brief Creates a sensor
     * 
     * \param name the name of the sensor
     * \param unit the unit used by the sensor
     * \param interval the interval of the sensor
     * \param minval the minimum value expected from the sensor
     * \param maxval the maximum value expected from the sensor
     * \param nrBackupMeasurements the amount of backup measurements to keep
    */
    Sensor(const char* name, const char* unit, size_t interval, double minval, double maxval, size_t nrBackupMeasurements)
        : _name{name}, _unit{unit}, _interval{interval}, _minVal{minval}, _maxVal{maxval}, _nrBackupMeasurements{nrBackupMeasurements}
    {}
    
    /**
     * \brief Signifies if the sensor is active
     */
    // TODO: implement
    bool active = false;

private:
    const char* _unit;
    const char* _name;
    const size_t _interval;
    const double _minVal;
    const double _maxVal;
    const size_t _nrBackupMeasurements;

    unsigned long lastTimeUpdated = 0;
};