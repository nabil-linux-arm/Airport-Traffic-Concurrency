#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"
#include "AirportObject.hpp"
#include "Gui.hpp"

#include <ctime>
#include <iostream>

int main()
{
    srand(time(0));

    auto landing_runway = std::make_shared<Runway>(); // OR
    auto boeing = std::make_shared<Airplane>();
    // Airplane boeing;
    // boeing->setCurrentRunway(landing_runway);
    // std::thread qatar(&Airplane::move, boeing);
    // qatar.join();
    boeing->simulate();

    // Needed to keep the main thread for leaving scope to early thereby destroying the object
    while(true) 
    {

    }

    // Group of airplanes
    // std::vector< std::shared_ptr<Airplane> > airplanes;
    // for (int i = 0; i < 5; i++)
    // {
    //     airplanes.push_back(std::make_shared<Airplane>());
    // }

    // std::for_each(airplanes.begin(), airplanes.end(), [](std::shared_ptr<Airplane> &a)
    // {
    //     a->simulate();
    // });
    // std::shared_ptr<Airplane> boeing_01(new Airplane);
    return 0;
}