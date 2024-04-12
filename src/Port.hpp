#ifndef PORT_H 
#define PORT_H 

#include "AirportObject.hpp"

class Airplane;
class Runway;

class Port : public AirportObject
{
    public:
        Port();

        bool isPortOccupied() { return _portOccupied; }
        void dockAirplane(std::shared_ptr<Airplane> airplane); // Add airplane to the dock
        // TODO: Think about the behavoiours of the port

    private:
        // TODO: Need to think about waiting functionality
        void startRandomWait(double min_ms, double max_ms); // Generates random delay and starts a timer in ms for the airplane to wait in port.
        void timerInterrupt();

        bool _portOccupied;  // Signal whether the port occupied or not (1 port to 1 airplane)
        std::shared_ptr<Airplane> _dockedAirplane;   // Airplane currently occupying the port
        std::shared_ptr<Runway> _runway; // Runway the port is attached to

};

#endif