#ifndef AIRPORTOBJECT_H
#define AIRPORTOBJECT_H

#include <vector>
#include <thread>
#include <mutex>

enum ObjectType
{
    airplane,
    runway,
    port,
    noObject
};

class AirportObject
{
    public:
        // Constructors/Destructors
        AirportObject();
        ~AirportObject();

    protected:
        ObjectType _type;                        // Type of object being created
        int _id;                                 // Every object is created with a unique id
        double _posX, _posY;                     // The coordinate position of the object on GUI
        std::vector<std::thread> _threads;       // A list that holds all threads spawned by this object
        static std::mutex _cout_mtx;             // A mutex to protect common cout

    private:
        static int _id_count;                    // Counter to keep track of created object ids
};

#endif
