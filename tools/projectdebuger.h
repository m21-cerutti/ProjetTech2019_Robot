#ifndef PROJECTDEBUGER_H
#define PROJECTDEBUGER_H


#include <cv.h>
#include <opencv2/opencv.hpp>

#include <time.h>

//Not set on robot
#define isComputer

#ifdef isComputer
#include <QDebug>
#endif //isComputer

class ProjectDebuger
{
public:

    template<typename T>
    static std::string matToString(const cv::Mat& source)
    {
        std::string print;
        for(int i=0; i<source.rows; i++)
        {
            print += "[";
            for(int j=0; j<source.cols; j++)
            {
                print += std::to_string(source.at<T>(i,j));
                if(j < source.cols-1)
                    print += ", ";
            }
            print += "]";
            print += "\n";
        }
        return print;
    }

    /**
     * @brief messageDebug
     * @param message
     */
    static void messageDebug(std::string message, bool error = true);

    /**
     * @brief show the matrice, without Qt
     * @param name the name of the windows cv
     * @param mat the matrice to display
     */
    static void showMatrice(std::string name, const cv::Mat& mat);

    /**
     * @brief Compute the approximate efficiency of a function in ms.
     * @param func template for a function who take a source cv::Mat and an output cv::Mat, at least.
     * @param src the source image matrice.
     * @param out output of the function.
     * @param argstereo optionnal argument for the fonction (especially stereo functions)
     * @return the time that have taken.
     */
    template<typename T>
    static double computeEfficiency(T func, const cv::Mat &src, cv::Mat &out)
    {
        double elapsed_time;
        clock_t start_time, stop_time;
        start_time = clock();

        func(src, out);

        stop_time = clock();
        elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

        return elapsed_time;
    }

    template<typename T, typename U>
    static double computeEfficiency(T func, const cv::Mat &src, cv::Mat &out, U argstereo)
    {
        double elapsed_time;
        clock_t start_time, stop_time;
        start_time = clock();

        func(src, out, argstereo);

        stop_time = clock();
        elapsed_time = (stop_time - start_time) / (CLOCKS_PER_SEC / (double) 1000.0);

        return elapsed_time;
    }
};

#endif // PROJECTDEBUGER_H
