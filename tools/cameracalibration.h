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

#define DEFAULT_FILE_CAMERA_PATH "calib_default_camera.xml"

class CameraCalibration
{

    ///Constants
public:

    static std::string DEFAULT_CAMERA_PATH(){return DEFAULT_FILE_CAMERA_PATH;}


    ///Functions

public:

    /**
     * @brief chessBoardCalibration
     * @param sources_images
     * @param path_camera_file
     * @return
     */
    static int chessBoardCalibration(const std::vector<cv::Mat>& sources_images, const std::string path_camera_file);

    /**
     * @brief chessBoardCalibration
     * @param sources_images
     * @param path_camera_file
     * @param camera_matrix
     * @param dist_coeffs
     * @param rvecs
     * @param tvecs
     * @return
     */
    static int chessBoardCalibration(const std::vector<cv::Mat>& sources_images,
                                     const std::string path_camera_file,
                                     cv::Mat& camera_matrix,
                                     cv::Mat& dist_coeffs,
                                     std::vector<cv::Mat>& rvecs,
                                     std::vector<cv::Mat>& tvecs);

    /**
     * @brief charucoCalibration
     * @param sources_images
     * @param path_camera_file
     * @param camera_matrix
     * @param dist_coeffs
     * @param rvecs
     * @param tvecs
     * @return
     */
    static int charucoCalibration(const std::vector<cv::Mat>& sources_images, const std::string path_camera_file);

    /**
     * @brief charucoCalibration
     * @param sources_images
     * @param path_camera_file
     * @param camera_matrix
     * @param dist_coeffs
     * @param rvecs
     * @param tvecs
     * @return
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
                                  const std::vector<cv::Mat>& sources_images_left, const std::vector<cv::Mat> &sources_images_right,
                                  const std::string path_camera_l,
                                  const std::string path_camera_r);


};

#endif // CAMERACALIBRATION_H
