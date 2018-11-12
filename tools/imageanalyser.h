#ifndef IMAGEANALYSER_H
#define IMAGEANALYSER_H

#include <QImage>

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

public:

    /**
     * @brief Transform into greyscale matrice if it's not one yet.
     * @param mat the matrice to transform
     * @return the gray matrice (or itself)
     */
    static cv::Mat applyGreyScaleCondition(const cv::Mat &mat);

    /**
     * @brief show the matrice, without Qt
     * @param name the name of the windows cv
     * @param mat the matrice to display
     */
    static void showMatrice(std::string name, cv::Mat& mat, int mode=0);


    ///Function pointer for efficiency calculation
    typedef cv::Mat (*filter_func) (const cv::Mat&);

    /**
    * @brief convert cv::Mat img to QImage img
    * @param in cv::Mat to convert
    * @param out QImage the output
    */
    static void toQImage(const cv::Mat& in, QImage &out);

    /**
    * @brief convert Qimage in cv::Mat img
    * @param in QImage to convert
    * @param out cv::Mat the output
    */
    static void toMatCV(const QImage& in, cv::Mat& out);

    /**
    * @brief Seperate the image in 2 equal parts: left & right)
    * @param mat the source matrice.
    * @param matL Left result
    * @param matR Right result
    */
    static void separateImage(const cv::Mat& mat, cv::Mat &matL, cv::Mat &matR);

    /**
     * @brief Apply a gaussian filter
     * @param mat the source matrice
     * @return the result
     */
    static cv::Mat computeGaussianBlur(const cv::Mat& mat);

    /**
     * @brief Apply a sobel filter
     * @param mat the source matrice
     * @return the result
     */
    static cv::Mat computeSobel(const cv::Mat& mat);

    /**
    * @brief Transform into greyscale if needed and apply the laplacian.
    * @param mat the source matrice
    * @return the result matrice
    */
    static cv::Mat computeLaplacian(const cv::Mat& mat);


    /**
     * @brief Make disparity map with BM algorithm
     * @param mat the matrice to compute with left and right image
     * @param bmState he parameters initialised for computing
     *     PRESET
     *     nbdisparities
     *     SADWindowsSize
     * @return the disparity matrice
     */
    static cv::Mat computeBMDisparity(const cv::Mat& mat, cv::Ptr<cv::StereoBM> bmState);

    /**
     * @brief Make disparity map with SGBM algorithm
     * @param mat the matrice to compute with left and right image
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
    static cv::Mat computeSGBMDisparity(const cv::Mat& mat, cv::Ptr<cv::StereoSGBM> sgbmState);

    /**
     * @brief Compute the approximate efficiency of a function in ms.
     * @param timeElapsed
     * @param func template for a function who return a cv::Mat and take a cv::Mat for parameter
     * @param mat argument for the fonction, often the source
     * @param argstereo optionnal argument for the fonction (especially stereo functions)
     * @return the resulting cv::Mat of the fonction
     */
    static cv::Mat computeEfficiency(double& time, filter_func func, const cv::Mat& mat);

    ///Surchage template
    template<typename T, typename U>
    static cv::Mat computeEfficiency(double& time, T func, const cv::Mat& mat, U argstereo)
    {
        double elapsedTime;
        clock_t stopTime;
        clock_t startTime = clock();

        cv::Mat result = func(mat, argstereo);

        stopTime = clock();
        elapsedTime = (stopTime - startTime) / (CLOCKS_PER_SEC / (double) 1000.0);
        time = elapsedTime;

        return result;
    }

};

#endif // IMAGEANALYSER_H
