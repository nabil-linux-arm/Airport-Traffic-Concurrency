#include "Port.hpp"
#include "Airplane.hpp"

Port::Port()
{
    setRunwayType(port);
    _port_count = 1;
    _type = runway;
    _length = 40.0;
    _const_port_count = 1;
}

Port::Port(int n_ports) : _port_count(n_ports)
{
    setRunwayType(port);
    _type = runway;
    _length = 40.0;
    _const_port_count = n_ports;
}

void Port::simulate()
{
    // To manage airplanes waiting for unblocked runway
    _threads.emplace_back(std::thread(&Port::processAirplaneQueue, this));
    // To manage airplanes waiting for a port to be assigned to them
    _threads.emplace_back(std::thread(&Port::processPortQueue, this));
}

// Adds airplane to Queue to wait for Port assignment
void Port::addAirplaneToPortQueue(std::shared_ptr<Airplane> airplane)
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT #%d] - Queue in Runway: AIRPLANE #%d\n", this->getID(), airplane->getID());
    lck.unlock();

    // (1) First the airplane must have a port assigned to him before moving on
    std::promise<void> promiseAssignPort;
    std::future<void> futureAirplaneAssignedPort = promiseAssignPort.get_future();

    _waitingPortQueue.pushBack(airplane, std::move(promiseAssignPort));
    
    futureAirplaneAssignedPort.wait();

    // Assign port to the plane
    airplane->setPortID(this->getPort());

    lck.lock();
    printf("[PORT #%d] - Enter Runway: AIRPLANE #%d\n", this->getID(), airplane->getID());
    printf("[PORT #%d] - Current Port count: %d\n", getID(),this->getPortCount());
}

// Generates random delay and starts a timer in ms for the airplane to wait in port.
void Terminal::startRandomWait(double min_ms, double max_ms)
{
    int range = max_ms - min_ms;
    int timeDelay = (rand() % abs(range)) + min_ms;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT #%d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}

void Port::processPortQueue()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT #%d] - Starting port queue processing\n",getID());
    lck.unlock();

    // Continually process the queue
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (_waitingPortQueue.getSize() > 0 && getPortCount() > 0) 
        {
            // this->decPortCount();

            // Let the airplane proceed into the runway
            _waitingPortQueue.permitEntry();
        }
    }

}

// Port Management
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

int Port::getPort()
{
    std::lock_guard<std::mutex> lock(_count_mtx);
    _port_count--;
    return _const_port_count - _port_count;
}

// Returns the port position in the runway
void Port::getPortPosition(double &x, double &y, int port_id)
{
    x = _posX + 200;
    y = _posY + (port_id * 100);
}