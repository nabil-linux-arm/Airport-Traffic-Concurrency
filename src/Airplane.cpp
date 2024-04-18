#include "Airplane.hpp"
#include "Runway.hpp"
#include <chrono>

// TODO: 
// - [] Define what the airplane state is intitially 
// - [] Define some initial checks to make sure the simulation is safe
// - [] Remove all debugging logic and refactor the code

const int MIN_DELAY = 100;  // milliseconds
const int MAX_DELAY = 5000; // milliseconds

Airplane::Airplane()
{
    _port = nullptr;
    _currentRunway = nullptr;
    _type = airplane;
    _speed = 500; // m/s
}

void Airplane::simulate() 
{
    startTimer(MIN_DELAY,MAX_DELAY);
    // Add Airplane to landing strip queue
    if (_currentRunway != nullptr)
    {
        _currentRunway->addAirplaneToQueue(get_shared_this());
        startTimer(MIN_DELAY,MAX_DELAY);    
        _currentRunway->permitAirplaneIn();
    }
    else
    {
        printf("[Airplane] ERROR - Runway not set\n");
    }
}

void Airplane::setCurrentRunway(std::shared_ptr<Runway> runway)
{
    _currentRunway = runway;
}

void move() {}                   
void moveToPort(int port_id) {}   
bool isDestinationReached() {}    

void Airplane::startTimer(int min, int max)
{
    // TODO: Create a limits for min and max
    int range = max - min;
    int timeDelay = (rand() % abs(range)) + min;
    printf("[Airplane] - Start Timer: %d ms\n", timeDelay);
    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
    printf("[Airplane] - Stop Timer\n");
}