#include "cameracalibration.h"


bool CameraCalibration::calibrateFromImages(const std::vector<cv::Mat> &sources_images, cv::Mat &out)
{
    return false;
}

bool CameraCalibration::findOneCalibration(const cv::Mat &source, cv::Mat &out)
{
    using namespace cv;

    Mat gray, undistorded;
    int num_squares = CHESS_WIDTH * CHESS_HEIGHT;
    Size board_size = Size(CHESS_WIDTH, CHESS_HEIGHT);

    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> image_points;

    std::vector<Point2f> corners;

    std::vector<Point3f> obj;
    for(int j=0;j<num_squares;j++)
    {
        obj.push_back(Point3f(j/CHESS_WIDTH, j%CHESS_WIDTH, 0.0f));
    }

    cv::cvtColor(source, gray, CV_BGRA2GRAY);
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

    //Parameters to save
    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat dist_coeffs;
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    //Calibration
    calibrateCamera(object_points, image_points, source.size(), intrinsic, dist_coeffs, rvecs, tvecs);

    //Save parameters
    std::ofstream outstream;
    std::ofstream::openmode mode{std::fstream::out};
    mode |= std::ofstream::trunc;
    outstream.open("default_camera.txt", mode );

    if (!outstream.is_open())
        throw Exception ( 1, "Fail to open file \"default_camera.txt\"." ,
                          "CameraCalibration::findCalibrate(const cv::Mat &source, cv::Mat &out)", __FILE__, __LINE__);

    outstream << "intrinsic" << std::endl;
    outstream << intrinsic << std::endl;
    outstream << "distCoeffs" << std::endl;
    outstream << dist_coeffs << std::endl;
    outstream << "rvecs" << std::endl;
    for(auto v : rvecs)
        outstream << v << std::endl;
    outstream << "tvecs" << std::endl;
    for(auto v : tvecs)
        outstream << v << std::endl;
    outstream.close();

    if ( !outstream.good())
        throw Exception ( 2, "Fail to write in the file \"default_camera.txt\".",
                          "CameraCalibration::findCalibrate(const cv::Mat &source, cv::Mat &out)", __FILE__, __LINE__);

    std::cout << "File done." << std::endl;
    undistort(source, undistorded, intrinsic, dist_coeffs);

    undistorded.copyTo(out);
    return true;
}
