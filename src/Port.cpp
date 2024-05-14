#include "Port.hpp"
#include "Airplane.hpp"

Port::Port()
{
    _runway_type = port;
    _port_count = 0;
    _type = runway;
    _length = 40.0;
}

Port::Port(int n_ports) : _port_count(n_ports)
{
    _runway_type = port;
    _type = runway;
    _length = 40.0;
}

void Port::simulate()
{
    // To manage airplanes waiting for unblocked runway
    _threads.emplace_back(std::thread(&Port::processAirplaneQueue, this));
    // To manage airplanes waiting for a port to be assigned to them
    _threads.emplace_back(std::thread(&Port::processPortQueue, this));
}

void Port::addAirplaneToQueue(std::shared_ptr<Airplane> airplane)
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT %d] - ENTER QUEUE: AIRPLANE #%d\n", this->getID(), airplane->getID());
    lck.unlock();

    // (1) First the airplane must have a port assigned to him before moving on
    std::promise<void> promiseAssignPort;
    std::future<void> futureAirplaneAssignedPort = promiseAssignPort.get_future();

    _waitingPortQueue.pushBack(airplane, std::move(promiseAssignPort));
    
    futureAirplaneAssignedPort.wait();
    airplane->setPortAssigned(true);

    // (2) Join the Runway queue
    std::promise<void> promiseAllowAirplaneToEnter;
    std::future<void> futureAllowAirplaneToEnter = promiseAllowAirplaneToEnter.get_future();

    _waitingQueue.pushBack(airplane, std::move(promiseAllowAirplaneToEnter));
    
    futureAllowAirplaneToEnter.wait();

    lck.lock();
    printf("[PORT %d] - EXIT QUEUE: AIRPLANE #%d\n", this->getID(), airplane->getID());
}

// Generates random delay and starts a timer in ms for the airplane to wait in port.
void Terminal::startRandomWait(double min_ms, double max_ms)
{
    int range = max_ms - min_ms;
    int timeDelay = (rand() % abs(range)) + min_ms;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT %d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}
void Port::processPortQueue()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT %d] - START PROCESS PORT QUEUE\n",getID());
    lck.unlock();

    // Continually process the queue
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (_waitingPortQueue.getSize() > 0 && getPortCount() > 0) 
        {
            lck.lock();
            printf("[PORT %d] - PORT COUNT: %d\n", getID(),this->getPortCount());
            lck.unlock();

            this->decPortCount();
            // Let the airplane proceed into the runway
            _waitingPortQueue.permitEntry();
        }
    }

}

int Port::getPortCount()
{ 
    std::lock_guard<std::mutex> lock(_count_mtx);
    return _port_count; 
}
void Port::incPortCount() 
{ 
    std::lock_guard<std::mutex> lock(_count_mtx);
    _port_count++; 
}
void Port::decPortCount() 
{ 
    std::lock_guard<std::mutex> lock(_count_mtx);
    _port_count--; 
}