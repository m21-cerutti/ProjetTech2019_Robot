#pragma once

#include "controller.h"

#ifdef IS_COMPUTER
#include <QDebug>
#endif

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

namespace cerutti
{

using namespace cv;


///////////////////////////////////////////
///@brief Utilities
///        gather all functions about debugging.
///
namespace Utilities
{

#define DEBUG_FOLDER "DEBUG_IMAGES_CERUTTI"

/**
  * @brief Convert a matrix to a formatted string.
  *         You must specify the interpretation
  *         like matToString<float> for example.
  * @param source the matrix to convert.
  * @return the result string.
  */
template<typename T>
std::string matToString(const Mat& source)
{
    std::string print;
    for(int i=0; i<source.rows; i++)
    {
        print += "[";
        for(int j=0; j<source.cols; j++)
        {
            print += std::to_string(source.at<T>(i,j));
            if(j < source.cols-1)
                print += ", ";
        }
        print += "]";
        print += "\n";
    }
    return print;
}

/**
  * @brief Personnal system of debugging.
  * @param message the message to display.
  */
void messageDebug(std::string message, bool error = true);

/**
  * @brief show the matrix, if not a computer, save as an image to DEBUG_FOLDER.
  * @param name the name of the windows cv or name of the file.
  * @param mat the matrix to display or save.
  */
void showMatrice(std::string name, const Mat& mat);

/**
  * @brief Compute the approximate efficiency of a function in ms.
  * @param func template for a function who take a source Mat and an output Mat, at least.
  * @param src* the sources images matrix.
  * @param out output of the function.
  * @param arg optionnal argument for the fonction (especially stereo functions)
  * @return the time that have taken.
  */
template<typename T>
double computeEfficiency(T func, const Mat &src, Mat &out)
{
    double elapsed_time;
    clock_t start_time, stop_time;
    start_time = clock();

    func(src, out);

    stop_time = clock();
    elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

    return elapsed_time;
}

template<typename T, typename U>
double computeEfficiency(T func, const Mat &src, Mat &out, U arg)
{
    double elapsed_time;
    clock_t start_time, stop_time;
    start_time = clock();

    func(src, out, arg);

    stop_time = clock();
    elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

    return elapsed_time;
}


template<typename T, typename U>
double computeEfficiency(T func, const Mat &src_left, const Mat &src_right, Mat &out, U arg)
{
    double elapsed_time;
    clock_t start_time, stop_time;
    start_time = clock();

    func(src_left, src_right, out, arg);

    stop_time = clock();
    elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

    return elapsed_time;
}

};

///////////////////////////////////////////
///@brief Filters
///        gather all functions about simple filters.
///
namespace Filters
{

/**
  * @brief Seperate the image in 2 equal parts (left & right).
  * @param src the source matrix.
  * @param matL Left result.
  * @param matR Right result.
  */
void separateImage(const Mat &src, Mat &mat_left, Mat &mat_right);

/**
  * @brief Apply a gaussian filter.
  * @param src the source matrix.
  * @param out the output matrix.
  */
void computeGaussianBlur(const Mat &src, Mat &out);

/**
  * @brief Apply a sobel filter.
  * @param src the source matrix.
  * @param out the output matrix.
  */
void computeGradient(const Mat &src, Mat &out);

/**
  * @brief Transform into greyscale if needed and apply the laplacian.
  * @param src the source matrix.
  * @param out the output matrix.
  */
void computeLaplacian(const Mat &src, Mat &out);


};

///////////////////////////////////////////
///@brief Calibration section
///         gather all stereo function about disparity and depth map.
///
namespace StereoMap
{

#define DEFAULT_BM_FILE "bmdisparity.xml"
#define DEFAULT_SGBM_FILE "sgbmdisparity.xml"


#define LAMBDA 8000
#define SIGMA 0.8

///Default parameters in m for depth
#define THRESHOLD_MIN 0.0f
//120
#define THRESHOLD_MAX 200.0f

/**
 * @brief save the BM parameters in a file. Permit to be reuse later.
 * @param file_path the path to the file.
 * @param bm_state the disparity parameters to save.
 * @return true if sucess, false otherwise.
 */
bool saveBMParameters(Ptr<StereoBM> &bm_state, std::string file_path = DEFAULT_BM_FILE);

/**
 * @brief save the SGBM parameters in a file. Permit to be reuse later.
 * @param file_path the path to the file.
 * @param sgbm_state the disparity parameters to save.
 * @return true if sucess, false otherwise.
 */
bool saveSGBMParameters( Ptr<StereoSGBM> &sgbm_state, std::string file_path = DEFAULT_SGBM_FILE);

/**
 * @brief lod the BM parameters from a file.
 * @param file_path the path to the file.
 * @param bm_state the disparity parameters to create from file.
 * @return true if sucess, false otherwise.
 */
bool loadBMParameters( Ptr<StereoBM> &bm_state, std::string file_path = DEFAULT_BM_FILE);

/**
 * @brief lod the SGBM parameters from a file.
 * @param file_path the path to the file.
 * @param sgbm_state the disparity parameters to create from file.
 * @return true if sucess, false otherwise.
 */
bool loadSGBMParameters( Ptr<StereoSGBM> &sgbm_state, std::string file_path = DEFAULT_SGBM_FILE);

/**
     * @brief Make disparity map with BM algorithm.
     * @param src_left the source image left.
     * @param src_right the source image right.
     * @param out the output matrix.
     * @param bmState he parameters initialised for computing
     *     preFilterCap
     *     PreFilterSize
     *     PreFilterType
     *     ROI1
     *     ROI2
     *     SmallerBlockSize
     *     TextureThreshold
     *     uniquenessRatio
     *     blockSize
     *     disp12MaxDiff
     *     minDisparity
     *     numDisparities
     *     speckleRange
     *     speckleWindowSize
     */
void computeBMDisparity (const Mat &src_left, const Mat &src_right, Mat &out, Ptr<StereoBM> bm_state);

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
void computeSGBMDisparity (const Mat &src_left, const Mat &src_right, Mat &out, Ptr<StereoSGBM> sgbm_state);


/**
 * @brief Make the depth map.
 * @param disparity  the disparity map used.
 * @param Q 4×4 disparity-to-depth mapping matrix.
 * @param depth_map the output matrix.
 * @param depth_min minimum distance to the camera.
 * @param depth_max maximum distance to the camera.
 */
void computeDepthMap(const cv::Mat &disparity, Mat &Q, cv::Mat &depth_map, float depth_min = THRESHOLD_MIN, float depth_max = THRESHOLD_MAX);

};

///////////////////////////////////////////
///@brief Calibration section
///         gather all functions and class about calibration.
///
namespace Calibration
{

//in cm
//Board chess
#define SQUARE_SIZE 2
#define CHESS_WIDTH 9
#define CHESS_HEIGHT 6

#define DEFAULT_CALIB_FILE "calib_stereo.xml"

/**
 * @brief The StereoCamera class permit to handle the calibration, save and load of stereo cameras.
 */
class StereoCamera
{
public:

