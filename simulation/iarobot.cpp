#include "simulation/iarobot.h"

IARobot::IARobot():
    CustomController()
{
    load();
    calib.load("calib_stereo_simulation.xml");
}

IARobot::~IARobot()
{

}

void IARobot::simulate(const Mat &left_img, const Mat &right_img)
{
    try
    {
        process(left_img, right_img, &vx, &vy, &omega);
        Utilities::messageDebug("Controller value : vx "+std::to_string(vx)+"; vy "+std::to_string(vy)+"; o "+std::to_string(omega), false);
    }
    catch (Exception e)
    {
        Utilities::messageDebug(e.what());
    }
}

