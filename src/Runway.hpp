#ifndef RUNWAY_H
#define RUNWAY_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "AirportObject.hpp"

enum RunwayType
{
    normal,
    landing,
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
        ~Runway();

        // Behavior functions
        void simulate();
        virtual void addAirplaneToQueue(std::shared_ptr<Airplane> airplane);

        void runwayClear();         // Signal to runway that the plane has left the runway

        std::shared_ptr<Runway> getExitRunway() { return _exitRunway; }
        void setExitRunway(std::shared_ptr<Runway> runway) { _exitRunway = runway; }

        double getLength() { return _length; }
        void setLength(double length) { _length = length; }

        RunwayType getRunwayType() { return _runway_type; }
        void setRunwayType(RunwayType type) { _runway_type = type; }

        std::shared_ptr<Runway> get_shared_this() { return shared_from_this(); }
    
    protected:
        WaitingAirplanes _waitingQueue;
        void processAirplaneQueue();
        double _length;                        // Length of the Runway

    private:

        void setIsBlocked(bool blocked) { _isBlocked = blocked; }

        std::shared_ptr<Runway> _exitRunway;    // The runway the airplane exits into after entering this one
        bool _isBlocked;
                                              // This variable is shared with multiple threads without mutex (unsafe),
                                              // However, the variable does not change and is constant throughout the
                                              // simulation.
        RunwayType _runway_type;
        
};

#endif