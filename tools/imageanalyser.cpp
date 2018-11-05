#include "imageanalyser.h"


void ImageAnalyser::showMatrice(std::string name, cv::Mat mat)
{
    if(mat.empty())
    {
        return;
    }

    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, mat);
}

cv::Mat ImageAnalyser::applyGreyScaleCondition(const cv::Mat mat)
{
    if(mat.type() != CV_8UC1)
    {
        return computeGrayScale(mat);
    }
    return mat;
}

void ImageAnalyser::toQImage(const cv::Mat in, QImage& out)
{
    if(in.empty())
    {
        out = QImage();
    }

    switch(in.type())
    {
    ///GreyScale case
    case CV_8UC1:
    {
        cv::Mat tmp;
        cv::cvtColor(in, tmp, CV_GRAY2BGRA);
        QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
        out = dest.copy();
        break;
    }
    default:
    {
        cv::Mat tmp;
        cv::cvtColor(in, tmp, CV_BGRA2RGBA);
        // make QImage
        QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
        out = dest.copy();
        break;
    }
    }
}

void ImageAnalyser::toMatCV(const QImage in, cv::Mat& out)
{
    cv::Mat result;
    if(!in.isNull())
    {
        QImage conv = in.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
        cv::Mat tmp(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(), conv.bytesPerLine());
        cv::cvtColor(tmp, result, cv::COLOR_RGBA2BGRA);
    }
    result.copyTo(out);
}

void ImageAnalyser::separateImage(const cv::Mat mat, cv::Mat &matL, cv::Mat &matR)
{
    /// Parity of the image
    int offset = 0;
    if(mat.empty())
    {
        return;
    }
    /// left img from otigin to src's middle
    matL = mat.colRange(0, mat.cols/2);
    /// Verify odd size
    offset = mat.cols % 2;
    matR = mat.colRange((mat.cols/2) + offset, mat.cols);
}

cv::Mat ImageAnalyser::computeGrayScale(const cv::Mat mat)
{
    cv::Mat gray;
    cv::cvtColor(mat, gray, CV_BGRA2GRAY);
    return gray;
}

cv::Mat ImageAnalyser::computeGaussianBlur(const cv::Mat mat)
{
    cv::Mat dest;
    double sigmaX = 0, sigmaY = 0;
    cv::Size size(3,3);
    cv::GaussianBlur( mat, dest, size, sigmaX, sigmaY, cv::BORDER_DEFAULT );
    return dest;
}

cv::Mat ImageAnalyser::computeSobel(const cv::Mat mat)
{
    cv::Mat gray, dest;

    ///Parameters default
    int dx = 1;
    int dy = 0;
    int ksize = 3;
    double scale = 1;
    double delta = 0;
    int borderType= cv::BORDER_DEFAULT;

    gray = applyGreyScaleCondition(mat);

    cv::Sobel(gray, dest, CV_16S, dx, dy, ksize, scale, delta, borderType);
    convertScaleAbs( dest, dest );

    return dest;
}

cv::Mat ImageAnalyser::computeLaplacian(const cv::Mat mat)
{
    cv::Mat tmp, dest;

    ///Transform to Gray and smooth
    tmp = applyGreyScaleCondition(mat);
    tmp = computeGaussianBlur(tmp);

    /// Apply Laplace function
    cv::Laplacian( tmp, dest, CV_16S, 3, 1, 0, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );

    return dest;
}

cv::Mat ImageAnalyser::computeBMDisparity(const cv::Mat mat, cv::StereoBM bmState)
{
    cv::Mat lMat, rMat, disparity;

    ///Separate
    ImageAnalyser::separateImage(mat, lMat, rMat);

    ///Gray
    lMat = applyGreyScaleCondition(lMat);
    rMat = applyGreyScaleCondition(rMat);

    ///Disparity map
    bmState(lMat, rMat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8U);

    return disparity;
}

cv::Mat ImageAnalyser::computeSGBMDisparity(const cv::Mat mat, cv::StereoSGBM sgbmState)
{

    cv::Mat lMat, rMat, disparity;

    ///Separate
    ImageAnalyser::separateImage(mat, lMat, rMat);

    ///Gray
    lMat = applyGreyScaleCondition(lMat);
    rMat = applyGreyScaleCondition(rMat);

    ///Disparity map
    sgbmState(lMat, rMat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8U);

    return disparity;
}

cv::Mat ImageAnalyser::computeEfficiency(double &time, ImageAnalyser::filter_func func, const cv::Mat mat)
{
    double elapsedTime;
    clock_t stopTime;
    clock_t startTime = clock();

    cv::Mat result = func(mat);

    stopTime = clock();
    elapsedTime = (stopTime - startTime) / (CLOCKS_PER_SEC / (double) 1000.0);
    time = elapsedTime;

    return result;

}
