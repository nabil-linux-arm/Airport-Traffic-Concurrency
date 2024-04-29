#ifndef PORT_H 
#define PORT_H 

#include "AirportObject.hpp"
#include "Runway.hpp"

class Airplane;
class Runway;

class PortQueue
{
    public:
    private:
};

class Port : public Runway
{
    public:
        Port();

        void addAirplaneToQueue(std::shared_ptr<Airplane> airplane);

    private:
        // TODO: Need to think about waiting functionality
        void startRandomWait(double min_ms, double max_ms); // Generates random delay and starts a timer in ms for the airplane to wait in port.

        bool _portOccupied;  // Signal whether the port occupied or not (1 port to 1 airplane)
        std::shared_ptr<Airplane> _dockedAirplane;   // Airplane currently occupying the port
        std::shared_ptr<Runway> _runway; // Runway the port is attached to

};

#endif