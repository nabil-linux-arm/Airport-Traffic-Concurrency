#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "AirportObject.hpp"
#include <memory>

class Port;
class Runway;

struct position
{
    double x;
    double y;
};

class Airplane : public AirportObject, public std::enable_shared_from_this<Airplane>
{
    public:
        // Constructors/Destructors
        Airplane();
        ~Airplane();

        // Behaviour functions
        void simulate();
        void setCurrentRunway(std::shared_ptr<Runway> runway);
        void setNextRunway(std::shared_ptr<Runway> runway);
        std::shared_ptr<Runway> getNextRunway() { return _nextRunway; }

        void setPortID(int port_id) { _portID = port_id; }
        int getPortID() { return _portID; }

        void setDestinationPosition(position destination)
        {
            _destinationPos = destination;
            _posRunway = 0.0;
        }

        void setStartPosition(position start) { _startPos = start; }

        double calculateDistance();

        std::shared_ptr<Airplane> get_shared_this() { return shared_from_this(); }

    private:
        void move(); // main logical function for the airplane
        void moveToPort(int port_id); // creates the animation for moving to a specified port

        // Support functions
        void startTimer(int min, int max);  // Starts the delay for planes to be flying
        
        int _portID;
        // std::shared_ptr<Port> _port;           // Port the Airplane is currently docked at (Undefined when not)
        std::shared_ptr<Runway> _currentRunway;   // The runway that the Airplane is currently waiting to enter
        std::shared_ptr<Runway> _nextRunway;   // The runway that the Airplane is currently heading towards
        double _posRunway;                     // Position of Airplane on runway
        double _speed;                         // Speed of Airplane during simulation
        double _default_speed;                 // Speed of Airplane on the runway

        position _startPos;
        position _destinationPos;

};

#endif