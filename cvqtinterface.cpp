#include "cvqtinterface.h"

using namespace cerutti;

void CVQTInterface::getSetImagesStereo(const QString folder_path, std::vector<cv::Mat>& left_images, std::vector<cv::Mat>& right_images)
{
    QDir dir_left(folder_path);
    QDir dir_right = QDir(dir_left);

    QStringList filter_left;
    QStringList filter_right;

    filter_left << QLatin1String("*_left.png");
    filter_right << QLatin1String("*_right.png");

    dir_left.setNameFilters(filter_left);
    dir_right.setNameFilters(filter_right);

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

void CVQTInterface::stereoVideoExtraction(std::string path_video_left, std::string path_video_right,
                                          int start_frame,
                                          int nb_frames,
                                          std::vector<cv::Mat> &output_left,
                                          std::vector<cv::Mat> &output_right,
                                          bool choose)
{
    using namespace cv;

    Utilities::messageDebug("Begin of capture's extraction.", false);

    VideoCapture cap_left(path_video_left);
    VideoCapture cap_right(path_video_right);

    if ( !cap_left.isOpened() || ! cap_right.isOpened())
    {
        Utilities::messageDebug("Cannot open the videos files.", true);
        return;
    }

    namedWindow("Video_reader_L", CV_WINDOW_AUTOSIZE);
    namedWindow("Video_reader_R", CV_WINDOW_AUTOSIZE);

    int index_frame = 0;
    int accepted_frames = 0;
    while(nb_frames == -1 || accepted_frames < nb_frames)
    {
        Mat frame_l, frame_r;

        index_frame++;
        if (!cap_left.read(frame_l) || !cap_right.read(frame_r))
        {
            Utilities::messageDebug("Cannot read the videos files.", true);
            break;
        }

        if(index_frame > start_frame)
        {
            if(choose)
            {
                while(true)
                {

                    imshow("Video_reader_L", frame_l);
                    imshow("Video_reader_R", frame_r);

                    if(waitKey(DELAY_DEBUG_VIDEO) == 32) // Wait for 'spacebar' key press to accept
                    {
                        accepted_frames++;
                        output_left.push_back(frame_l);
                        output_right.push_back(frame_r);
                        Utilities::messageDebug("Frame "+std::to_string(accepted_frames)+" accepted. Index "+std::to_string(index_frame)+".", false);
                        break;
                    }
                    else if(waitKey(DELAY_DEBUG_VIDEO) == 27) // Wait for 'esc' key press to exit
                    {
                        Utilities::messageDebug("Frame rejected : index "+std::to_string(index_frame)+".", false);
                        break;
                    }
                }
            }
            else
            {
                output_left.push_back(frame_l);
                output_right.push_back(frame_r);
                accepted_frames++;
            }
        }
    }

    cvDestroyWindow( "Video_reader_L" );
    cvDestroyWindow( "Video_reader_R" );
    Utilities::messageDebug("End of capture.", false);
    return;
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
        Utilities::messageDebug("Non valid format from openCV, Must be CV_8UC1 | CV_8UC3 | CV_8UC4 | CV_32F.", true);
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

