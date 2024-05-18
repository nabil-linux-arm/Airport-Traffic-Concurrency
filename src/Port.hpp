#ifndef PORT_H 
#define PORT_H 

#include "AirportObject.hpp"
#include "Runway.hpp"

#include <vector>

class Airplane;
class Runway;

class Terminal : AirportObject
{
    public:
        void startRandomWait(double min_ms, double max_ms); // Generates random delay and starts a timer in ms for the airplane to wait in port.
    
};

class Port : public Runway 
{
    public:
        Port();
        Port(int n_ports);

        void simulate();
        void addAirplaneToPortQueue(std::shared_ptr<Airplane> airplane);

        int getPortCount();
        void freePort(int port_id);

        void incPortCount();
        void decPortCount();

        int getPort();        // Return the port id
        void getPortPosition(double &x, double &y, int port_id);   // Return position of port in runway

    private:
        void processPortQueue();

        WaitingAirplanes _waitingPortQueue;
        bool _portOccupied;  // Signal whether the port occupied or not (1 port to 1 airplane)
        std::shared_ptr<Airplane> _dockedAirplane;   // Airplane currently occupying the port
        std::shared_ptr<Runway> _runway; // Runway the port is attached to

        int _port_count;             // Number of ports available
        int _const_port_count;       // Number of ports assigned
        std::mutex _count_mtx;
        
        std::vector<int> _port_pool;  // List of ports available for assignment

};

#endif