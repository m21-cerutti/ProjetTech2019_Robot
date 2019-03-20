#ifndef PROJECTFILES_H
#define PROJECTFILES_H

#include <cv.h>
#include <opencv2/opencv.hpp>

#include "tools/utilities.h"

/**
 * @brief The ProjectFiles class gather all functions about saves ahnd load of files.
 */
namespace Files
{
    /**
     * @brief Get a particular matrix parameter in a file. Need to be known.
     * @param file_path the path of the file.
     * @param name_parameter the name of the desired parameter.
     * @param out the matrix needed.
     * @return true if suceeded, false otherwise.
     */
    bool getMatrixCalibrationFileStorage(const std::string file_path,
                                                const std::string name_parameter,
                                                cv::Mat& out);

    /**
     * @brief saveSetImages
     * @param prefix
     * @param images_left
     * @param images_right
     * @return
     */
    bool saveSetImages(const std::string prefix, const std::vector<cv::Mat> images_left, const std::vector<cv::Mat> images_right);

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
    bool loadIntrinsicCamera(const std::string file_path,
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
    bool loadIntrinsicCamera(const std::string file_path,
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
    bool saveIntrinsicCamera(const std::string file_path,
                                    const cv::Size img_size,
                                    const cv::Mat &camera_matrix,
                                    const cv::Mat &dist_coeffs,
                                    const std::vector<cv::Mat> &rvecs,
                                    const std::vector<cv::Mat> &tvecs );




#endif // PROJECTFILES_H
