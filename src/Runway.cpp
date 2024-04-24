#include "Runway.hpp"
#include "Airplane.hpp"
#include <chrono>

int WaitingAirplanes::getSize()
{
    std::lock_guard<std::mutex> lck(_mtx);
    return _airplanes.size();
}

void WaitingAirplanes::pushBack(std::shared_ptr<Airplane> airplane, std::promise<void> &&prms)
{
    std::lock_guard<std::mutex> lck(_mtx);
    _airplanes.push_back(airplane);
    _promises.push_back(std::move(prms));
}

void WaitingAirplanes::permitEntry()
{
    std::lock_guard<std::mutex> lck(_mtx);

    auto firstAirplane = _airplanes.begin();
    auto firstPromise = _promises.begin();

    firstPromise->set_value();

    _airplanes.erase(firstAirplane);
    _promises.erase(firstPromise);
}


// Runway Class

Runway::Runway()
{
    _length = 40.0;
    _isBlocked = false;
}

Runway::~Runway()
{
    // std::unique_lock<std::mutex> lck(_cout_mtx);
    // printf("[Runway] - CALLING DECONSTRUCTOR\n");
    // lck.unlock();
}

void Runway::simulate()
{
    // TODO:
    // - [] Improve the fact that this does not create a thread that shares ownership with
    //      the main thread object.
    _threads.emplace_back(std::thread(&Runway::processAirplaneQueue, this));
}

void Runway::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Runway] - ADDING AIRPLANE #%d\n", airplane->getID());
    lck.unlock();

    std::promise<void> promiseAllowAirplaneToEnter;
    std::future<void> futureAllowAirplaneToEnter = promiseAllowAirplaneToEnter.get_future();

    _waitingQueue.pushBack(airplane, std::move(promiseAllowAirplaneToEnter));

    futureAllowAirplaneToEnter.wait();
    lck.lock();
    printf("[Runway] - Allow airplane to enter runway, #%d\n", airplane->getID());
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
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Runway] - START RUNWAY PROCESS: %d\n", getID());
    lck.unlock();

    // Continually process the queue
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (_waitingQueue.getSize() > 0 && _isBlocked == false) 
        {
            _isBlocked = true;
            // Let the airplane proceed into the runway
            lck.lock();
            printf("[Runway] - REMOVING AIRPLANE\n");
            lck.unlock();

            _waitingQueue.permitEntry();
        }
    }
}
void Runway::runwayClear()    
{
    // Unblock the runway queue
    this->setIsBlocked(false);
}