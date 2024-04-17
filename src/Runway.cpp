#include "Runway.hpp"
#include "Airplane.hpp"

Runway::Runway(){}

void Runway::simulate(){}
void Runway::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    printf("[Runway] - ADDING AIRPLANE, id: %d\n", airplane->getID());
    _waitingQueue.pushBack(airplane);
}

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
void Runway::permitAirplaneIn()
{
    _waitingQueue.permitEntry();
}