#include <algorithm>
#include <iostream>
#include <thread>
#include "AirportObject.hpp"

int AirportObject::_id_count = 0;
std::mutex AirportObject::_cout_mtx;

/**
 * Setter for the position of the object.
*/
void AirportObject::setPosition(double x, double y)
{
    _posX = x;
    _posY = y;
}

/**
 * Getter for the position of the object.
 * 
 * @param x The variable to store the X coordinate.
 * @param y The variable to store the Y coordinate.
*/
void AirportObject::getPosition(double &x, double &y)
{
    x = _posX;
    y = _posY;
}

AirportObject::AirportObject()
{
    _type = ObjectType::noObject;
    _id = _id_count++;
}

AirportObject::~AirportObject()
{
    // Keeps track of all running threads spawned by the object.
    std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
        t.join();
    });
}