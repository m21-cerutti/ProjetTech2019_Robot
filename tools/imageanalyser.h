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

private:

    /**
     * @brief Transform into greyscale matrice if it's not one yet.
     * @param mat the matrice to transform
     * @return the gray matrice (or itself)
     */
    static cv::Mat applyGreyScaleCondition(const cv::Mat &mat);

public:

    ///Function pointer for efficiency calculation
    typedef void (*filter_func) (const cv::Mat&, cv::Mat);

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
    static void toMatCV(const QImage& in, cv::Mat& out);

    /**
    * @brief Seperate the image in 2 equal parts: left & right)
    * @param mat the source matrice.
    * @param matL Left result
    * @param matR Right result
    */
    static void separateImage(const cv::Mat& mat, cv::Mat &mat_left, cv::Mat &mat_right);

    /**
     * @brief Apply a gaussian filter
     * @param mat the source matrice
     * @return the result
     */
    static void computeGaussianBlur(const cv::Mat& mat, cv::Mat& out);

    /**
     * @brief Apply a sobel filter
     * @param mat the source matrice
     * @return the result
     */
    static void computeGradient(const cv::Mat& mat);

    /**
    * @brief Transform into greyscale if needed and apply the laplacian.
    * @param mat the source matrice
    * @return the result matrice
    */
    static void computeLaplacian(const cv::Mat& mat);

    /**
     * @brief Fill an image with blank. THe source determine the type
     * Use for disparity map.
     * @param mat
     * @param out the result matrice.
     */
    static void fillBlank(const cv::Mat& src, cv::Mat &out);


    /**
     * @brief Make disparity map with BM algorithm
     * @param mat the matrice to compute with left and right image
     * @param bmState he parameters initialised for computing
     *     PRESET
     *     nbdisparities
     *     SADWindowsSize
     * @return the disparity matrice
     */
    static void computeBMDisparity(const cv::Mat& mat, cv::Ptr<cv::StereoBM> bm_state);

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
    static void computeSGBMDisparity(const cv::Mat& mat, cv::Ptr<cv::StereoSGBM> sgbm_state);




    /*
    template<typename T, typename U>
    static cv::Mat computeFilterSteroFunc(T func, const cv::Mat& mat, U argstereo)
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
        double elapsed_time;
        clock_t start_time, stop_time;
        start_time = clock();

        cv::Mat result = func(mat, argstereo);

        stop_time = clock();
        elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);
        time = elapsed_time;

        return result;
    }

};

#endif // IMAGEANALYSER_H
