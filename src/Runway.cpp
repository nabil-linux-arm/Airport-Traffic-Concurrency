#include "Runway.hpp"
#include "Airplane.hpp"

Runway::Runway(){}

void Runway::simulate(){}
void Runway::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    printf("[Runway] - ADDING AIRPLANE %d", airplane->getID());
}

int WaitingAirplanes::getSize(){}

void WaitingAirplanes::pushBack(std::shared_ptr<Airplane> airplane, std::promise<void> &&promise){}
void WaitingAirplanes::permitEntry(){}