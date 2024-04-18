#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"
#include "AirportObject.hpp"
#include "Gui.hpp"

#include <ctime>

int main()
{
    srand(time(0));
    auto landing_runway = std::make_shared<Runway>(); // OR
    std::shared_ptr<Airplane> boeing_01(new Airplane);
    boeing_01->setCurrentRunway(landing_runway);
    boeing_01->simulate();
    return 0;
}