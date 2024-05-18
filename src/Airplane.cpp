#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"

#include <chrono>
#include <thread>
#include <iostream>

// TODO: 
// - [] Define what the airplane state is intitially 
// - [] Define some initial checks to make sure the simulation is safe
// - [] Remove all debugging logic and refactor the code

const int MIN_DELAY = 100;  // milliseconds
const int MAX_DELAY = 5000; // milliseconds

Airplane::Airplane()
{
    // _port = nullptr;
    _currentRunway = nullptr;
    _type = airplane;
    _speed = 150; // m/s
    _portID = 0;
}

Airplane::~Airplane()
{
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Airplane] - DESTRUCTOR CALLED\n");
}

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

    this->setNextRunway(_currentRunway->getExitRunway());
    // Creates a threasd for the move function of this airplane
    // emplace_back is used instead of push because thread objects cannot be copied (done when pushing objects)
    // Sufficient amount of time must pass in order for thread object to increment the shared_ptr
    // count for the object. If not the Main thread will finish before this thread and deallocate it.
    _threads.emplace_back(std::thread(&Airplane::move, this));
}

void Airplane::setCurrentRunway(std::shared_ptr<Runway> runway)
{
    _currentRunway = runway;
    _posRunway = 0.0;
}

void Airplane::setNextRunway(std::shared_ptr<Runway> runway)
{
    _nextRunway = runway;
}

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
                }
                
                // // ----- RUNWAY IS NOT A PORT ------ //
                // _currentRunway->addAirplaneToQueue(get_shared_this());

                // // Set src and destination coordinates
                // _currentRunway->getPosition(source.x, source.y);
                // _currentRunway->getExitRunway()->getPosition(destination.x, destination.y);

                // setStartPosition(source);
                // setDestinationPosition(destination); 

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

            // lck.lock();
            // std::cout << "Current src: " << _startPos.x << " " << _startPos.y << std::endl;
            // std::cout << "Current destination: " << _destinationPos.x << " " << _destinationPos.y << std::endl;
            // std::cout << "Completion: " << completion << std::endl;
            // lck.unlock();

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
                    this->startTimer(MIN_DELAY, MAX_DELAY+2000);

                    port_runway->getPortPosition(start.x, start.y, this->getPortID());
                    this->setStartPosition(start);

                    // Reset all port parameters to default
                    port_runway->freePort(this->getPortID());
                    this->setPortID(0);

                    hasEnteredPort = true;
                    port_runway->incPortCount(); 
                }
                
            }


            lastUpdate = std::chrono::system_clock::now();
        }
    }
}                   

void Airplane::moveToPort(int port_id) {}   

void Airplane::startTimer(int min, int max)
{
    // TODO: Create a limits for min and max
    int range = max - min;
    int timeDelay = (rand() % abs(range)) + min;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[AIRPLANE #%d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}

double Airplane::calculateDistance()
{
    double x_length = _startPos.x - _destinationPos.x;
    double y_length = _startPos.y - _destinationPos.y;
    double length = sqrt((x_length*x_length) + (y_length*y_length)); 
    return length;
}