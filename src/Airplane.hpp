#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "AirportObject.hpp"
#include <memory>

class Port;
class Runway;

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

        bool isPortAssigned() { return _portAssigned; }
        void setPortAssigned(bool have_port) { _portAssigned = have_port; }

        std::shared_ptr<Airplane> get_shared_this() { return shared_from_this(); }

    private:
        void move(); // main logical function for the airplane
        void moveToPort(int port_id); // creates the animation for moving to a specified port
        bool isDestinationReached();  // Abstracted function to decide when it reaches destination

        // Support functions
        void startTimer(int min, int max);  // Starts the delay for planes to be flying
        
        bool _portAssigned;
        // std::shared_ptr<Port> _port;           // Port the Airplane is currently docked at (Undefined when not)
        std::shared_ptr<Runway> _currentRunway;   // The runway that the Airplane is currently waiting to enter
        std::shared_ptr<Runway> _nextRunway;   // The runway that the Airplane is currently heading towards
        double _posRunway;                     // Position of Airplane on runway
        double _speed;                         // Speed of Airplane on runway

};

#endif