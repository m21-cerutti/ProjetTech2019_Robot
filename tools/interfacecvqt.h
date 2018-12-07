#ifndef INTERFACECVQT_H
#define INTERFACECVQT_H

#include <QImage>
#include <cv.h>
#include <opencv2/opencv.hpp>

class InterfaceCvQt
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

#endif // INTERFACECVQT_H
