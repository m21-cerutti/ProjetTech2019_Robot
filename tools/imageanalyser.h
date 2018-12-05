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

#include  <opencv2/ximgproc/disparity_filter.hpp>

#include <time.h>

class ImageAnalyser
{

public:

    /**
     * @brief show the matrice, without Qt
     * @param name the name of the windows cv
     * @param mat the matrice to display
     */
    static void showMatrice(std::string name, const cv::Mat& mat);

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
    static void toMatCV(const QImage& in, cv::Mat &out);

    /**
     * @brief Transform into greyscale matrice if it's not one yet.
     * @param src the matrice to transform
     * @param out the output matrice
     */
    static void applyGray(const cv::Mat &src, cv::Mat &out);

    /**
    * @brief Seperate the image in 2 equal parts: left & right)
    * @param src the source matrice.
    * @param matL Left result
    * @param matR Right result
    */
    static void separateImage(const cv::Mat &src, cv::Mat &mat_left, cv::Mat &mat_right);

    /**
     * @brief Apply a gaussian filter
     * @param src the source matrice
     * @param out the output matrice
     */
    static void computeGaussianBlur(const cv::Mat &src, cv::Mat &out);

    /**
     * @brief Apply a sobel filter
     * @param src the source matrice
     * @param out the output matrice
     */
    static void computeGradient(const cv::Mat &src, cv::Mat &out);

    /**
    * @brief Transform into greyscale if needed and apply the laplacian.
    * @param src the source matrice
    * @param out the output matrice
    */
    static void computeLaplacian(const cv::Mat &src, cv::Mat &out);

    /**
     * @brief Fill an image with blank. THe source determine the type
     * Use for disparity map.
     * @param src the source matrice
     * @param out the output matrice.
     */
    static void fillBlank(const cv::Mat& src, cv::Mat &out);


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

    /*
    template<typename T, typename U>
    static cv::Mat computeFilterSteroFunc(T func, const cv::Mat &src, U argstereo)
    {
        cv::Mat result;

        cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;
        double matching_time;
        double filtering_time;
        double scale = 1.0;

        cv::Mat result = func(mat, argstereo);

        wls_filter->setLambda(lambda);
        wls_filter->setSigmaColor(sigma);

        filtering_time = (double)getTickCount();
        wls_filter->filter( result, left_view, filtered_disparity_map);
        filtering_time = ((double)getTickCount() - filtering_time)/getTickFrequency();

        getDisparityVis(result, result, scale);

        return result;
    }
*/

    /**
     * @brief Compute the approximate efficiency of a function in ms.
     * @param func template for a function who take a source cv::Mat and an output cv::Mat, at least.
     * @param src the source image matrice.
     * @param out output of the function.
     * @param argstereo optionnal argument for the fonction (especially stereo functions)
     * @return the time that have taken.
     */
    template<typename T>
    static double computeEfficiency(T func, const cv::Mat &src, cv::Mat &out)
    {
        double elapsed_time;
        clock_t start_time, stop_time;
        start_time = clock();

        func(src, out);

        stop_time = clock();
        elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

        return elapsed_time;
    }

    template<typename T, typename U>
    static double computeEfficiency(T func, const cv::Mat &src, cv::Mat &out, U argstereo)
    {
        double elapsed_time;
        clock_t start_time, stop_time;
        start_time = clock();

        func(src, out, argstereo);

        stop_time = clock();
        elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

        return elapsed_time;
    }

};

#endif // IMAGEANALYSER_H
