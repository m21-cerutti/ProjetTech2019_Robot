#include "interfacecvqt.h"

void InterfaceCvQt::showMatrice(std::string name, const cv::Mat &mat)
{
    if(mat.empty())
    {
        std::cerr << "Matrice empty.\n";
        return;
    }
    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, mat);
}

void InterfaceCvQt::toQImage(const cv::Mat &in, QImage &out)
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

void InterfaceCvQt::toMatCV(const QImage &in, cv::Mat& out)
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
