#ifndef PROJECTFILES_H
#define PROJECTFILES_H

#include <cv.h>
#include <opencv2/opencv.hpp>

#include "tools/projectutilities.h"

/**
 * @brief The ProjectFiles class gather all functions about saves ahnd load of files.
 */
class ProjectFiles
{
public:

    /**
     * @brief Get a particular matrix parameter in a file. Need to be known.
     * @param file_path the path of the file.
     * @param name_parameter the name of the desired parameter.
     * @param out the matrix needed.
     * @return true if suceeded, false otherwise.
     */
    static bool getMatrixCalibrationFileStorage(const std::string file_path,
                                                const std::string name_parameter,
                                                cv::Mat& out);

    /**
     * @brief saveSetImages
     * @param prefix
     * @param images_left
     * @param images_right
     * @return
     */
    static bool saveSetImages(const std::string prefix, const std::vector<cv::Mat> images_left, const std::vector<cv::Mat> images_right);

    /**
      * @brief loadSetImages
      * @param prefix
      * @param images_left
      * @param images_right
      * @return
      */
     static bool loadSetImages(const std::string prefix, std::vector<cv::Mat> images_left, std::vector<cv::Mat> images_right);

    /**
     * @brief Load the intrinsic camera parameters.
     * @param file_path the path of the file.
     * @param img_size Output image size.
     * @param camera_matrix Output camera matrix.
     * @param dist_coeffs Output distorsion coefficients.
     * @param rvecs Output vector of rotation vectors estimated for each pattern view.
     * @param tvecs Output vector of translation vectors estimated for each pattern view.
     * @return true if suceeded, false otherwise.
     */
    static bool loadIntrinsicCamera(const std::string file_path,
                                    cv::Size &img_size,
                                    cv::Mat& camera_matrix,
                                    cv::Mat& dist_coeffs,
                                    std::vector<cv::Mat>& rvecs,
                                    std::vector<cv::Mat>& tvecs);

    /**
     * @brief Load the intrinsic camera parameters.
     * @param file_path the path of the file.
     * @param img_size Output image size.
     * @param camera_matrix Output camera matrix.
     * @param dist_coeffs Output distorsion coefficients.
     * @return true if suceeded, false otherwise.
     */
    static bool loadIntrinsicCamera(const std::string file_path,
                                    cv::Size &img_size,
                                    cv::Mat& camera_matrix,
                                    cv::Mat& dist_coeffs);

    /**
     * @brief Save the intrinsic camera parameters.
     * @param img_size Input image size.
     * @param camera_matrix Input camera matrix.
     * @param dist_coeffs Input distorsion coefficients.
     * @param rvecs Input vector of rotation vectors estimated for each pattern view.
     * @param tvecs Input vector of translation vectors estimated for each pattern view.
     * @return true if suceeded, false otherwise.
     */
    static bool saveIntrinsicCamera(const std::string file_path,
                                    const cv::Size img_size,
                                    const cv::Mat &camera_matrix,
                                    const cv::Mat &dist_coeffs,
                                    const std::vector<cv::Mat> &rvecs,
                                    const std::vector<cv::Mat> &tvecs );


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
    static bool loadCameraStereoParameters(const std::string file_path,
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
    static bool saveCameraStereoParameters(const std::string file_path,
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

#endif // PROJECTFILES_H
