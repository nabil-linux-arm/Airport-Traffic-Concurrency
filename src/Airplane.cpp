#include "Airplane.hpp"
#include "Runway.hpp"
#include <chrono>
#include <thread>

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
    // Creates a threasd for the move function of this airplane
    // emplace_back is used instead of push because thread objects cannot be copied (done when pushing objects)
    _threads.emplace_back(std::thread(&Airplane::move, this));
    // move();
}

void Airplane::setCurrentRunway(std::shared_ptr<Runway> runway)
{
    _currentRunway = runway;
}

void Airplane::move() 
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Airplane %d] - Simulation Started\n", this->getID());
    lck.unlock();

    startTimer(MIN_DELAY, MAX_DELAY);

    // _currentRunway->addAirplaneToQueue(get_shared_this());
    auto my_address = get_shared_this();

    startTimer(MIN_DELAY, MAX_DELAY);

    lck.lock();
    printf("[Airplane %d] - Simulation Finished\n", this->getID());
    lck.unlock();
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