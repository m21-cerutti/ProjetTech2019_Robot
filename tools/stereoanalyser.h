#ifndef STEREOANALYSIS_H
#define STEREOANALYSIS_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "tools/cameracalibration.h"
#include "tools/imagefilter.h"


#include "tools/projectutilities.h"

class StereoAnalyser
{

public:


    /**
     * @brief Make disparity map with BM algorithm
     * @param src the matrice to compute with left and right image
     * @param out the output matrice.
     * @param bmState he parameters initialised for computing
     *     PRESET
     *     nbdisparities
     *     SADWindowsSize
     */
    static void computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state);

    /**
     * @brief computeBMDisparityStereo
     * @param src_left
     * @param src_right
     * @param out
     * @param bm_state
     */
    static void computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state);


    /**
     * @brief Make disparity map with SGBM algorithm
     * @param src the matrice to compute with left and right image
     * @param out the output matrice.
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
     * @brief computeSGBMDisparityStereo
     * @param src_left
     * @param src_right
     * @param out
     * @param sgbm_state
     */
    static void computeSGBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state);

    /**
     * @brief depthMap
     * @param disparity
     * @param Q
     * @param image_3d
     */
    static void depthMap(const cv::Mat& disparity, const cv::Mat& Q, cv::Mat& image_3d);

};

#endif // STEREOANALYSIS_H
