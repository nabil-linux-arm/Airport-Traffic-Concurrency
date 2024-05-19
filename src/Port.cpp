#include "Port.hpp"
#include "Airplane.hpp"

Port::Port()
{
    setRunwayType(port);
    _port_count = 1;
    _type = runway;
    setLength(40.0);

    _port_pool.push_back(1);
}

/**
 * Secondary constructor to define number of ports in port.
 * 
 * @param n_ports The number of ports the port object will initialise with.
*/
Port::Port(int n_ports) : _port_count(n_ports)
{
    setRunwayType(port);
    _type = runway;
    setLength(40.0);

    // Fill up the port pool
    for (int i = n_ports; i > 0; i--)
    {
        _port_pool.push_back(i);
    }
}

/**
 * Starts thread for Port.
 * 
 * Specifially starts threads for port::processPortQueue() and runway::processAirplaneQueue.
*/
void Port::simulate()
{
    // To manage airplanes waiting for unblocked runway
    _threads.emplace_back(std::thread(&Port::processAirplaneQueue, this));
    // To manage airplanes waiting for a port to be assigned to them
    _threads.emplace_back(std::thread(&Port::processPortQueue, this));
}

/**
 * Adds airplane to port waiting queue.
 * 
 * Any airplanes that enters the port and have not been assigned a port can be added to 
 * this queue. Usually started as a seperate thread by the airplane. Airplanes currently 
 * cannot do anything while waiting in the queue.
 * 
 * @param airplane The airplane that wants to dock into an available port.
*/
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
}

/**
 * Main processing function for the port.
 * 
 * Continuously check whether there are available ports to be assigned. If so 
 * it will let an airplane enter the Runway.
*/
void Port::processPortQueue()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[PORT #%d] - Starting port queue processing\n",getID());
    lck.unlock();

    // Continually process the queue
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (_waitingPortQueue.getSize() > 0 && getPortCount() > 0) 
        {
            // Let the airplane proceed into the runway
            _waitingPortQueue.permitEntry();
        }
    }

}

// ----- Port Management ------ //

/**
 * Getter for the size of the port pool.
 * 
 * @return Number of ports available to be assigned to an airplane.
*/
int Port::getPortCount()
{ 
    std::lock_guard<std::mutex> lock(_count_mtx);
    // return _port_count; 
    return _port_pool.size();
}

/**
 * Getter for the port id from the port pool.
 * 
 * Also removes the port id from the port pool thus simulating a limited
 * resource pool. The port pool is a very primitive type of resource pool.
 * 
 * @return Port id that is in the port pool.
*/
int Port::getPort()
{
    std::lock_guard<std::mutex> lock(_count_mtx);
    int free_port = 0; 
    // Safety feature to prevent undefined behaviour
    if (_port_pool.size() > 0)
    {
        free_port = _port_pool.back();
        _port_pool.pop_back();
    }

    return free_port;
}

/**
 * Adds a port id back into the port pool.
 * 
 * @param port_id The port being returned back to the the port pool.
*/
void Port::freePort(int port_id)
{
    std::lock_guard<std::mutex> lock(_count_mtx);
    _port_pool.push_back(port_id);
}

/**
 * Returns the position of a port relative to the runway and
 * according to the port's id.
 * 
 * @param x X coordinate of the port.
 * @param y Y coordinate of the port.
 * @param port_id The port being questioned about it's position.
*/
void Port::getPortPosition(double &x, double &y, int port_id)
{
    // When runway parallel to the x axis
    x = _posX + (port_id * 150);
    y = _posY - 200;

    // When runway parallel to the y axis
    // x = _posX + 200;
    // y = _posY + (port_id * 150);
}