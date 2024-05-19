#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Gui.hpp"

void Gui::simulate()
{
    this->loadBackgroundImg();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // update graphics
        this->drawAirportObjects();
    }
}

void Gui::loadBackgroundImg()
{
    // create window
    _windowName = "Concurrency Airport Simulation";
    cv::namedWindow(_windowName, cv::WINDOW_NORMAL);

    // load image and create copy to be used for semi-transparent overlay
    cv::Mat background = cv::imread(_bgFilename);
    _images.push_back(background);         // first element is the original background
    _images.push_back(background.clone()); // second element will be the transparent overlay
    _images.push_back(background.clone()); // third element will be the result image for display
}

void Gui::drawAirportObjects()
{
    // reset images
    _images.at(1) = _images.at(0).clone();
    _images.at(2) = _images.at(0).clone();

    // create overlay from all traffic objects
    for (auto it : _airportObjects)
    {
        double posX, posY;
        it->getPosition(posX, posY);

        if (it->getType() == ObjectType::airplane)
        {
            cv::RNG rng(it->getID());
            int g = rng.uniform(0, 255);
            int b = sqrt(255*255 - g*g - b*b);
            int r = rng.uniform(0, 255);
            cv::Scalar vehicleColor = cv::Scalar(b,g,r);
            cv::circle(_images.at(1), cv::Point2d(posX, posY), 40, vehicleColor, -1);
        }

    }

    float opacity = 0.95;
    cv::addWeighted(_images.at(1), opacity, _images.at(0), 1.0 - opacity, 0, _images.at(2));

    // display background and overlay image
    cv::imshow(_windowName, _images.at(2));
    cv::waitKey(33);
}