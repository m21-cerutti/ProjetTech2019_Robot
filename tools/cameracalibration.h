#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

#include <fstream>

//Debug
#include "tools/projectdebuger.h"

#define SQUARE_SIZE 5
#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6
#define DEFAULT_FILE_CAMERA_PATH "calib_default_camera.txt"

class CameraCalibration
{

    ///Constants
public:

    static std::string DEFAULT_CAMERA_PATH(){return DEFAULT_FILE_CAMERA_PATH;}

    enum MODE_CALIBRATION{
        Chessboard,
        Charuco
    };

    ///Functions
private:

    static int findChessBoard(const std::vector<cv::Mat>& sources_images,
                               std::vector<std::vector<cv::Point3f> > &object_points,
                               std::vector<std::vector<cv::Point2f> > &image_points);

public:

    /**
     * @brief Find a special calibration for one image.
     * Put the result in the file "default_camera.txt" in the build directory.
     * @param source the source matrice.
     * @param out the output matrice, undistorded camera.
     * @return if we have find a chess correspondence
     */
    static void calibrateFromImages(const std::vector<cv::Mat>& sources_images, std::string path_camera_file, MODE_CALIBRATION behaviour = Chessboard);

    static void applyCameraParametersUndistorded(const std::string file_path, const cv::Mat& source, cv::Mat& out);

    static void saveCameraParameters(const std::string file_path, cv::Mat intrinsic,  cv::Mat dist_coeffs, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs );


};

#endif // CAMERACALIBRATION_H
