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
        bmState = dial.getBMState();
        ProjectFiles::getMatrixCalibrationFileStorage("stereo_calibration.xml", "Q", Q);

        //qDebug()<<"Q\n"<< (Q.type()==CV_32F) << ProjectUtilities::matToString<double>(Q).c_str();


        StereoMap::computeBMDisparityStereo(img_left, img_right, disparity, bmState);

        StereoMap::computeDepthMap(disparity, Q, reproj);

        qDebug() <<">>>>>>>>>>>>>\n";
        qDebug() << QString(ProjectUtilities::matToString<double>(reproj).c_str());
        qDebug() << "End "<<">>>>>>>>>>>>>\n";

        double min;
        double max;

        cv::minMaxIdx(reproj, &min, &max);
        cv::Mat adjMap;
        // expand your range to 0..255. Similar to histEq();
        float scale = 255 / (max-min);
        reproj.convertTo(adjMap, CV_8UC1, scale, -min*scale);

        // this is great. It converts your grayscale image into a tone-mapped one,
        // much more pleasing for the eye
        // function is found in contrib module, so include contrib.hpp
        // and link accordingly
        cv::Mat falseColorsMap;
        applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_AUTUMN);
        ProjectUtilities::showMatrice("3D", falseColorsMap);
    }
}
