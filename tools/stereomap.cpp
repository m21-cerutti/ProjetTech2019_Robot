#include "stereomap.h"



void StereoMap::computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat left_mat, right_mat;

    ///Separate
    ImageFilter::separateImage(src, left_mat, right_mat);

    computeBMDisparityStereo(left_mat, right_mat, out, bm_state);
}

void StereoMap::computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Gray
    ImageFilter::applyGray(src_left, left_mat);
    ImageFilter::applyGray(src_right, right_mat);

    ///Disparity map
    bm_state->compute(left_mat, right_mat, disparity);
    ImageFilter::fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);

    disparity.convertTo(disparity, CV_32F);


    disparity.copyTo(out);
}

void StereoMap::computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat left_mat, right_mat;
    ImageFilter::separateImage(src, left_mat, right_mat);

    computeSGBMDisparityStereo(left_mat, right_mat, out, sgbm_state);
}

void StereoMap::computeSGBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Gray
    ImageFilter::applyGray(src_left, left_mat);
    ImageFilter::applyGray(src_right, right_mat);

    ///Disparity map
    sgbm_state->compute(left_mat, right_mat, disparity);
    ImageFilter::fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.convertTo(disparity, CV_32F);


    disparity.copyTo(out);
}

void StereoMap::computeDepthMap(const cv::Mat &disparity, const cv::Mat &Q, cv::Mat &image_3d)
{
    cv::Mat disp, q_32;
    Q.convertTo(q_32, CV_32FC1);
    disparity.convertTo(disp, CV_32FC1, 1 / 16.);
    cv::reprojectImageTo3D(disp, image_3d, q_32, true, CV_32FC1);

    //cv::normalize(image_3d, image_3d, 0, 255, CV_MINMAX, CV_8UC1);
    //cv::threshold( image_3d, image_3d, 0, 1, CV_THRESH_TRUNC );


}
