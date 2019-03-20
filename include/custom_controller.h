#pragma once

#include "controller.h"

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

namespace cerutti
{

using namespace cv;

#define NAME_FILE_CONFIG "robot.xml"
#define START_DISTANCE 200
#define EPSILON_START 40
#define PERCENT_PRESENCE 0.25
#define DISTANCE_REFRESH (EPSILON_START/4)

#define CUBE_START_X1 0.25
#define CUBE_START_Y1 0
#define CUBE_START_X2 0.75
#define CUBE_START_Y2 0.5

#define NB_FRAME_INIT 10

#define MOVE_Y_SPEED_DIV 2

class CustomController : public Controller
{
public:

    CustomController();

    virtual ~CustomController();

    virtual void process(const Mat & left_img,
                         const Mat & right_img,
                         float * vx, float * vy, float * omega) override;

    virtual void load() override;


private:

    int nb_frame = 0;

    int size_width, size_height;

    Mat Q, R1, P1, R2, P2,
    camera_matrix_l, dist_coeffs_l,
    camera_matrix_r, dist_coeffs_r;

    Ptr<StereoBM> bm_state;
    Ptr<SimpleBlobDetector> blob_detector;

};

///////////////////////////////////////////


namespace Utilities
{

/**
     * @brief Convert a matrix to a formatted string.
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
     * @brief show the matrix.
     * @param name the name of the windows cv to refresh or create.
     * @param mat the matrix to display
     */
void showMatrice(std::string name, const Mat& mat);

/**
     * @brief Compute the approximate efficiency of a function in ms.
     * @param func template for a function who take a source Mat and an output Mat, at least.
     * @param src the source image matrix.
     * @param out output of the function.
     * @param argstereo optionnal argument for the fonction (especially stereo functions)
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

};

///////////////////////////////////////////

/**
 * @brief The ImageFilter gather all functions about simple filters.
 */
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


/**
 * @brief The StereoAnalyser class gather all stereo function analyser after calibration.
 */
namespace StereoMap
{

/**
     * @brief Make disparity map with BM algorithm
     * @param src the matrix to compute with left and right image
     * @param out the output matrix.
     * @param bmState he parameters initialised for computing
     *     PRESET
     *     nbdisparities
     *     SADWindowsSize
     */
void computeBMDisparity(const Mat &src, Mat &out, Ptr<StereoBM> bm_state);

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
void computeBMDisparityStereo(const Mat &src_left, const Mat &src_right, Mat &out, Ptr<StereoBM> bm_state);


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
void computeSGBMDisparity(const Mat &src, Mat &out, Ptr<StereoSGBM> sgbm_state);

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
void computeSGBMDisparityStereo(const Mat &src_left, const Mat &src_right, Mat &out, Ptr<StereoSGBM> sgbm_state);

/**
     * @brief Make the depth map.
     * @param disparity Input the disparity map use.
     * @param Q Input 4×4 disparity-to-depth mapping matrix.
     * @param image_3d the Output matrix.
     */
void computeDepthMap(const Mat& disparity, const Mat& Q, Mat& depth_map);

};


}

