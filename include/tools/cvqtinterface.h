#ifndef INTERFACECVQT_H
#define INTERFACECVQT_H

#include <QDir>
#include <QFileInfoList>
#include <QImage>
#include <cv.h>
#include <opencv2/opencv.hpp>

#include "tools/utilities.h"

/**
 * @brief The CVQTInterface class gather all function of conversion.
 */
namespace CVQTInterface
{

    void getSetImagesStereo(const QString folder_path, std::vector<cv::Mat>& left_images, std::vector<cv::Mat>& right_images);


    /**
     * @brief Do the extraction of a stereo video.
     * @param path_video_left the path of the left video.
     * @param path_video_right the path of the right video.
     * @param start_frame the starting frame to begin extraction.
     * @param nb_frames the number of frames to extract. -1 is unlimited.
     * @param output_left Output left vector of images.
     * @param output_right Output right vector of images.
     * @param choose Boolean for manual selection.
     */
    void stereoVideoExtraction(std::string path_video_left,
                                          std::string path_video_right,
                                          int start_frame, int nb_frames,
                                          std::vector<cv::Mat>& output_left, std::vector<cv::Mat> &output_right,
                                          bool choose = false);


    /**
    * @brief convert cv::Mat img to QImage img
    * @param in cv::Mat to convert
    * @param out QImage the output
    */
    void toQImage(const cv::Mat& in, QImage &out);

    /**
    * @brief convert Qimage in cv::Mat img
    * @param in QImage to convert
    * @param out cv::Mat the output
    */
    void toMatCV(const QImage& in, cv::Mat &out);

};

#endif // INTERFACECVQT_H

