#include "projectdebuger.h"

void ProjectDebuger::messageDebug(std::string message, bool error)
{
#ifdef isComputer
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

void ProjectDebuger::showMatrice(std::string name, const cv::Mat &mat)
{
    #ifdef isComputer
    if(mat.empty())
    {
        messageDebug("Matrice empty.", true);
        return;
    }
    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, mat);
    #endif
}

