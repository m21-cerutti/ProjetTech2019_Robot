#include "videoanalyser.h"


void VideoAnalyser::videoChessDebug(std::string path_video)
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
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, DELAY_DEBUG_VIDEO, 0.1));
            drawChessboardCorners(gray, board_size, corners, found);
        }

        imshow("Video_reader", gray);
        if(waitKey(DELAY_DEBUG_VIDEO) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
    }
    return;
}

void VideoAnalyser::stereoVideoExtraction(std::string path_video_left, std::string path_video_right,
                                          int start_frame,
                                          int nb_frames,
                                          std::vector<cv::Mat> &output_left,
                                          std::vector<cv::Mat> &output_right,
                                          bool choose)
{
    using namespace cv;

    ProjectUtilities::messageDebug("Begin of capture's extraction.", false);

    VideoCapture cap_left(path_video_left);
    VideoCapture cap_right(path_video_right);

    if ( !cap_left.isOpened() || ! cap_right.isOpened())
    {
        ProjectUtilities::messageDebug("Cannot open the videos files.", true);
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
            ProjectUtilities::messageDebug("Cannot read the videos files.", true);
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
                        ProjectUtilities::messageDebug("Frame "+std::to_string(accepted_frames)+" accepted. Index "+std::to_string(index_frame)+".", false);
                        break;
                    }
                    else if(waitKey(DELAY_DEBUG_VIDEO) == 27) // Wait for 'esc' key press to exit
                    {
                        ProjectUtilities::messageDebug("Frame rejected : index "+std::to_string(index_frame)+".", false);
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
    ProjectUtilities::messageDebug("End of capture.", false);
    return;
}

