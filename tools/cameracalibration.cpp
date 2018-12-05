#include "cameracalibration.h"


bool CameraCalibration::calibrateFromImages(const std::vector<cv::Mat> &sources_images, cv::Mat &out)
{

    return false;
}

bool CameraCalibration::findCalibrate(const cv::Mat &source, cv::Mat &out)
{
    using namespace cv;

    ImageAnalyser::showMatrice("Start", source);

    Mat gray, undistorded;
    int num_squares = CHESS_WIDTH * CHESS_HEIGHT;
    Size board_size = Size(CHESS_WIDTH, CHESS_HEIGHT);

    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> image_points;

    std::vector<Point2f> corners;

    std::vector<Point3f> obj;
    for(int j=0;j<num_squares;j++)
    {
        obj.push_back(Point3f(j%CHESS_WIDTH, j/CHESS_WIDTH, 0.0f));
        //qDebug() << "Points("<< j%CHESS_WIDTH <<" ; "<< j/CHESS_WIDTH << ")";
    }

    cvtColor(source, gray, CV_BGR2GRAY);

    bool found = findChessboardCorners(gray, board_size, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FILTER_QUADS | CALIB_CB_NORMALIZE_IMAGE |  CALIB_CB_FAST_CHECK);

    if(found)
    {
        cornerSubPix(gray, corners,
                     Size(11, 11),
                     Size(-1, -1),
                     TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(gray, board_size, corners, found);

        image_points.push_back(corners);
        object_points.push_back(obj);

    }
    else
    {
        return false;
    }

    ImageAnalyser::showMatrice("Points", gray);

    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    calibrateCamera(object_points, image_points, source.size(), intrinsic, distCoeffs, rvecs, tvecs);

    undistort(source, undistorded, intrinsic, distCoeffs);

    ImageAnalyser::showMatrice("Undistorded", undistorded);

    undistorded.copyTo(out);
    return true;
}
