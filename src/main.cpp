#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"
#include "AirportObject.hpp"
#include "Gui.hpp"

#include <ctime>
#include <iostream>

void createAirport(std::shared_ptr<Runway> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes);

int main()
{
    srand(time(0));

    // auto landing_runway = std::make_shared<Runway>(); // OR
    // auto boeing = std::make_shared<Airplane>();
    // boeing->setCurrentRunway(landing_runway);

    // boeing->simulate();
    // landing_runway->simulate();


    // Initialise simulation
    auto landing_runway = std::make_shared<Runway>(); // OR
    std::vector< std::shared_ptr<Airplane> > airplanes;

    createAirport(landing_runway, airplanes, 5);

    // Begin simulation
    landing_runway->simulate();
    std::for_each(airplanes.begin(), airplanes.end(), [](std::shared_ptr<Airplane> &a)
    {
        a->simulate();
    });

    // Needed to keep the main thread for leaving scope to early thereby destroying the object
    while(true) 
    {

    }
    return 0;
}

void createAirport(std::shared_ptr<Runway> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes)
{
    for (int i = 0; i < nAirplanes; i++)
    {
        airplanes.push_back(std::make_shared<Airplane>());
        airplanes.at(i)->setCurrentRunway(runway);
    }

}