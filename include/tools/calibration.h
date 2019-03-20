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

#include "tools/utilities.h"
#include "tools/files.h"

//in cm
//Board chess
#define SQUARE_SIZE 2
#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6

//Board Charuco
#define MARKER_SIZE 1
#define CHARU_WIDHT 9
#define CHARU_HEIGHT 6
#define MIN_MARKERS 2

#define DEFAULT_FILE_CAMERA_L "calib_default_camera_l.xml"
#define DEFAULT_FILE_CAMERA_R "calib_default_camera_r.xml"
#define DEFAULT_FILE_STEREO "calib_stereo.xml"


/**
 * @brief The CameraCalibration class, gather all the functions about camera calibration.
 */
namespace Calibration
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
     * @brief Undistord using the camera calibration.
     * @param source the source matrix.
     * @param out the output matrix.
     * @param camera_matrix the input camera matrix calibration.
     * @param dist_coeffs the input distorsion coefficients calibration.
     */
    static void applyUndistorded( const cv::Mat& source,
                                  cv::Mat& out,
                                  cv::Mat& camera_matrix,
                                  cv::Mat& dist_coeffs);

    /**
     * @brief Undistord using the camera calibration.
     * @param file_path the camera calibration path.
     * @param source the source matrix.
     * @param out the output matrix.
     */
    static void applyUndistorded(const cv::Mat& source, cv::Mat& out,
                                 const std::string file_path);


    /**
     * @brief Do the stereo find chessboard, more restrictive than individual.
     * @param sources_images_left the left source matrix.
     * @param sources_images_right the right source matrix.
     * @param path_camera_l the path for save the calibration of left camera.
     * @param path_camera_r the path for save the calibration of right camera.
     * @param object_points Output array of theorical Point3f.
     * @param left_img_points Output array of Point2f for left correspondence.
     * @param right_img_points Output array of Point2f for right correspondence.
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
     * @brief Do the stereo calibration.
     * @param path_file the path for saving the calibration.
     * @param sources_images_left the source left image.
     * @param sources_images_right the source right image.
     * @param path_camera_l the path for saving left calibration.
     * @param path_camera_r the path for saving right calibration.
     */
    static void stereoCalibration(const std::string path_file,
                                  const std::vector<cv::Mat>& sources_images_left,
                                  const std::vector<cv::Mat> &sources_images_right,
                                  const std::string path_camera_file_l,
                                  const std::string path_camera_file_r);


};

#endif // CAMERACALIBRATION_H
