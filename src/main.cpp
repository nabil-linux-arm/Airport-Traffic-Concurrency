#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"
#include "AirportObject.hpp"
#include "Gui.hpp"

#include <ctime>
#include <iostream>

void createAirport(std::shared_ptr<Runway> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes);
void createAirport(std::shared_ptr<Port> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes);

int main()
{
    srand(time(0));
    int num_plane = 0;
    int num_ports = 0;

    // Background Image file name:
    std::string bck_image_name = "../data/paris.jpg";

    // Get number of planes
    std::cout << "Starting simulation ..." << std::endl;
    std::cout << "Number of Planes: ";
    std::cin >> num_plane;
    std::cout << "Planes: " << num_plane << "\n\n";

    // ------ Initialise simulation ------- //
    auto landing_runway = std::make_shared<Runway>(); 
    // auto port_runway = std::make_shared<Port>(5); 
    std::vector< std::shared_ptr<Airplane> > airplanes;
    std::vector< std::shared_ptr<Runway> > runways;

    runways.push_back(landing_runway);
    // runways.push_back(port_runway);
    
    createAirport(landing_runway, airplanes, num_plane);

    // Set exit ruways (this one will be cyclic)
    landing_runway->setPosition(1430, 625);
    landing_runway->setLength(500);
    // landing_runway->setExitRunway(port_runway);
    // port_runway->setExitRunway(landing_runway);

    // ------- Begin simulation -------- //
    landing_runway->simulate();
    // port_runway->simulate();
    std::for_each(airplanes.begin(), airplanes.end(), [](std::shared_ptr<Airplane> &a)
    {
        a->simulate();
    });

    // ------- Begin Gui animation ------- //
    std::vector<std::shared_ptr<AirportObject> > airportObjects;

    std::for_each(runways.begin(), runways.end(), [&airportObjects](std::shared_ptr<Runway> &runway) {
        std::shared_ptr<AirportObject> airport_object = std::dynamic_pointer_cast<AirportObject>(runway);
        airportObjects.push_back(airport_object);
    });

    std::for_each(airplanes.begin(), airplanes.end(), [&airportObjects](std::shared_ptr<Airplane> &airplane) {
        std::shared_ptr<AirportObject> airport_object = std::dynamic_pointer_cast<AirportObject>(airplane);
        airportObjects.push_back(airport_object);
    });

    // Draw all objects
    Gui *graphics = new Gui();
    graphics->setBgFilename(bck_image_name);
    graphics->setAirportObjects(airportObjects);
    graphics->simulate();

    // Needed to keep the main thread for leaving scope to early thereby destroying the object
    // while(true) 
    // {

    // }
    return 0;
}

void createAirport(std::shared_ptr<Runway> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes)
{
    for (int i = 0; i < nAirplanes; i++)
    {
        airplanes.push_back(std::make_shared<Airplane>());
        airplanes.at(i)->setCurrentRunway(runway);
        // airplanes.at(i)->setPosition(400,400 + i*200);
    }

}

void createAirport(std::shared_ptr<Port> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes)
{
    for (int i = 0; i < nAirplanes; i++)
    {
        airplanes.push_back(std::make_shared<Airplane>());
        airplanes.at(i)->setCurrentRunway(runway);
    }
}
