#include "Runway.hpp"
#include "Airplane.hpp"
#include <chrono>

// ---- WaitingAirplanes Class ----- //

/**
 * Returns the size of the airplanes waiting in the queue.
 * 
 * @return Number of airplanes in the queue.
*/
int WaitingAirplanes::getSize()
{
    std::lock_guard<std::mutex> lck(_mtx);
    return _airplanes.size();
}

/**
 * Appends an airplane object into the back of the waiting queue.
 * 
 * @param airplane Airplane object that is waiting for the runway to be clear.
 * @param prms A promise used to signal that the airplane has been permitted to enter the runway.
*/
void WaitingAirplanes::pushBack(std::shared_ptr<Airplane> airplane, std::promise<void> &&prms)
{
    std::lock_guard<std::mutex> lck(_mtx);
    _airplanes.push_back(airplane);
    _promises.push_back(std::move(prms));
}

/**
 * Removes airplane from queue and signal that the airplane is out of queue.
*/
void WaitingAirplanes::permitEntry()
{
    std::lock_guard<std::mutex> lck(_mtx);

    auto firstAirplane = _airplanes.begin();
    auto firstPromise = _promises.begin();

    firstPromise->set_value();

    _airplanes.erase(firstAirplane);
    _promises.erase(firstPromise);
}


// ---- Runway Class ------ //

Runway::Runway()
{
    _length = 100.0;
    _isBlocked = false;
    _exitRunway = nullptr;
    _runway_type = normal;

    _type = ObjectType::runway;
}

/**
 * Secondary Runway constructor.
 * 
 * The purpose is to set the runway to a specific type to help the airplane state machine.
 * 
 * @param type The type of runway this needs to be classified as.
*/
Runway::Runway(RunwayType type) : _runway_type(type)
{
    _length = 100.0;
    _isBlocked = false;
    _exitRunway = nullptr;

    _type = ObjectType::runway;
}

/**
 * Starts thread for runway.
 * 
 * Executes the runway::processAirplaneQueue() function in a seperate thread besides the main.
*/
void Runway::simulate()
{
    // TODO:
    // - [] Improve the fact that this does not create a thread that shares ownership with
    //      the main thread object.
    _threads.emplace_back(std::thread(&Runway::processAirplaneQueue, this));
}

/**
 * Adds airplane object into the waiting queue.
 * 
 * When an airplane is in the waiting queue it's thread stops and waits.
 * 
 * @param airplane Airplane just entering the runway and is going to wait for 
 * runway to be clear.
*/
void Runway::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[RUNWAY #%d] - Queue in Runway: AIRPLANE #%d\n", this->getID(), airplane->getID());
    lck.unlock();

    std::promise<void> promiseAllowAirplaneToEnter;
    std::future<void> futureAllowAirplaneToEnter = promiseAllowAirplaneToEnter.get_future();

    _waitingQueue.pushBack(airplane, std::move(promiseAllowAirplaneToEnter));

    // Wait until permit entry is called
    futureAllowAirplaneToEnter.wait();

    lck.lock();
    printf("[RUNWAY #%d] - Enter Runway: AIRPLANE #%d\n", this->getID(), airplane->getID());
}

/**
 * Main processing function of the Runway object.
 * 
 * During the runtime of this function, it continually checks whether the runway is clear
 * and if so proceeds to permit entry to the next airplane in the waiting queue. Has an
 * implicit connection with the runway::addAirplaneToQueue function.
*/
void Runway::processAirplaneQueue()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[RUNWAY #%d] - Starting airplane queue processing\n", getID());
    lck.unlock();

    // Continually process the queue
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (_waitingQueue.getSize() > 0 && _isBlocked == false) 
        {
            this->setIsBlocked(true);
            // Let the airplane proceed into the runway
            _waitingQueue.permitEntry();
        }
    }
}

/**
 * Signals that the runway is clear.
 * 
 * Mainly used by the airplane object to communicate that it has left the runway.
*/
void Runway::runwayClear()    
{
    // Unblock the runway queue
    this->setIsBlocked(false);
}