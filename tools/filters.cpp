#include "tools/filters.h"

void Filters::separateImage(const cv::Mat& mat, cv::Mat &mat_left, cv::Mat &mat_right)
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

void Filters::computeGaussianBlur(const cv::Mat &src, cv::Mat &out)
{
    double sigma_x = 0, sigma_y = 0;
    cv::Size size(3,3);
    cv::GaussianBlur( src, out, size, sigma_x, sigma_y, cv::BORDER_DEFAULT );
}

void Filters::computeGradient(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, grad_x, grad_y;

    double scale = 1;
    double delta = 0;

    if(src.type() != CV_8UC1)
    {
        cv::cvtColor(src, gray, CV_BGRA2GRAY);
    }

    computeGaussianBlur(gray, gray);

    /// Gradient X
    Sobel( gray, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, grad_x );

    /// Gradient Y
    Sobel( gray, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, grad_y );

    /// Total Gradient
    addWeighted( grad_x, 1, grad_y, 1, 0, gray, CV_8UC1);
    gray.convertTo(out, CV_8UC1);

}

void Filters::computeLaplacian(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, dest;

    ///Transform to Gray and smooth
    if(src.type() != CV_8UC1)
    {
        cv::cvtColor(src, gray, CV_BGRA2GRAY);
    }

    computeGaussianBlur(gray, gray);

    /// Apply Laplace function
    cv::Laplacian( gray, dest, CV_16S, 3, 1, 1, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );
    dest.convertTo(out, CV_8UC1);
}



