#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "AirportObject.hpp"
#include <memory>

class Port;
class Runway;

// TODO:
// - [] Getter and setters for airplane speed

// Struct for 2D pixel position with x and y
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

        // Behaviour functions
        void simulate();
        void setCurrentRunway(std::shared_ptr<Runway> runway);

        // Setter and Getters
        void setPortID(int port_id);
        int getPortID();

        void setDestinationPosition(position destination);
        void setStartPosition(position start);

        // Miscellaneous
        double calculateDistance();
        std::shared_ptr<Airplane> get_shared_this() { return shared_from_this(); }

    private:
        void move(); // state machine logic for the airplane

        void startTimer(int min, int max);     // Generates random delay between min and max
        
        std::shared_ptr<Runway> _currentRunway;   // The runway that the Airplane is currently in
        int _portID;                           // The port id of the port assigned to the airplane
        double _posRunway;                     // Position of Airplane on runway
        double _speed;                         // Current speed of Airplane during simulation
        double _default_speed;                 // Speed of Airplane on the runway

        position _startPos;                    // The starting the position for the airplane on the runway
        position _destinationPos;              // The destination position for the airplane on the runway

};

#endif