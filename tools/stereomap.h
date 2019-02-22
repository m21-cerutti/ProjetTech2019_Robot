#ifndef STEREOANALYSIS_H
#define STEREOANALYSIS_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "tools/cameracalibration.h"
#include "tools/imagefilter.h"


#include "tools/projectutilities.h"

/**
 * @brief The StereoAnalyser class gather all stereo function analyser after calibration.
 */
class StereoMap
{

public:


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
    static void computeDepthMap(const cv::Mat& disparity, const cv::Mat& Q, cv::Mat& image_3d);

};

#endif // STEREOANALYSIS_H
