#ifndef STEREOCAMERA_H
#define STEREOCAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv/cvwimage.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/affine.hpp>

#include "tools/utilities.h"

using namespace cv;

namespace StereoCamera
{

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
int stereoChessboard(const std::vector<cv::Mat>& sources_images_left,
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
void stereoCalibration(const std::string path_file,
                              const std::vector<cv::Mat>& sources_images_left,
                              const std::vector<cv::Mat> &sources_images_right,
                              const std::string path_camera_file_l,
                              const std::string path_camera_file_r);

/**
 * @brief Make disparity map with BM algorithm
 * @param src the matrix to compute with left and right image
 * @param out the output matrix.
 * @param bmState he parameters initialised for computing
 *     PRESET
 *     nbdisparities
 *     SADWindowsSize
 */
static void computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state);

/**
 * @brief Make disparity map with BM algorithm.
 * @param src_left the source image left.
 * @param src_right the source image right.
 * @param out the output matrix.
 * @param bmState he parameters initialised for computing
 *     PRESET
 *     nbdisparities
 *     SADWindowsSize
 */
static void computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state);


/**
 * @brief Make disparity map with SGBM algorithm.
 * @param src the matrix to compute with left and right image.
 * @param out the output matrix.
 * @param sgbmState the parameters initialised for computing
 *     preFilterCap
 *     fullDP
 *     P1
 *     P2
 *     minDisparity
 *     numberOfDisparities
 *     uniquenessRatio
 *     speckleWindowSize
 *     speckleRange
 *     disp12MaxDiff
 *     SADWindowSize
 */
static void computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state);

/**
 * @brief Make disparity map with SGBM algorithm.
 * @param src_left the source image left.
 * @param src_right the source image right.
 * @param out the output matrix.
 * @param sgbmState the parameters initialised for computing
 *     preFilterCap
 *     fullDP
 *     P1
 *     P2
 *     minDisparity
 *     numberOfDisparities
 *     uniquenessRatio
 *     speckleWindowSize
 *     speckleRange
 *     disp12MaxDiff
 *     SADWindowSize
 */
static void computeSGBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state);

/**
 * @brief Make the depth map.
 * @param disparity Input the disparity map use.
 * @param Q Input 4×4 disparity-to-depth mapping matrix.
 * @param image_3d the Output matrix.
 */
static void computeDepthMap(const cv::Mat& disparity, const cv::Mat& Q, cv::Mat& depth_map);


/**
 * @brief Load the stereo calibration.
 * @param file_path
 * @param camera_matrix_l
 * @param dist_coeffs_l
 * @param camera_matrix_r
 * @param dist_coeffs_r
 * @param img_size
 * @param R
 * @param F
 * @param E
 * @param T
 * @param R1
 * @param R2
 * @param P1
 * @param P2
 * @param Q
 * @see cv::stereoCalibrate and cv::setreoRectify
 * @return
 */
bool loadCameraStereoParameters(const std::string file_path,
                                       cv::Mat &camera_matrix_l,
                                       cv::Mat &dist_coeffs_l,
                                       cv::Mat &camera_matrix_r,
                                       cv::Mat &dist_coeffs_r,
                                       cv::Size &img_size,
                                       cv::Mat &R,
                                       cv::Mat &F,
                                       cv::Mat &E,
                                       cv::Vec3d &T,
                                       cv::Mat &R1,
                                       cv::Mat &R2,
                                       cv::Mat &P1,
                                       cv::Mat &P2,
                                       cv::Mat &Q);

/**
 * @brief Save the stereo calibration.
 * @param file_path the path for saving stereo calibration.
 * @param camera_matrix_l
 * @param dist_coeffs_l
 * @param camera_matrix_r
 * @param dist_coeffs_r
 * @param img_size
 * @param R
 * @param F
 * @param E
 * @param T
 * @param R1
 * @param R2
 * @param P1
 * @param P2
 * @param Q
 * @see cv::stereoCalibrate and cv::setreoRectify
 * @return true if suceeded, false otherwise.
 */
bool saveCameraStereoParameters(const std::string file_path,
                                       const cv::Mat &camera_matrix_l,
                                       const cv::Mat &dist_coeffs_l,
                                       const cv::Mat &camera_matrix_r,
                                       const cv::Mat &dist_coeffs_r,
                                       const cv::Size &img_size,
                                       const cv::Mat &R,
                                       const cv::Mat &F,
                                       const cv::Mat &E,
                                       const cv::Vec3d &T,
                                       const cv::Mat &R1,
                                       const cv::Mat &R2,
                                       const cv::Mat &P1,
                                       const cv::Mat &P2,
                                       const cv::Mat &Q);
};

};

#endif // STEREOCAMERA_H
