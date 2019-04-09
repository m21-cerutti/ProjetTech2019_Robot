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
    if(nb_frame < NB_FRAME_TEST_WINDOW)
    {
        Mat undist_left, undist_right;
        undistort(left_img, undist_left, calib.camera_matrix_l, calib.dist_coeffs_l);
        undistort(right_img, undist_right, calib.camera_matrix_r, calib.dist_coeffs_r);

        SGBMParamDialog dial(undist_left, undist_right);
        if(dial.exec() != QDialog::Rejected)
        {
            load();
        }
    }

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

