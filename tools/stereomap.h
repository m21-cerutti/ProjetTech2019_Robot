#ifndef STEREOANALYSIS_H
#define STEREOANALYSIS_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/disparity_filter.hpp>

#include "tools/calibration.h"
#include "tools/filters.h"


#include "tools/utilities.h"


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
    void computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state);

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
    void computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state);


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
    void computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state);

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
    void computeSGBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state);

    /**
     * @brief Make the depth map.
     * @param disparity Input the disparity map use.
     * @param Q Input 4×4 disparity-to-depth mapping matrix.
     * @param image_3d the Output matrix.
     */
    void computeDepthMap(const cv::Mat &disparity, const cv::Mat &Q, cv::Mat &depth_map, float depth_min, float depth_max);

};

#endif // STEREOANALYSIS_H
