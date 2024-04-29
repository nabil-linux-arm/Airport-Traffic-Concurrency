#include "Port.hpp"
#include "Airplane.hpp"

Port::Port(){}

void Port::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT %d] - ENTER QUEUE: AIRPLANE #%d\n", this->getID(), airplane->getID());
    lck.unlock();

    std::promise<void> promiseAllowAirplaneToEnter;
    std::future<void> futureAllowAirplaneToEnter = promiseAllowAirplaneToEnter.get_future();

    _waitingQueue.pushBack(airplane, std::move(promiseAllowAirplaneToEnter));
    
    // Wait until permit entry is called
    futureAllowAirplaneToEnter.wait();

    lck.lock();
    printf("[PORT %d] - EXIT QUEUE: AIRPLANE #%d\n", this->getID(), airplane->getID());
}

// Generates random delay and starts a timer in ms for the airplane to wait in port.
void Port::startRandomWait(double min_ms, double max_ms)
{
    int range = max_ms - min_ms;
    int timeDelay = (rand() % abs(range)) + min_ms;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT %d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}
