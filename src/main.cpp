#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"
#include "AirportObject.hpp"
#include "Gui.hpp"

#include <ctime>
#include <iostream>

void createAirport(std::shared_ptr<Runway> &runway,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes);
void createAirportObjects(std::vector< std::shared_ptr<Runway> > &runways,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes, std::string &filename);

int main()
{
    srand(time(0));
    int num_plane = 0;

    // Background Image file name:
    std::string bck_image_name;

    // Get number of planes
    std::cout << "Starting simulation ..." << std::endl;
    std::cout << "Number of Planes: ";
    std::cin >> num_plane;
    std::cout << "Planes: " << num_plane << "\n\n";

    // ------ Initialise simulation ------- //
    std::vector< std::shared_ptr<Airplane> > airplanes;
    std::vector< std::shared_ptr<Runway> > runways;

    createAirportObjects(runways, airplanes, num_plane, bck_image_name);

    // ------- Begin simulation -------- //
    std::for_each(runways.begin(), runways.end(), [](std::shared_ptr<Runway> &r)
    {
        r->simulate();
    });

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

void createAirportObjects(std::vector< std::shared_ptr<Runway> > &runways,std::vector< std::shared_ptr<Airplane> > &airplanes,int nAirplanes, std::string &filename)
{
    // Background image for chosen airport
    filename = "../data/airport_vector_2.jpg";

    // Initialise Runways
    auto landing_runway = std::make_shared<Runway>(); 
    auto port_runway = std::make_shared<Port>(4); 
    auto exit_runway = std::make_shared<Runway>();
    auto sky_runway = std::make_shared<Runway>(sky);

    runways.push_back(landing_runway);
    runways.push_back(port_runway);
    runways.push_back(exit_runway);
    runways.push_back(sky_runway);

    // Position Runways and set their next runway
    landing_runway->setPosition(600, 200);
    port_runway->setPosition(1150, 500);
    exit_runway->setPosition(2100, 560);
    sky_runway->setPosition(600, 1400);

    landing_runway->setExitRunway(port_runway);
    port_runway->setExitRunway(exit_runway);
    exit_runway->setExitRunway(sky_runway);
    sky_runway->setExitRunway(landing_runway);

    // Initialise Airplanes
    for (int i = 0; i < nAirplanes; i++)
    {
        airplanes.push_back(std::make_shared<Airplane>());
        airplanes.at(i)->setCurrentRunway(runways.at(0));
    }
}