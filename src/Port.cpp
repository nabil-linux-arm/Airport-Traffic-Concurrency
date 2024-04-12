#include "Port.hpp"

Port::Port(){}

void Port::dockAirplane(std::shared_ptr<Airplane> airplane){}
void Port::startRandomWait(double min_ms, double max_ms){} // Generates random delay and starts a timer in ms for the airplane to wait in port.
void Port::timerInterrupt(){}