#ifndef RUNWAY_H
#define RUNWAY_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "AirportObject.hpp"

class Airplane;

class WaitingAirplanes
{
    public:

        int getSize();

        void pushBack(std::shared_ptr<Airplane> airplane, std::promise<void> &&promise);
        void permitEntry();

    private:

        std::vector<std::shared_ptr<Airplane>> _airplanes;
        std::vector<std::promise<void>> _promises;     
        std::mutex _mtx;
};

class Runway : public AirportObject, public std::enable_shared_from_this<Runway>
{
    public:
        
        Runway();

        // Behavior functions
        void simulate();
        void addAirplaneToQueue(std::shared_ptr<Airplane> airplane);

        std::shared_ptr<Runway> getExitRunway() { return _exitRunway; }
        void setExitRunway(std::shared_ptr<Runway> runway) { _exitRunway = runway; }
        double getLength() { return _length; }
        void setLength(double length) { _length = length; }

        std::shared_ptr<Runway> get_shared_this() { return shared_from_this(); }

    private:

        double _length;                        // Length of the Runway
        std::shared_ptr<Runway> _exitRunway;    // The runway the airplane exits into after entering this one
        WaitingAirplanes _waitingQueue;

};

#endif