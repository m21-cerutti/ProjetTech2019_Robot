#include "cvqtinterface.h"

void CVQTInterface::getSetImagesStereo(const QString folder_path, std::vector<cv::Mat>& left_images, std::vector<cv::Mat>& right_images)
{
    QDir dir_left(folder_path);
    QDir dir_right = QDir(dir_left);

    QStringList filter_left;
    QStringList filter_right;

    filter_left << QLatin1String("*_left.png");
    filter_right << QLatin1String("*_right.png");

    dir_left.setNameFilters(filter_left);
    dir_right.setNameFilters(filter_left);

    QFileInfoList filelistinfo_left = dir_left.entryInfoList();
    QFileInfoList filelistinfo_right = dir_right.entryInfoList();

    foreach (const QFileInfo &fileinfo, filelistinfo_left) {
        QString imageFile = fileinfo.absoluteFilePath();
        cv::Mat img_left;
        img_left = cv::imread(imageFile.toStdString());
        left_images.push_back(img_left);
    }

    foreach (const QFileInfo &fileinfo, filelistinfo_right) {
        QString imageFile = fileinfo.absoluteFilePath();
        cv::Mat img_right;
        img_right = cv::imread(imageFile.toStdString());
        right_images.push_back(img_right);
    }

}

void CVQTInterface::toQImage(const cv::Mat &in, QImage &out)
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
        cv::cvtColor(in, tmp, CV_GRAY2RGBA);
        QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
        out = dest.copy();
        break;
    }
    ///BGR case
    case CV_8UC3:
    {
        cv::Mat tmp;
        cv::cvtColor(in, tmp, CV_BGR2RGBA);
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
    case CV_32F:
    {
        cv::Mat tmp;
        in.copyTo(tmp);
        cv::normalize(tmp, tmp, 0, 255, CV_MINMAX, CV_8UC1);
        cv::cvtColor(tmp, tmp, CV_GRAY2RGBA);
        QImage dest((const uchar *) tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888_Premultiplied);
        out = dest.copy();
        break;
    }

    default:
    {
        out = QImage();
        ProjectUtilities::messageDebug("Non valid format from openCV, Must be CV_8UC1 | CV_8UC3 | CV_8UC4 | CV_32F.", true);
        break;
    }
    }
}

void CVQTInterface::toMatCV(const QImage &in, cv::Mat& out)
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
