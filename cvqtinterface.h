#ifndef INTERFACECVQT_H
#define INTERFACECVQT_H

#include <QDir>
#include <QFileInfoList>
#include <QFileDialog>
#include <QImage>
#include <cv.h>
#include <opencv2/opencv.hpp>

#include "custom_controller.h"

#define IS_COMPUTER
#define DELAY_DEBUG_VIDEO 30
#define DEFAULT_NB_FRAME_EXTRACT 30
#define DEFAULT_BEGIN_FRAME_EXTRACT 50

/**
 * @brief The CVQTInterface class gather all function of conversion and mangement of extracting images from a folder or a video.
 */
namespace CVQTInterface
{

/**
  * @brief Ask a dialog to pick a folder of stereo images and extract left-right.
  *         Need to explicity contain "left" or "right" in the file name and be a PNG file.
  * @param left_images the output container of left images.
  * @param right_images the output container of right images.
  * @return true if succes, false otherwise.
  */
bool getSetImagesStereo(std::vector<cv::Mat>& left_images, std::vector<cv::Mat>& right_images);

/**
  * @brief Save a set of right-left images
  * @param prefix_folder
  * @param left_images the input container of left images.
  * @param right_images the input container of right images.
  * @return true if succes, false otherwise.
  */
bool saveSetImagesStereo(std::string prefix_folder, const std::vector<cv::Mat> &left_images, const std::vector<cv::Mat> &right_images);


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
                           std::vector<cv::Mat>& output_left,
                           std::vector<cv::Mat> &output_right,
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

