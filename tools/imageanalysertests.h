#ifndef IMAGEANALYSERTESTS_H
#define IMAGEANALYSERTESTS_H

#include <QDebug>

#include "tools/imageanalyser.h"


class ImageAnalyserTests
{
public:
    ImageAnalyserTests();
    ~ImageAnalyserTests();

    int startTests();

private:

    int test_showMatrice();

    int test_applyGreyScaleCondition();

    int test_toQImage();

    int test_toMatCV();

    int test_separateImage();

    int test_computeGreyScale();

    int test_computeGaussianBlur();

    int test_computeSobel();

    int test_computeLaplacian();

    int test_computeBMDisparity();

    int test_computeSGBMDisparity();

    int test_computeEfficiency();

private:

    std::string prefix_images= "./STEREO/";
    std::string images_tests_dir = "./STEREO/images_tests.txt";

    std::vector<cv::Mat> images;

};

#endif // IMAGEANALYSERTESTS_H
