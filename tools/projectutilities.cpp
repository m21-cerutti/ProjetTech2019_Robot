#include "projectutilities.h"

void ProjectUtilities::messageDebug(std::string message, bool error)
{
#ifdef IS_COMPUTER
    if(error)
    {
        qWarning()<<"Error : " << message.c_str() << "\n";
        return;
    }
    qDebug()<<"Debug : " << message.c_str() << "\n";
#else
    if(error)
    {
        std::cerr << "Error : " << message <<std::endl;
        return;
    }
    std::cout << "Debug : " << message <<std::endl;
#endif
}

void ProjectUtilities::showMatrice(std::string name, const cv::Mat &mat)
{
    #ifdef IS_COMPUTER
    if(mat.empty())
    {
        messageDebug("Matrice empty.", true);
        return;
    }

    cv::Mat tmp;
    if(mat.depth() != CV_8U)
    {
        cv::normalize(mat, tmp, 0, 255, CV_MINMAX, CV_8UC1);
    }
    else
    {
        tmp = mat;
    }

    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, tmp);
    #endif
}

