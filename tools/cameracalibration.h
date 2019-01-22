#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

#include <fstream>

//Debug
#include "tools/projectdebuger.h"

#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6
#define FILE_CAMERA_PATH "default_camera.txt"

class CameraCalibration
{
public:

    static void calibrateFromImages(const std::vector<cv::Mat>& sources_images);

    /**
     * @brief Find a special calibration for one image.
     * Put the result in the file "default_camera.txt" in the build directory.
     * @param source the source matrice.
     * @param out the output matrice, undistorded camera.
     * @return if we have find a chess correspondence
     */
    static bool findOneCalibration(const cv::Mat &source, cv::Mat& out);


    static void applyCameraParametersUndistorded(const std::string file_path, const cv::Mat& source, cv::Mat& out);


    static void saveCameraParameters(const std::string file_path, cv::Mat intrinsic,  cv::Mat dist_coeffs, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs );
};

#endif // CAMERACALIBRATION_H
