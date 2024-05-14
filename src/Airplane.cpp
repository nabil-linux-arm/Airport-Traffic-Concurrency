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
    _speed = 50; // m/s
    _portAssigned = false;
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
        printf("[Airplane %d] - ERROR: runway not set\n", this->getID());
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
    double cycleDuration = 1; // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate >= cycleDuration)
        {
            // update position with a constant velocity motion model
            _posRunway += _speed * timeSinceLastUpdate / 1000;

            // compute completion rate of current street
            double completion = _posRunway / _currentRunway->getLength();

            // compute current pixel position on street based on driving direction
            std::shared_ptr<Runway> i1, i2;
            i2 = _currentRunway->getExitRunway();
            // i1 = i2->getID() == _currentRunway->getExitRunway()->getID() ? _currStreet->getOutIntersection() : _currStreet->getInIntersection();
            i1 = _currentRunway;

            double x1, y1, x2, y2, xv, yv, dx, dy, l;
            i1->getPosition(x1, y1);
            if (i2 == nullptr)
            {
                x2 = x1 + _currentRunway->getLength();
                y2 = y1;
            }
            else
            {
                i2->getPosition(x2, y2);
            }

            dx = x2 - x1;
            dy = y2 - y1;
            // l = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (x1 - x2));
            xv = x1 + completion * dx; // new position based on line equation in parameter form
            yv = y1 + completion * dy;
            this->setPosition(xv, yv);

            lck.lock();
            // std::cout << "Pos: " << xv << " "<< yv << std::endl;
            std::cout << "Completion: " << completion << std::endl;
            lck.unlock();

            // if (_currentRunway->getIsLandingRunway() == true)
            // {
            //     // Timer for when the airplane is in the air
            //     startTimer(MIN_DELAY, MAX_DELAY);
            // }
            // _currentRunway->addAirplaneToQueue(get_shared_this());

            // check wether halting position in front of destination has been reached
            if (completion >= 0.9 && !hasEnteredRunway)
            {
                // _currentRunway->addAirplaneToQueue(get_shared_this());
                if(this->_nextRunway != nullptr)
                {
                    _nextRunway->addAirplaneToQueue(get_shared_this());
                }

                // slow down and set intersection flag
                _speed /= 10.0;
                hasEnteredRunway = true;
            }

            if (completion >= 1.0 && hasEnteredRunway)
            {
                if (this->_nextRunway != nullptr)
                {
                    _currentRunway = _nextRunway;
                    _nextRunway = _nextRunway->getExitRunway();
                }
                hasEnteredRunway = false;
            }

            lastUpdate = std::chrono::system_clock::now();

            // // Simulates time it takes for a plane to fully travel across the runway or other stuff
            // std::this_thread::sleep_for(std::chrono::milliseconds(5000));

            // // Signal that the runway is clear
            // _currentRunway->runwayClear();

            // // If Port is assigned
            // if(this->isPortAssigned() == true)
            // {
            //     startTimer(2000, 10000);
            //     this->setPortAssigned(false);

            //     // Convert to derived Port class
            //     std::shared_ptr<Port> runway_port = std::dynamic_pointer_cast<Port>(_currentRunway);
            //     runway_port->incPortCount();
            // }

            // // Set new runway as next runway
            // if (_currentRunway->getExitRunway() != nullptr) 
            // {
            //     _currentRunway = _currentRunway->getExitRunway();
            // }

        }
    }
}                   

void Airplane::moveToPort(int port_id) {}   
bool Airplane::isDestinationReached() {}    

void Airplane::startTimer(int min, int max)
{
    // TODO: Create a limits for min and max
    int range = max - min;
    int timeDelay = (rand() % abs(range)) + min;
    std::unique_lock<std::mutex> lck(_cout_mtx);
    printf("[Airplane %d] - Start Timer: %d ms\n", this->getID(), timeDelay);
    lck.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
}