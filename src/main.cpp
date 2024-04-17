#include "Airplane.hpp"
#include "Runway.hpp"
#include "Port.hpp"
#include "AirportObject.hpp"
#include "Gui.hpp"

#include <ctime>

int main()
{
    srand(time(0));
    Airplane boeing_01; 
    boeing_01.simulate();
    return 0;
}