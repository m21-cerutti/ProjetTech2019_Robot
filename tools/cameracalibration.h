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

#include "tools/projectutilities.h"
#include "tools/projectfiles.h"

//in cm
//Board chess
#define SQUARE_SIZE 0.02
#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6

//Board Charuco
#define MARKER_SIZE 0.01
#define CHARU_WIDHT 9
#define CHARU_HEIGHT 6
#define MIN_MARKERS 4

#define DEFAULT_FILE_CAMERA_L"calib_default_camera_l.xml"
#define DEFAULT_FILE_CAMERA_R "calib_default_camera_r.xml"
#define DEFAULT_FILE_STEREO "calib_stereo.xml"


/**
 * @brief The CameraCalibration class, gather all the functions about camera calibration.
 */
class CameraCalibration
{

    ///Functions
public:

    /**
     * @brief Do the chessbard calibration for one camera.
     * @param sources_images the images for calibration.
     * @param path_camera_file the path to save the calibration.
     * @return The number of images rejected for calibration.
     */
    static int chessBoardCalibration(const std::vector<cv::Mat>& sources_images, const std::string path_camera_file);

    /**
     * @brief Do the chessbard calibration for one camera.
     * @param sources_images the images for calibration.
     * @param path_camera_file the path to save the calibration.
     * @param camera_matrix Output 3x3 floating-point camera matrix.
     * @param dist_coeffs Output vector of distortion coefficients.
     * @param rvecs Output vector of rotation vectors estimated for each pattern view.
     * @param tvecs Output vector of translation vectors estimated for each pattern view.
     * @see cv::calibrateCamera for more information.
     * @return The number of images rejected for calibration.
     */
    static int chessBoardCalibration(const std::vector<cv::Mat>& sources_images,
                                     const std::string path_camera_file,
                                     cv::Mat& camera_matrix,
                                     cv::Mat& dist_coeffs,
                                     std::vector<cv::Mat>& rvecs,
                                     std::vector<cv::Mat>& tvecs);

    /**
     * @brief Do the charuco calibration for one camera.
     * Not very effective, use chessBoardCalibration instead.
     * @param sources_images the images for calibration.
     * @param path_camera_file the path to save the calibration.
     * @return The number of images rejected for calibration.
     */
    static int charucoCalibration(const std::vector<cv::Mat>& sources_images, const std::string path_camera_file);

    /**
     * @brief Do the charuco calibration for one camera.
     * Not very effective, use chessBoardCalibration instead.
     * @param sources_images the images for calibration.
     * @param path_camera_file the path to save the calibration.
     * @param camera_matrix Output 3x3 floating-point camera matrix.
     * @param dist_coeffs Output vector of distortion coefficients.
     * @param rvecs Output vector of rotation vectors estimated for each pattern view.
     * @param tvecs Output vector of translation vectors estimated for each pattern view.
     * @see cv::calibrateCamera for more information.
     * @return The number of images rejected for calibration.
     */
    static int charucoCalibration(const std::vector<cv::Mat> &sources_images,
                                  const std::string path_camera_file,
                                  cv::Mat& camera_matrix,
                                  cv::Mat& dist_coeffs,
                                  std::vector<cv::Mat>& rvecs,
                                  std::vector<cv::Mat>& tvecs);

    /**
     * @brief applyUndistorded
     * @param source
     * @param out
     * @param camera_matrix
     * @param dist_coeffs
     */
    static void applyUndistorded( const cv::Mat& source,
                                  cv::Mat& out,
                                  cv::Mat& camera_matrix,
                                  cv::Mat& dist_coeffs);

    /**
     * @brief applyUndistordedFromFile
     * @param file_path
     * @param source
     * @param out
     */
    static void applyUndistorded(const cv::Mat& source, cv::Mat& out,
                                 const std::string file_path);


    /**
     * @brief stereoChessboard
     * @param sources_images_left
     * @param sources_images_right
     * @param path_camera_l
     * @param path_camera_r
     * @param object_points
     * @param left_img_points
     * @param right_img_points
     * @return
     */
    static int stereoChessboard(const std::vector<cv::Mat>& sources_images_left,
                                const std::vector<cv::Mat> &sources_images_right,
                                const std::string path_camera_l,
                                const std::string path_camera_r,
                                std::vector<std::vector<cv::Point3f> > &object_points,
                                std::vector<std::vector<cv::Point2f> > &left_img_points,
                                std::vector<std::vector<cv::Point2f> > &right_img_points);


    /**
     * @brief stereoCalibration
     * @param path_file
     * @param object_points
     * @param left_img_points
     * @param right_img_points
     * @param path_camera_l
     * @param path_camera_r
     */
    static void stereoCalibration(const std::string path_file,
                                  const std::vector<cv::Mat>& sources_images_left,
                                  const std::vector<cv::Mat> &sources_images_right,
                                  const std::string path_camera_l,
                                  const std::string path_camera_r);


};

#endif // CAMERACALIBRATION_H
