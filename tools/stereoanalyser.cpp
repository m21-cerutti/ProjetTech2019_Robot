#include "stereoanalyser.h"



void StereoAnalyser::computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat left_mat, right_mat;

    ///Separate
    ImageFilter::separateImage(src, left_mat, right_mat);

    computeBMDisparityStereo(left_mat, right_mat, out, bm_state);
}

void StereoAnalyser::computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Gray
    ImageFilter::applyGray(src_left, left_mat);
    ImageFilter::applyGray(src_right, right_mat);

    ///Disparity map
    bm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    ImageFilter::fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.copyTo(out);
}

void StereoAnalyser::computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat left_mat, right_mat;
    ImageFilter::separateImage(src, left_mat, right_mat);

    computeSGBMDisparityStereo(left_mat, right_mat, out, sgbm_state);
}

void StereoAnalyser::computeSGBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Gray
    ImageFilter::applyGray(src_left, left_mat);
    ImageFilter::applyGray(src_right, right_mat);

    ///Disparity map
    sgbm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    ImageFilter::fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.copyTo(out);
}

void StereoAnalyser::computeDepthMap(const cv::Mat &disparity, const cv::Mat &Q, cv::Mat &image_3d)
{
    cv::reprojectImageTo3D(disparity, image_3d, Q, true, CV_32F);
    cv::normalize(image_3d, image_3d, 0, 255, CV_MINMAX, CV_8UC1);
}
