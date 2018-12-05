#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cv.h>
#include <highgui.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

#include <time.h>

#include "imageanalyser.h"
#include <QtDebug>

#define WIDTH_CHESS_CASE 1
#define HEIGHT_CHESS_CASE 1
#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6


class CameraCalibration
{
public:

    /**
     * @brief find
     * @param mat
     */
    static bool calibrateFromImages(const std::vector<cv::Mat>& sources_images, cv::Mat& out);

    /**
     * @brief find
     * @param mat
     */
    static bool findCalibrate(const cv::Mat &source, cv::Mat& out);

};

#endif // CAMERACALIBRATION_H
