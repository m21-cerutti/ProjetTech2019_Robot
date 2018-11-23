#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <opencv2/core/core.hpp>
#include <opencv/cvwimage.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/affine.hpp>

#include  <opencv2/ximgproc/disparity_filter.hpp>

#include <time.h>

#define WIDTH_CHESS_CASE 1
#define HEIGHT_CHESS_CASE 1

class CameraCalibration
{
public:

    /**
     * @brief find
     * @param mat
     */
    static bool calibrateFromImages(std::vector<cv::Mat>& sources_images, cv::Mat& out);

    /**
     * @brief find
     * @param mat
     */
    static bool calibrateCamera(cv::Mat source, cv::Mat& out);

};

#endif // CAMERACALIBRATION_H
