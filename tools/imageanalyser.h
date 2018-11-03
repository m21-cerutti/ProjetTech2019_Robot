#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H

#include <QObject>
#include <QWidget>
#include <QDebug>

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

#include <time.h>

class ImageAnalyser
{

private:

    static void showMatrice(std::string name, cv::Mat mat);

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
     * @brief Make disparity map with BM algorithm
     * @param sourceMat the matrice to compute with left and right image
     * @param bmState he parameters initialised for computing
     *     PRESET
     *     nbdisparities
     *     SADWindowsSize
     * @return the disparity matrice
     */
    static cv::Mat computeBMDisparity(const cv::Mat sourceMat,  cv::StereoBM bmState);

    /**
     * @brief Make disparity map with SGBM algorithm
     * @param sourceMat the matrice to compute with left and right image
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
     * @return the disparity matrice
     */
    static cv::Mat computeSGBMDisparity(const cv::Mat sourceMat, cv::StereoSGBM sgbmState);

    /**
     * @brief Compute the approximate efficiency of a function in ms.
     * @param timeElapsed
     * @param func template for a function who return a cv::Mat and take a cv::Mat for parameter
     * @param sourceMat argument for the fonction
     * @param argstereo optionnal argument for the fonction (especially stereo functions)
     * @return the cv::Mat of the fonction
     */
    template<typename T>
    static cv::Mat computeEfficiency(double& time, T func, const cv::Mat sourceMat)
    {
        double elapsedTime;
        clock_t stopTime;
        clock_t startTime = clock();

        cv::Mat result = func(sourceMat);

        stopTime = clock();
        elapsedTime = (stopTime - startTime) / (CLOCKS_PER_SEC / (double) 1000.0);
        time = elapsedTime;

        return result;
    }
    template<typename T, typename U>
    static cv::Mat computeEfficiency(double& time, T func, const cv::Mat sourceMat, U argstereo)
    {
        double elapsedTime;
        clock_t stopTime;
        clock_t startTime = clock();

        cv::Mat result = func(sourceMat, argstereo);

        stopTime = clock();
        elapsedTime = (stopTime - startTime) / (CLOCKS_PER_SEC / (double) 1000.0);
        time = elapsedTime;

        return result;
    }

};

#endif // IMAGEANALYSER_H
