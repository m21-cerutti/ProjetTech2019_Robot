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


#include "tools/projectutilities.h"

/**
 * @brief The ImageFilter class
 */
class ImageFilter
{

public:

    /**
     * @brief Transform into greyscale matrix if it's not one yet.
     * @param src the matrix to transform.
     * @param out the output matrix.
     */
    static void applyGray(const cv::Mat &src, cv::Mat &out);

    /**
    * @brief Seperate the image in 2 equal parts (left & right).
    * @param src the source matrix.
    * @param matL Left result.
    * @param matR Right result.
    */
    static void separateImage(const cv::Mat &src, cv::Mat &mat_left, cv::Mat &mat_right);

    /**
     * @brief Apply a gaussian filter.
     * @param src the source matrix.
     * @param out the output matrix.
     */
    static void computeGaussianBlur(const cv::Mat &src, cv::Mat &out);

    /**
     * @brief Apply a sobel filter.
     * @param src the source matrix.
     * @param out the output matrix.
     */
    static void computeGradient(const cv::Mat &src, cv::Mat &out);

    /**
    * @brief Transform into greyscale if needed and apply the laplacian.
    * @param src the source matrix.
    * @param out the output matrix.
    */
    static void computeLaplacian(const cv::Mat &src, cv::Mat &out);

    /**
     * @brief Fill an image with blank. THe source determine the type
     * Use for disparity map.
     * @param src the source matrix.
     * @param out the output matrix.
     */
    static void fillBlank(const cv::Mat& src, cv::Mat &out);




};

#endif // IMAGEANALYSER_H