    /**
     * @param file_path the path to save and load.
     */
    StereoCamera(std::string file_path = DEFAULT_CALIB_FILE);

    /**
     * @brief calibrate the stereo camera.
     * @param sources_images_left
     * @param sources_images_right
     */
    void calibrate(std::vector<cv::Mat>& sources_images_left,
                   std::vector<cv::Mat> &sources_images_right);

    bool save();
    bool load();

    /**
     * @brief load from a different path.
     * @param file_path the new path.
     */
    bool load(std::string file_path);

public:
    std::string file_path;

    ///Camera's calibration datas
    cv::Mat camera_matrix_l;
    cv::Mat dist_coeffs_l;
    cv::Mat camera_matrix_r;
    cv::Mat dist_coeffs_r;
    cv::Size img_size;
    cv::Mat R;
    cv::Mat F;
    cv::Mat E;
    cv::Vec3d T;
    cv::Mat R1;
    cv::Mat R2;
    cv::Mat P1;
    cv::Mat P2;
    cv::Mat Q;
};

}

///////////////////////////////////////////
///@brief Controller section
///

#define MOVE_SPEED_MULT 0.2

class CustomController : public Controller
{
public:

    CustomController();

    ~CustomController() override;

    virtual void process(const Mat & left_img,
                         const Mat & right_img,
                         float * vx, float * vy, float * omega) override;

    virtual void load() override;


protected:

    int nb_frame = 0;

    ///Contain all matrix and datas of calibration
    Calibration::StereoCamera calib;

    Ptr<StereoSGBM> sgbm_state;

};

}

