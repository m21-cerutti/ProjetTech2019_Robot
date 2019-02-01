#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/aruco/dictionary.hpp>

#include <fstream>

//Debug
#include "tools/projectdebuger.h"

//Board chess
#define SQUARE_SIZE (2*10e-2)
#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6

//Board Charuco
#define MARKER_SIZE (1*10e-2)
#define CHARU_WIDHT 9
#define CHARU_HEIGHT 6

#define DEFAULT_FILE_CAMERA_PATH "calib_default_camera.xml"

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

public:

    static int chessBoardCalibration(const std::vector<cv::Mat>& sources_images,std::string path_camera_file);

    static int charucoCalibration(const std::vector<cv::Mat>& sources_images, std::string path_camera_file);

    /**
     * @brief Find a special calibration for one image.
     * Put the result in the file "default_camera.txt" in the build directory.
     * @param source the source matrice.
     * @param out the output matrice, undistorded camera.
     * @return if we have find a chess correspondence
     */
    static void calibrateFromImages(const std::vector<cv::Mat>& sources_images, std::string path_camera_file, MODE_CALIBRATION behaviour = Chessboard);

    static void applyUndistordedFromFile(const std::string file_path, const cv::Mat& source, cv::Mat& out);

    static bool loadCameraParemeters(const std::string file_path, cv::Mat& camera_matrix,  cv::Mat& dist_coeffs, std::vector<cv::Mat>& rvecs, std::vector<cv::Mat>& tvecs);

    static bool saveCameraParameters(const std::string file_path, const cv::Mat &camera_matrix,  const cv::Mat &dist_coeffs, const std::vector<cv::Mat> &rvecs, const std::vector<cv::Mat> &tvecs );


};

#endif // CAMERACALIBRATION_H
