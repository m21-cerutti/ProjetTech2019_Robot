#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H

#include <opencv2/core/core.hpp>
#include <opencv/cvwimage.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/affine.hpp>


#include "tools/utilities.h"

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
    void separateImage(const cv::Mat &src, cv::Mat &mat_left, cv::Mat &mat_right);

    /**
     * @brief Apply a gaussian filter.
     * @param src the source matrix.
     * @param out the output matrix.
     */
    void computeGaussianBlur(const cv::Mat &src, cv::Mat &out);

    /**
     * @brief Apply a sobel filter.
     * @param src the source matrix.
     * @param out the output matrix.
     */
    void computeGradient(const cv::Mat &src, cv::Mat &out);

    /**
    * @brief Transform into greyscale if needed and apply the laplacian.
    * @param src the source matrix.
    * @param out the output matrix.
    */
    void computeLaplacian(const cv::Mat &src, cv::Mat &out);


};

#endif // IMAGEANALYSER_H
