# Airport-Runway-Traffic-Simulation

## Project Description

The aim of this project is to simulate the airplane traffic within an airport using concurrency concepts such as *threads, tasks, mutexes, locks and monitor objects*. The simulation will be displayed on a GUI to showcase the functionality of the program in a more user friendly way. The minimum viable product for this project will be a GUI that displays the airport traffic and airplanes following concurrency rules. The main idea behind this project is that **Only ONE airplane is allowed to travel in the runway at any given time**. My intention was to simulate the management of airplane movement within an airport which is important because they are giant vehicles and are not very manouverable on the ground. Additionally, airplanes which takeoff and land require the entire runway to be clear or risk a lot of lives. The simulation will automatically prevent more than one airplane to enter the runway.

## Dependencies

* cmake >= 2.8
* make >= 3.81
* openCV >= 4.1.0
    * Mac: There might be a problem with running the program for the first time and the GUI will not show. The culprit seemed to be opencv installation. Attempt to add opencv to the $PATH variable.
* gcc/g++ >= 5.4
    * Linux: usually pre-installed in most distributions
    * Mac: [Install GCC on mac](https://osxdaily.com/2023/05/02/how-install-gcc-mac/)

## Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./airport_traffic`.

## Minimum Viable Product

* Only 1 airplane moves in the runway at a time.
* Airplanes will wait and remain stationary at the start of the runway until it is clear.
* Airplanes can be assigned to a specific port on the terminal and be able to enter and exit the terminal.
* GUI that has an airport background and each airplane will have a different colour.

## Future Improvements

* Easier system to create airport layout and connect runways together.
* Improved GUI:
    * Airplanes are represented as actual airplanes.
    * Clearer runways and taxiways.
    * Better terminals.
    * Dashboard to present stats.
    * Buttons to change values while simulation is running.
* Algorithm improvements:
    * Landing queues:
        * Airplanes LAND then WAIT
        * Airplanes WAIT then LAND
    * Terminal port assignement:
        * Nearest-port
        * Static or pre-defined ports (maybe according to airplane flight)
    * Asynchronous Taxiway: Airplanes can move in and out of terminals whilst other airplanes do the same, without waiting for the Taxiway to be clear.
* When airplanes takeoff and leave the airport they will move in a predefined flight path for a random time until they can land back into the airport.