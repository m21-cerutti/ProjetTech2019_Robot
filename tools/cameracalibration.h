#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

#include <fstream>

//Debug
//#include "tools/interfacecvqt.h"

#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6


class CameraCalibration
{
public:

    static bool calibrateFromImages(const std::vector<cv::Mat>& sources_images, cv::Mat& out);

    /**
     * @brief Find a special calibration for one image.
     * Put the result in the file "default_camera.txt" in the build directory.
     * @param source the source matrice.
     * @param out the output matrice, undistorded camera.
     * @return if we have find a chess correspondence
     */
    static bool findOneCalibration(const cv::Mat &source, cv::Mat& out);

};

#endif // CAMERACALIBRATION_H
