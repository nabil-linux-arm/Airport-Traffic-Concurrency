#include "Airplane.hpp"

// TODO: 
// Define what the airplane state is intitially 
Airplane::Airplane()
{
    _port = nullptr;
    _nextRunway = nullptr;
    _type = airplane;
    _speed = 500; // m/s
}

void Airplane::simulate() {}
void move() {}                   
void moveToPort(int port_id) {}   
bool isDestinationReached() {}    