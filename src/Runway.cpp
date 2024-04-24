#include "Runway.hpp"
#include "Airplane.hpp"

int WaitingAirplanes::getSize()
{
    std::lock_guard<std::mutex> lck(_mtx);
    return _airplanes.size();
}

void WaitingAirplanes::pushBack(std::shared_ptr<Airplane> airplane)
{
    std::lock_guard<std::mutex> lck(_mtx);
    _airplanes.push_back(airplane);
}

void WaitingAirplanes::permitEntry()
{
    std::lock_guard<std::mutex> lck(_mtx);
    _airplanes.pop_back();
}

// Runway Class

Runway::Runway()
{
    _length = 40.0;
}

Runway::~Runway()
{
    // std::unique_lock<std::mutex> lck(_cout_mtx);
    // printf("[Runway] - CALLING DECONSTRUCTOR\n");
    // lck.unlock();
}

void Runway::simulate()
{
    _threads.emplace_back(std::thread(&Runway::processAirplaneQueue, this));
}

void Runway::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Runway] - ADDING AIRPLANE, id: %d\n", airplane->getID());
    lck.unlock();

    _waitingQueue.pushBack(airplane);
}

void Runway::permitAirplaneIn()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Runway] - Queue size: %d\n", _waitingQueue.getSize());
    lck.unlock();
    if (_waitingQueue.getSize() > 0) 
    {
        lck.lock();
        printf("[Runway] - REMOVING AIRPLANE\n");
        lck.unlock();
        _waitingQueue.permitEntry();

    }
}
void Runway::processAirplaneQueue()
{
}
