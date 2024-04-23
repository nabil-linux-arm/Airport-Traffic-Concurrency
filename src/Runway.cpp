#include "Runway.hpp"
#include "Airplane.hpp"

int WaitingAirplanes::getSize()
{
    return _airplanes.size();
}

void WaitingAirplanes::pushBack(std::shared_ptr<Airplane> airplane)
{
     _airplanes.push_back(airplane);
}

void WaitingAirplanes::permitEntry()
{
    printf("[Runway] - REMOVING AIRPLANE, Current size of waiting Queue: %d\n", this->getSize());
    printf("[Runway] - REMOVING AIRPLANE, id: %d\n", _airplanes.back()->getID());
    _airplanes.pop_back();
}

// Runway Class

Runway::Runway()
{
    _length = 40.0;
}

Runway::~Runway()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Runway] - CALLING DECONSTRUCTOR\n");
    lck.unlock();
}

void Runway::simulate()
{
    _threads.emplace_back(std::thread(&Runway::processAirplaneQueue, this));
}

void Runway::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    // std::unique_lock<std::mutex> lck(_cout_mtx);
    // printf("[Runway] - ADDING AIRPLANE, id: %d\n", airplane->getID());
    // lck.unlock();

    // _waitingQueue.pushBack(airplane);
}

void Runway::permitAirplaneIn()
{
    _waitingQueue.permitEntry();
}
void Runway::processAirplaneQueue()
{
}
