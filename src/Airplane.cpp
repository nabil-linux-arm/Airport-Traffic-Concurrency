#include "Airplane.hpp"
#include "Runway.hpp"
#include <chrono>
#include <thread>
#include <iostream>

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

Airplane::~Airplane()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Airplane] - DESTRUCTOR CALLED\n");
}

void Airplane::simulate() 
{
    // Provide some error checking
    if (_currentRunway == nullptr) 
    {
        std::unique_lock<std::mutex> lck(_cout_mtx);
        printf("[Airplane %d] - ERROR: runway not set\n", this->getID());
        lck.unlock();
        return;
    }
    // Creates a threasd for the move function of this airplane
    // emplace_back is used instead of push because thread objects cannot be copied (done when pushing objects)
    // Sufficient amount of time must pass in order for thread object to increment the shared_ptr
    // count for the object. If not the Main thread will finish before this thread and deallocate it.
    _threads.emplace_back(std::thread(&Airplane::move, this));
}

void Airplane::setCurrentRunway(std::shared_ptr<Runway> runway)
{
    _currentRunway = runway;
}

void Airplane::move() 
{
    while (true)
    {
        if (_currentRunway->getIsLandingRunway() == true)
        {
            // Timer for when the airplane is in the air
            startTimer(MIN_DELAY, MAX_DELAY);
        }
        _currentRunway->addAirplaneToQueue(get_shared_this());

        // Simulates time it takes for a plane to fully travel across the runway or other stuff
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        // Signal that the runway is clear
        _currentRunway->runwayClear();

        // Set new runway as next runway
        _currentRunway = _currentRunway->getExitRunway();
    }
}                   

void Airplane::moveToPort(int port_id) {}   
bool Airplane::isDestinationReached() {}    

void Airplane::startTimer(int min, int max)
{
    // TODO: Create a limits for min and max
    int range = max - min;
    int timeDelay = (rand() % abs(range)) + min;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Airplane %d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}