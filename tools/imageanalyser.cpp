#include "imageanalyser.h"


//TO CHANGE
void ImageAnalyser::showMatrice(std::string name, cv::Mat& mat)
{
    if(mat.empty())
    {
        std::cerr << "Matrice empty.\n";
        return;
    }
    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, mat);
}

cv::Mat ImageAnalyser::applyGreyScaleCondition(const cv::Mat& mat)
{
    if(mat.type() != CV_8UC1 && mat.type() == (CV_8UC3 | CV_8UC4))
    {
        cv::Mat gray;
        cv::cvtColor(mat, gray, CV_BGRA2GRAY);
        return gray;
    }
    return mat;
}

void ImageAnalyser::toQImage(const cv::Mat &in, QImage& out)
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

cv::Mat ImageAnalyser::computeGaussianBlur(const cv::Mat& mat)
{
    cv::Mat dest;
    double sigma_x = 0, sigma_y = 0;
    cv::Size size(3,3);
    cv::GaussianBlur( mat, dest, size, sigma_x, sigma_y, cv::BORDER_DEFAULT );
    return dest;
}

cv::Mat ImageAnalyser::computeGradient(const cv::Mat& mat)//1D FAUT 2D, prendre x, y et max des deux
{
    cv::Mat gray, dest;

    ///Parameters default
    int dx = 1;
    int dy = 0;//A CHANGER
    int ksize = 3;
    double scale = 1;
    double delta = 0;

    gray = applyGreyScaleCondition(mat);

    cv::Sobel(gray, dest, CV_8UC1, dx, dy, ksize, scale, delta);

    return dest;
}

cv::Mat ImageAnalyser::computeLaplacian(const cv::Mat& mat)
{
    cv::Mat tmp, dest;

    ///Transform to Gray and smooth
    tmp = applyGreyScaleCondition(mat);
    tmp = computeGaussianBlur(tmp);

    /// Apply Laplace function
    cv::Laplacian( tmp, dest, CV_8UC1, 3, 1, 0, cv::BORDER_DEFAULT );

    return dest;
}

cv::Mat ImageAnalyser::computeBMDisparity(const cv::Mat& mat,  cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat left_mat, right_mat, disparity;

    ///Separate
    ImageAnalyser::separateImage(mat, left_mat, right_mat);

    ///Gray
    left_mat = ImageAnalyser::applyGreyScaleCondition(left_mat);
    right_mat = ImageAnalyser::applyGreyScaleCondition(right_mat);

    ///Disparity map
    bm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);

    return disparity;
}

cv::Mat ImageAnalyser::computeSGBMDisparity(const cv::Mat& mat, cv::Ptr<cv::StereoSGBM> sgbm_state)
{

    cv::Mat left_mat, right_mat, disparity;

    ///Separate
    ImageAnalyser::separateImage(mat, left_mat, right_mat);

    ///Gray
    left_mat = applyGreyScaleCondition(left_mat);
    right_mat = applyGreyScaleCondition(right_mat);

    ///Disparity map
    sgbm_state->compute(left_mat, right_mat, disparity);
    cv::normalize(disparity, disparity, 0, 255, CV_MINMAX, CV_8UC1);


    return disparity;
}

cv::Mat ImageAnalyser::computeEfficiency(double &time, ImageAnalyser::filter_func func, const cv::Mat& mat)
{
    double elapsed_time;
    clock_t start_time, stop_time;
    start_time = clock();

    cv::Mat result = func(mat);

    stop_time = clock();
    elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);
    time = elapsed_time;

    return result;

}
