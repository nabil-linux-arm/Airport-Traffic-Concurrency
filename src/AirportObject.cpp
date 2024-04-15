#include <algorithm>
#include <iostream>
#include <thread>
#include "AirportObject.hpp"

int AirportObject::_id_count = 0;
std::mutex AirportObject::_cout_mtx;

void AirportObject::setPosition(double x, double y)
{
    _posX = x;
    _posY = y;
}

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
    std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
        t.join();
    });
}