#ifndef RUNWAY_H
#define RUNWAY_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "AirportObject.hpp"

// TODO:
// - [] Currently the length variable is not used. Maybe it will help in the future?

/**
 * An identifier for type of runway.
 * 
 * Depending on the runway type the airplane will act differently like move
 * move to a port, set a delay or speed up.
*/
enum RunwayType
{
    normal,
    sky,
    port
};

class Airplane;

class WaitingAirplanes
{
    public:

        int getSize();

        void pushBack(std::shared_ptr<Airplane> airplane, std::promise<void> &&prms);
        void permitEntry();

    private:

        std::vector<std::shared_ptr<Airplane> > _airplanes;
        std::vector<std::promise<void> > _promises;     
        std::mutex _mtx;
};

class Runway : public AirportObject, public std::enable_shared_from_this<Runway>
{
    public:
        
        Runway();
        Runway(RunwayType type);

        // Behavior functions
        void simulate();
        virtual void addAirplaneToQueue(std::shared_ptr<Airplane> airplane);
        void processAirplaneQueue();

        void runwayClear();         

        // Setters and Getters
        std::shared_ptr<Runway> getExitRunway() { return _exitRunway; }
        void setExitRunway(std::shared_ptr<Runway> runway) { _exitRunway = runway; }

        double getLength() { return _length; }
        void setLength(double length) { _length = length; }

        RunwayType getRunwayType() { return _runway_type; }
        void setRunwayType(RunwayType type) { _runway_type = type; }

        // Miscellanious
        std::shared_ptr<Runway> get_shared_this() { return shared_from_this(); }

    private:

        void setIsBlocked(bool blocked) { _isBlocked = blocked; }

        WaitingAirplanes _waitingQueue;
        std::shared_ptr<Runway> _exitRunway;    // The runway the airplane exits into after entering this one
        bool _isBlocked;
        double _length;                        // Length of the Runway
                                              
        RunwayType _runway_type;              // This variable is shared with multiple threads without mutex (unsafe),
                                              // However, the variable does not change and is constant throughout the
                                              // simulation.
        
};

#endif