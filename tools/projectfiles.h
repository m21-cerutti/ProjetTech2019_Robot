#ifndef PROJECTFILES_H
#define PROJECTFILES_H

#include <cv.h>
#include <opencv2/opencv.hpp>

#include "tools/projectutilities.h"

class ProjectFiles
{
public:


    static bool getMatrixCalibrationFileStorage(const std::string file_path,
                                                const std::string name_parameter,
                                                cv::Mat& out);

    /**
     * @brief loadCameraParemeters
     * @param file_path
     * @param width
     * @param height
     * @param camera_matrix
     * @param dist_coeffs
     * @param rvecs
     * @param tvecs
     * @return
     */
    static bool loadIntrinsicCamera(const std::string file_path,
                                    cv::Size &img_size,
                                    cv::Mat& camera_matrix,
                                    cv::Mat& dist_coeffs,
                                    std::vector<cv::Mat>& rvecs,
                                    std::vector<cv::Mat>& tvecs);

    /**
     * @brief loadIntrinsicCamera
     * @param file_path
     * @param img_size
     * @param camera_matrix
     * @param dist_coeffs
     * @return
     */
    static bool loadIntrinsicCamera(const std::string file_path,
                                    cv::Size &img_size,
                                    cv::Mat& camera_matrix,
                                    cv::Mat& dist_coeffs);

    /**
     * @brief saveCameraParameters
     * @param file_path
     * @param width
     * @param height
     * @param camera_matrix
     * @param dist_coeffs
     * @param rvecs
     * @param tvecs
     * @return
     */
    static bool saveIntrinsicCamera(const std::string file_path,
                                    const cv::Size img_size,
                                    const cv::Mat &camera_matrix,
                                    const cv::Mat &dist_coeffs,
                                    const std::vector<cv::Mat> &rvecs,
                                    const std::vector<cv::Mat> &tvecs );


    /**
     * @brief loadExtrinsicCameraStereo
     * @param file_path
     * @param camera_matrix
     * @param dist_coeffs
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
     * @brief saveExtrinsicCameraStereo
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
     * @return
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
