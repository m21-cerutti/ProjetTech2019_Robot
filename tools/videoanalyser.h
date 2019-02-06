#ifndef VIDEOANALYSIS_H
#define VIDEOANALYSIS_H

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "tools/cameracalibration.h"
#include "tools/imagefilter.h"


#include "tools/projectutilities.h"

#define DELAY_DEBUG_VIDEO 1

/**
 * @brief The VideoAnalyser class gather all  funtions dealing with video.
 */
class VideoAnalyser
{
public:

    /**
     * @brief Template function that apply filters on video.
     * @param path_video the path of the video.
     * @param func the filter used.
     */
    template<typename T>
    static void filterVideo(std::string path_video, T func)
    {
        using namespace cv;
        VideoCapture cap(path_video);
        if ( !cap.isOpened() )
        {
            ProjectUtilities::messageDebug("Cannot open the video file.", true);
            return;
        }

        double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
        ProjectUtilities::messageDebug("Start video "+ path_video, false);
        ProjectUtilities::messageDebug("FPS : "+std::to_string(fps), false);
        namedWindow("Video_reader", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
        while(true)
        {
            Mat frame;
            if (!cap.read(frame))
            {
                std::cout<<"\n Cannot read the video file. \n";
                ProjectUtilities::messageDebug("Cannot read the video file.", true);
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

    /**
     * @brief Debug in realtime the chessboard detection.
     * @param path_video the path of the video.
     */
    static void videoChessDebug(std::string path_video);

    /**
     * @brief Do the extraction of a stereo video.
     * @param path_video_left the path of the left video.
     * @param path_video_right the path of the right video.
     * @param start_frame the starting frame to begin extraction.
     * @param nb_frames the number of frames to extract. -1 is unlimited.
     * @param output_left Output left vector of images.
     * @param output_right Output right vector of images.
     * @param choose Boolean for manual selection.
     */
    static void stereoVideoExtraction(std::string path_video_left,
                                      std::string path_video_right,
                                      int start_frame, int nb_frames,
                                      std::vector<cv::Mat>& output_left, std::vector<cv::Mat> &output_right,
                                      bool choose = false);


};

#endif // VIDEOANALYSIS_H
