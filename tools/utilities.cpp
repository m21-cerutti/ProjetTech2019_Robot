#include "tools/utilities.h"

void Utilities::messageDebug(std::string message, bool error)
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

void Utilities::showMatrice(std::string name, const cv::Mat &mat)
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
#else

    using namespace cv;

    std::string folder_cmd = "mkdir -p ./DEBUG_IMAGES_CERUTTI/"+name;
    if (std::system(folder_cmd.c_str()) == 0)
    {

        Utilities::messageDebug("Create folder done.", false);
    }
    else
    {
        Utilities::messageDebug("Error create folder.", true);
    }

    Utilities::messageDebug( "Starting save image: " +name, false);

    time_t rawtime; time(&rawtime);
    std::string date =  asctime(localtime(&rawtime));
    imwrite("./DEBUG_IMAGES_CERUTTI/"+name+"_"+date, mat);

    Utilities::messageDebug("Image saved.", false);

#endif
}

