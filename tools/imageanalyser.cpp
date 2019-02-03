#include "imageanalyser.h"


void ImageAnalyser::applyGray(const cv::Mat &src, cv::Mat &out)
{
    if(src.type() != CV_8UC1)
    {
        cv::Mat gray;
        cv::cvtColor(src, gray, CV_BGRA2GRAY);
        gray.copyTo(out);
    }
    else if(src.type() == CV_8UC1)
    {
        src.copyTo(out);
    }
}

void ImageAnalyser::separateImage(const cv::Mat& mat, cv::Mat &mat_left, cv::Mat &mat_right)
{
    /// Parity of the image
    int offset = 0;
    if(mat.empty())
    {
        return;
    }
    /// left img from otigin to src's middle
    mat_left = mat.colRange(0, mat.cols/2);
    /// Verify odd size
    offset = mat.cols % 2;
    mat_right = mat.colRange((mat.cols/2) + offset, mat.cols);
}

void ImageAnalyser::computeGaussianBlur(const cv::Mat &src, cv::Mat &out)
{
    double sigma_x = 0, sigma_y = 0;
    cv::Size size(3,3);
    cv::GaussianBlur( src, out, size, sigma_x, sigma_y, cv::BORDER_DEFAULT );
}

void ImageAnalyser::computeGradient(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, grad_x, grad_y;

    double scale = 1;
    double delta = 0;

    applyGray(src, gray);
    computeGaussianBlur(gray, gray);

    /// Gradient X
    Sobel( gray, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, grad_x );

    /// Gradient Y
    Sobel( gray, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, grad_y );

    /// Total Gradient (approximate)
    addWeighted( grad_x, 1, grad_y, 1, 0, gray, CV_8UC1);
    gray.convertTo(out, CV_8UC1);

}

void ImageAnalyser::computeLaplacian(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, dest;

    ///Transform to Gray and smooth
    applyGray(src, gray);
    computeGaussianBlur(gray, gray);

    /// Apply Laplace function
    cv::Laplacian( gray, dest, CV_16S, 3, 1, 1, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );
    dest.convertTo(out, CV_8UC1);
}

void ImageAnalyser::fillBlank(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat tmp(src.rows, src.cols, src.type(), cvScalar(255, 255, 255, 255));;
    tmp.copyTo(out);
}

void ImageAnalyser::computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Separate
    separateImage(src, left_mat, right_mat);

    ///Gray
    applyGray(left_mat, left_mat);
    applyGray(right_mat, right_mat);

    ///Disparity map
    bm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.copyTo(out);
}

void ImageAnalyser::computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Separate
    separateImage(src, left_mat, right_mat);

    ///Gray
    applyGray(left_mat, left_mat);
    applyGray(right_mat, right_mat);

    ///Disparity map
    sgbm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.copyTo(out);
}

