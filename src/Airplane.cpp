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
    _speed = 100; // m/s
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
            // ------ Airplane first enters the queue before being allowed too move ----- //
            if (!hasEnteredRunway)
            {
                _currentRunway->addAirplaneToQueue(get_shared_this());

                hasEnteredRunway = true;
            }

            // ------- If Airplane is on Port Runway ------- //
            if(this->isPortAssigned() == true)
            {
                // Wait simulation of the port
                startTimer(2000, 10000);


                // Signal that it is finished with the port
                this->setPortAssigned(false);

                // Convert to derived Port class
                std::shared_ptr<Port> runway_port = std::dynamic_pointer_cast<Port>(_nextRunway);

                // Make available the current port
                runway_port->incPortCount();
            }

            // ------ Compute position on Runway and determine next position ------ //
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
            // std::cout << "Pos: " << dx << " "<< dy << std::endl;
            // std::cout << "Pos: " << xv << " "<< yv << std::endl;
            // std::cout << "Completion: " << completion << " Position in Runway: " << _posRunway << std::endl;
            lck.unlock();

            // ------- Airplane has reached the end of the runway -------- //
            if (completion >= 1.0 && hasEnteredRunway)
            {
                // Signal that the runway is clear
                _currentRunway->runwayClear();
                hasEnteredRunway = false;

                if(_currentRunway->getExitRunway() != nullptr)
                {
                    setCurrentRunway(_currentRunway->getExitRunway());
                }
            }

            lastUpdate = std::chrono::system_clock::now();
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