#ifndef INTERFACECVQT_H
#define INTERFACECVQT_H

#include <QDir>
#include <QFileInfoList>
#include <QImage>
#include <cv.h>
#include <opencv2/opencv.hpp>

#include "tools/projectutilities.h"

/**
 * @brief The CVQTInterface class gather all function of conversion.
 */
class CVQTInterface
{
public:

    static void getSetImagesStereo(const QString folder_path, std::vector<cv::Mat>& left_images, std::vector<cv::Mat>& right_images);


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

