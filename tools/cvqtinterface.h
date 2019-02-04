#ifndef INTERFACECVQT_H
#define INTERFACECVQT_H

#include <QImage>
#include <cv.h>
#include <opencv2/opencv.hpp>

#include "tools/projectutilities.h"

class CVQTInterface
{
public:

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

};

#endif // INTERFACECVQT_H
