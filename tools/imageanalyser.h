#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H

#include <QObject>
#include <QWidget>

#include <QImage>
#include <QPixmap>

#include <opencv2/core/core.hpp>
#include <opencv/cvwimage.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/affine.hpp>


class ImageAnalyser
{
public:


    /**
    * @brief convert cv::Mat img to QImage img
    * @param mat cv::Mat to convert
    * @return QImage
    */
    static QImage toQImage(const cv::Mat mat);

    /**
    * @brief convert Qimage in cv::Mat img
    * @param img QImage to convert
    * @return cv::Mat
    */
    static cv::Mat toMatCV(const QImage img);

    /**
    * @brief Seperate the image in 2 equal parts: left & right)
    * @param mat the source matrice.
    * @param matL Left result
    * @param matR Right result
    */
    static void separateImage(const cv::Mat mat, cv::Mat &matL, cv::Mat &matR);

    /**
    * @brief Transform into greyscale matrice and return the laplacian image.
    * @param src the source matrice
    * @return the result matrice
    */
    static cv::Mat computeLaplacian(const cv::Mat sourceMat);

    /**
    * @brief Make simple depth map without parameters
    * @param mat the matrice to compute with left and right image
    * @return the result matrice
    */
    static cv::Mat computeDepthMap(const cv::Mat sourceMat);

    /**
    * @brief Make advanced depth map
    * @param mat the matrice to compute with left and right image
    * @return the result matrice
    */
    static cv::Mat computeAdvancedDepthMap(const cv::Mat sourceMat, cv::StereoSGBM bmState);

};

#endif // IMAGEANALYSER_H
