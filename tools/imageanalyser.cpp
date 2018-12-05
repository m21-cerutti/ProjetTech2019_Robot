#include "imageanalyser.h"


//TO CHANGE
void ImageAnalyser::showMatrice(std::string name, const cv::Mat &mat)
{
    if(mat.empty())
    {
        std::cerr << "Matrice empty.\n";
        return;
    }
    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, mat);
}

void ImageAnalyser::applyGray(const cv::Mat &src, cv::Mat &out)
{
    if(src.type() != CV_8UC1 && src.type() == (CV_8UC3 | CV_8UC4))
    {
        cv::Mat gray;
        cv::cvtColor(src, gray, CV_BGRA2GRAY);
        gray.copyTo(out);
    }
}

void ImageAnalyser::toQImage(const cv::Mat &in, QImage &out)
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
        ///BGR case
    case CV_8UC3:
    {
        cv::Mat tmp;
        cv::cvtColor(in, tmp, CV_BGR2BGRA);
        QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
        out = dest.copy();
        break;
    }
        ///BGRA case
    case CV_8UC4:
    {
        cv::Mat tmp;
        cv::cvtColor(in, tmp, CV_BGRA2RGBA);
        QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
        out = dest.copy();
        break;
    }
    default:
    {
        out = QImage();
        std::cerr << "Non valid format from openCV, Must be CV_8UC1 | CV_8UC3 | CV_8UC4.\n";
        break;
    }
    }
}

//TO CHANGE
void ImageAnalyser::toMatCV(const QImage &in, cv::Mat& out)
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

void ImageAnalyser::separateImage(const cv::Mat& mat, cv::Mat &mat_left, cv::Mat &mat_right)
{
    /// Parity of the image
    int offset = 0;
    if(mat.empty())
    {
        return;
    }
    /// left img from otigin to src's middle
    mat_left = mat.colRange(0, mat.cols/2);
    /// Verify odd size
    offset = mat.cols % 2;
    mat_right = mat.colRange((mat.cols/2) + offset, mat.cols);
}

void ImageAnalyser::computeGaussianBlur(const cv::Mat &src, cv::Mat &out)
{
    double sigma_x = 0, sigma_y = 0;
    cv::Size size(3,3);
    cv::GaussianBlur( src, out, size, sigma_x, sigma_y, cv::BORDER_DEFAULT );
}

void ImageAnalyser::computeGradient(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, grad_x, grad_y;

    double scale = 1;
    double delta = 0;

    applyGray(src, gray);
    computeGaussianBlur(gray, gray);

    /// Gradient X
    Sobel( gray, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, grad_x );

    /// Gradient Y
    Sobel( gray, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, grad_y );

    /// Total Gradient (approximate)
    addWeighted( grad_x, 1, grad_y, 1, 0, gray, CV_8UC1);
    gray.convertTo(out, CV_8UC1);

}

void ImageAnalyser::computeLaplacian(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, dest;

    ///Transform to Gray and smooth
    applyGray(src, gray);
    computeGaussianBlur(gray, gray);

    /// Apply Laplace function
    cv::Laplacian( gray, dest, CV_16S, 3, 1, 1, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );
    dest.convertTo(out, CV_8UC1);
}

void ImageAnalyser::fillBlank(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat tmp(src.rows,src.cols, src.type(), cvScalar(255, 255, 255, 255));;
    tmp.copyTo(out);
}

void ImageAnalyser::computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Separate
    separateImage(src, left_mat, right_mat);

    ///Gray
    applyGray(left_mat, left_mat);
    applyGray(right_mat, right_mat);

    ///Disparity map
    bm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.copyTo(out);
}

void ImageAnalyser::computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat invert, left_mat, right_mat, disparity;

    ///Separate
    separateImage(src, left_mat, right_mat);

    ///Gray
    applyGray(left_mat, left_mat);
    applyGray(right_mat, right_mat);

    ///Disparity map
    sgbm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    fillBlank(disparity, invert);
    cv::subtract(invert, disparity, disparity);
    disparity.copyTo(out);
}

