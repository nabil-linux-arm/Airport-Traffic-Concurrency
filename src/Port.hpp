#ifndef PORT_H
#define PORT_H

#include <utility>
#include <vector>
#include <map>

#include "AirportObject.hpp"
#include "Runway.hpp"

// TODO:
// - [] Dedicated move functionality for port.
// - [] Port positions automatically adjust to always be parallel to
//      the direction of the runway.

class Airplane;
class Runway;

class Port : public Runway
{
    public:
        Port();
        Port(int n_ports);

        void simulate();
        void addAirplaneToPortQueue(std::shared_ptr<Airplane> airplane);

        int getPortCount();
        void freePort(int port_id);
        int getPort();
        void getPortPosition(double &x, double &y, int port_id);
        void setPortPosition(double x, double y, int port_id);

    private:
        void processPortQueue();

        WaitingAirplanes _waitingPortQueue; // Queue of airplanes waiting to for a port.
        std::shared_ptr<Runway> _runway; // Runway the port is attached to

        int _port_count;             // Number of ports available
        std::mutex _count_mtx;       // Mutex to protect the port pool

        std::vector<int> _port_pool;  // List of ports available for assignment
        std::map<int , std::pair<double, double> > _port_pos; // List of coordinates for corresponding ports

};

#endif
