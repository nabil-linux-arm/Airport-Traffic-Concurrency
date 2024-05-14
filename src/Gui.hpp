#ifndef GUI_H
#define GUI_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "AirportObject.hpp"

class Gui
{
    public:
        // constructor / desctructor

        // getters / setters
        void setBgFilename(std::string filename) { _bgFilename = filename; }
        void setAirportObjects(std::vector<std::shared_ptr<AirportObject>> &AirportObject) { _airportObjects = AirportObject; };

        // typical behaviour methods
        void simulate();

    private:
        // typical behaviour methods
        void loadBackgroundImg();
        void drawAirportObjects();

        // member variables
        std::vector<std::shared_ptr<AirportObject>> _airportObjects;
        std::string _bgFilename;
        std::string _windowName;
        std::vector<cv::Mat> _images;
};

#endif