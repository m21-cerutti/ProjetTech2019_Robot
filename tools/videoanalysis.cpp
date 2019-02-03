#include "videoanalysis.h"


void VideoAnalysis::videoChessDebug(std::string path_video)
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

    int index_frame = 0;

    Mat gray;
    Size board_size = Size(CHESS_WIDTH, CHESS_HEIGHT);
    std::vector<Point2f> corners;

    while(true)
    {
        Mat frame;
        index_frame++;
        if (!cap.read(frame))
        {
            std::cout<<"\n Cannot read the video file. \n";
            break;
        }

        //Visualisation
        cv::cvtColor(frame, gray, CV_BGRA2GRAY);
        bool found = findChessboardCorners(gray, board_size, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FILTER_QUADS|  CALIB_CB_FAST_CHECK);

        if(found)
        {
            cornerSubPix(gray, corners,
                         Size(11, 11),
                         Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(gray, board_size, corners, found);
        }

        imshow("Video_reader", gray);
        if(waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
    }
    return;
}
