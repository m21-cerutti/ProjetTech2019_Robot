#include "iarobot.h"

IARobot::IARobot()
{
}

void IARobot::update(cv::Mat &img_left, cv::Mat &img_right)
{
    this->img_left = img_left;
    this->img_right = img_right;

    //ProjectUtilities::showMatrice("left", this->img_left);
    //ProjectUtilities::showMatrice("right", this->img_right);

}

void IARobot::onDisplay()
{
    cv::Mat Q, reproj, disparity;
    cv::Ptr<cv::StereoBM> bmState;

    QImage disp_q;
    cv::hconcat(img_left, img_right, disparity);
    CVQTInterface::toQImage(disparity, disp_q);
    BMParamDialog dial(disp_q);

    if(dial.exec() != QDialog::Rejected)
    {
        bmState = dial.getBMState();
        //ProjectFiles::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);
        /*
        StereoMap::computeBMDisparityStereo(img_left, img_right, disparity, bmState);
        ProjectUtilities::showMatrice("disparity", disparity);
        StereoMap::computeDepthMap(disparity, Q, reproj);
        ProjectUtilities::showMatrice("depth", reproj);
        */
    }
}
