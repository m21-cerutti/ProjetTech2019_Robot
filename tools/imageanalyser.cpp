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

QImage ImageAnalyser::toQImage(const cv::Mat mat)
{
    if(mat.empty())
    {
        return QImage();
    }

    // convert to rgb format
    cv::Mat tmp;
    cv::cvtColor(mat, tmp, CV_BGRA2RGBA);
    // make QImage
    QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
    dest.bits(); // copy to avoid memory share on same array
    return dest;
}

cv::Mat ImageAnalyser::toMatCV(const QImage img)
{
    cv::Mat result;
    if(img.isNull())
    {
        return result;
    }
    QImage conv = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
    cv::Mat tmp(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(), conv.bytesPerLine());
    cv::cvtColor(tmp, result, cv::COLOR_RGBA2BGRA);

    return result.clone();
}

void ImageAnalyser::separateImage(const cv::Mat mat, cv::Mat &matL, cv::Mat &matR)
{
    // variables
    int offset = 0;
    // assertions
    if(mat.empty())
    {
        return;
    }
    // left img from otigin to src's middle
    matL = mat.colRange(0, mat.cols/2);
    // verify odd size
    offset = mat.cols % 2;
    matR = mat.colRange((mat.cols/2) + offset, mat.cols);
}

cv::Mat ImageAnalyser::computeLaplacian(const cv::Mat sourceMat)
{
    cv::Mat gray, dest;
    ///Transform to Gray and smooth
    cv::GaussianBlur( sourceMat, sourceMat, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    cv::cvtColor( sourceMat, gray, CV_BGRA2GRAY );

    /// Apply Laplace function
    cv::Laplacian( gray, dest, CV_16S, 3, 1, 0, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );

    ///Conversion gor QImage
    cv::cvtColor( dest, dest, CV_GRAY2BGRA );
    return dest;
}

cv::Mat ImageAnalyser::computeBMDisparity(const cv::Mat sourceMat, cv::StereoBM bmState)
{
    cv::Mat lMat, rMat, disparity;

    // get img and separate in two parts
    ImageAnalyser::separateImage(sourceMat, lMat, rMat);
    // get grey img
    cv::cvtColor(lMat, lMat, CV_BGRA2GRAY);
    cv::cvtColor(rMat, rMat, CV_BGRA2GRAY);

    // get disparity map
    bmState(lMat, rMat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8U);

    // convert
    cv::cvtColor(disparity, disparity, CV_GRAY2BGRA);
    return disparity;
}

cv::Mat ImageAnalyser::computeSGBMDisparity(const cv::Mat sourceMat, cv::StereoSGBM sgbmState)
{

    cv::Mat lMat, rMat, disparity;

    // get img and separate in two parts
    ImageAnalyser::separateImage(sourceMat, lMat, rMat);
    // get grey img
    cv::cvtColor(lMat, lMat, CV_BGRA2GRAY);
    cv::cvtColor(rMat, rMat, CV_BGRA2GRAY);
    // get disparity map
    sgbmState(lMat, rMat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8U);
    cv::cvtColor(disparity, disparity, CV_GRAY2BGRA);

    return disparity;
}
