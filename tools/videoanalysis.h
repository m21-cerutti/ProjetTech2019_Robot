#ifndef VIDEOANALYSIS_H
#define VIDEOANALYSIS_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "tools/cameracalibration.h"
#include "tools/imageanalyser.h"

//Debug
#include "tools/projectdebuger.h"

class VideoAnalysis
{
public:

    template<typename T>
    static void filterVideo(std::string path_video, T func)
    {
        using namespace cv;
        VideoCapture cap(path_video);
        if ( !cap.isOpened() )
        {
            ProjectDebuger::messageDebug("Cannot open the video file.", true);
            return;
        }

        double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
        ProjectDebuger::messageDebug("Start video "+ path_video, false);
        ProjectDebuger::messageDebug("FPS : "+std::to_string(fps), false);
        namedWindow("Video_reader", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
        while(true)
        {
            Mat frame;
            if (!cap.read(frame))
            {
                std::cout<<"\n Cannot read the video file. \n";
                break;
            }
            func(frame, frame);
            imshow("Video_reader", frame);
            if(waitKey(30) == 27) // Wait for 'esc' key press to exit
            {
                break;
            }
        }
        return;
    }

    static void videoChessDebug(std::string path_video);

    static void stereoVideo(std::string path_video_left, std::string path_video_right);


};

#endif // VIDEOANALYSIS_H
