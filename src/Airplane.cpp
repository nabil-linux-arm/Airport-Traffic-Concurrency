#include "Airplane.hpp"
#include <chrono>

const int MIN_DELAY = 100;  // milliseconds
const int MAX_DELAY = 5000; // milliseconds
// TODO: 
// Define what the airplane state is intitially 
Airplane::Airplane()
{
    _port = nullptr;
    _nextRunway = nullptr;
    _type = airplane;
    _speed = 500; // m/s
}

void Airplane::simulate() 
{
    startTimer(MIN_DELAY,MAX_DELAY);
    printf("[Airplane] - Stop Timer\n");
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
}