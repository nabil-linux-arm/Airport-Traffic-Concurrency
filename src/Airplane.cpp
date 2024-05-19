#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"

#include <chrono>
#include <thread>
#include <iostream>

const int MIN_DELAY = 100;  // minimum time in milliseconds
const int MAX_DELAY = 5000; // maximum time in milliseconds

Airplane::Airplane()
{
    _currentRunway = nullptr;
    _type = airplane;
    _speed = _default_speed = 200; // m/s
    _portID = 0;
}

/**
 * Begins airplane thread.
 * 
 * Creates a thread for airplane::move() to begin the airplane program.
*/
void Airplane::simulate() 
{
    // Provide some error checking
    if (_currentRunway == nullptr) 
    {
        std::unique_lock<std::mutex> lck(_cout_mtx);
        printf("[AIRPLANE #%d] - ERROR: runway not set\n", this->getID());
        lck.unlock();
        return;
    }

    // Creates a threasd for the move function of this airplane
    // emplace_back is used instead of push because thread objects cannot be copied (done when pushing objects)
    // Sufficient amount of time must pass in order for thread object to increment the shared_ptr
    // count for the object. If not the Main thread will finish before this thread and deallocate it.
    _threads.emplace_back(std::thread(&Airplane::move, this));
}

/**
 * Sets the runway the airplane is currently on.
 * 
 * @param runway A shared pointer for a runway object.
*/
void Airplane::setCurrentRunway(std::shared_ptr<Runway> runway)
{
    _currentRunway = runway;
    _posRunway = 0.0;
}

/**
 * Executes the main state machine of the airplane.
 * 
 * Executed by airplane::simulate() on a seperate thread. Currently the main states include the Enter, Wait, Port and Exit states.
*/
void Airplane::move() 
{
    // print id of the current thread
    std::unique_lock<std::mutex> lck(_cout_mtx);
    std::cout << "Airplane #" << _id << "::move: thread id = " << std::this_thread::get_id() << std::endl;
    lck.unlock();

    bool hasEnteredRunway = false;
    bool hasEnteredPort = false;

    double cycleDuration = 1; // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate >= cycleDuration)
        {
            // ------- WAIT AND ENTER STATES ------- //
            if (!hasEnteredRunway)
            {
                position source;
                position destination;
                RunwayType current_type = _currentRunway->getRunwayType();
                _speed = _default_speed;

                if (current_type == port && !hasEnteredPort)
                {
                    // ----- BEFORE PORT VISIT ------ //
                    std::shared_ptr<Port> port_runway = std::dynamic_pointer_cast<Port>(_currentRunway);

                    port_runway->addAirplaneToPortQueue(this->get_shared_this());

                    port_runway->addAirplaneToQueue(this->get_shared_this());
                    
                    // Set src and destination coordiantes
                    port_runway->getPosition(source.x, source.y);
                    port_runway->getPortPosition(destination.x, destination.y, this->getPortID());

                    setStartPosition(source);
                    setDestinationPosition(destination); 
                }
                else if (hasEnteredPort)
                {
                    // ----- AFTER PORT VISIT ------ //
                    _currentRunway->addAirplaneToQueue(this->get_shared_this());

                    // Set src and destination coordinates
                    _currentRunway->getExitRunway()->getPosition(destination.x, destination.y);

                    setDestinationPosition(destination);
                }
                else if (current_type != port)
                {
                    // ----- RUNWAY IS NOT A PORT ------ //
                    _currentRunway->addAirplaneToQueue(this->get_shared_this());

                    // Set src and destination coordinates
                    _currentRunway->getPosition(source.x, source.y);
                    _currentRunway->getExitRunway()->getPosition(destination.x, destination.y);

                    setStartPosition(source);
                    setDestinationPosition(destination); 

                    if (_currentRunway->getExitRunway()->getRunwayType() == RunwayType::sky)
                    {
                        _speed += 200;
                    }
                }
                
                hasEnteredRunway = true;
            }

            // ----- CALCULATE POSITION VECTOR ------- //
            // update position with a constant velocity motion model
            _posRunway += _speed * timeSinceLastUpdate / 1000;

            // compute completion rate of current street
            double completion = _posRunway / this->calculateDistance();

            double xv, yv, dx, dy, l;

            dx = _destinationPos.x - _startPos.x;
            dy = _destinationPos.y - _startPos.y;
            xv = _startPos.x + completion * dx; // new position based on line equation in parameter form
            yv = _startPos.y + completion * dy;

            // Finally set the correct position
            this->setPosition(xv, yv);

            // ------- ACTION TO TAKE ONCE DESTINATION REACHED ------ //
            if (completion >= 1.0 && hasEnteredRunway)
            {
                _currentRunway->runwayClear();
                hasEnteredRunway = false;

                if (this->getPortID() == 0)
                {
                    // ------ Reached exit of runway ------ //
                    _currentRunway = _currentRunway->getExitRunway();
                    hasEnteredPort = false;
                }
                else
                {
                    // ----- Reached port ------ //
                    std::shared_ptr<Port> port_runway = std::dynamic_pointer_cast<Port>(_currentRunway);
                    position start;
                    // Simulate processing in the port
                    this->startTimer(MAX_DELAY, 20000);

                    port_runway->getPortPosition(start.x, start.y, this->getPortID());
                    this->setStartPosition(start);

                    // Reset all port parameters to default
                    port_runway->freePort(this->getPortID());
                    this->setPortID(0);

                    hasEnteredPort = true;
                }
                
            }


            lastUpdate = std::chrono::system_clock::now();
        }
    }
}                   

/**
 * Generates random delay.
 * 
 * Sleeps the airplane thread for a random period of time between a set min and max. 
 * Initially was first used to simulate airplane processing.
 * 
 * @param min The minimum delay the airplane will sleep for in milliseconds.
 * @param max The maximum delay the airplane will sleep for in milliseconds.
*/
void Airplane::startTimer(int min, int max)
{
    // TODO: 
    // - [] Create a limits for min and max
    int range = max - min;
    int timeDelay = (rand() % abs(range)) + min;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[AIRPLANE #%d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}

/**
 * Calculates the distance between the start and destination.
 * 
 * Uses internal variables _startPos and _destinationPos to calculate the 
 * length/distance between the two points. 
 * 
 * @return double type representing the length between start and destination.
*/
double Airplane::calculateDistance()
{
    double x_length = _startPos.x - _destinationPos.x;
    double y_length = _startPos.y - _destinationPos.y;
    double length = sqrt((x_length*x_length) + (y_length*y_length)); 
    return length;
}

/**
 * Setter for airplane starting position.
 * 
 * Usually the starting the position is the position of the current runway.
 * 
 * @param start A position struct representing the x and y coordinates of the starting position.
*/
void Airplane::setStartPosition(position start) { _startPos = start; }

/**
 * Setter for airplane destination.
 * 
 * Additionally resets the position in runway variable.
 * 
 * @param destination x and y pixel coordinates of the target destination.
*/
void Airplane::setDestinationPosition(position destination)
{
    _destinationPos = destination;
    _posRunway = 0.0;
}

/**
 * Setter for port id of port airplane is assigned to.
 * 
 * @param port_id The port id of the assigned port.
*/
void Airplane::setPortID(int port_id) { _portID = port_id; }

/**
 * Getter for port id of assigned port.
 * 
 * @return non-zero integer represents the port the airplane is assigned to. If zero the airplane is not assigned to a port.
*/
int Airplane::getPortID() { return _portID; }
