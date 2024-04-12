#ifndef RUNWAY_H
#define RUNWAY_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "AirportObject.hpp"

class Airplane;

class Runway : public AirportObject
{
    public:
        // Behavior functions
        void addAirplaneToQueue(std::shared_ptr<Airplane> airplane);
                
    private:

};

#endif