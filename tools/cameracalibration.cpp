#include "cameracalibration.h"


int CameraCalibration::findChessBoard(const std::vector<cv::Mat> &sources_images,
                                       std::vector<std::vector<cv::Point3f>>& object_points,
                                       std::vector<std::vector<cv::Point2f>>& image_points)
{
    using namespace cv;

    Mat gray;

    //Initialisation
    Size board_size = Size(CHESS_WIDTH, CHESS_HEIGHT);
    std::vector<Point2f> corners;
    std::vector<Point3f> obj;
    for (float i = 0; i < CHESS_HEIGHT; i++){
        for (float j = 0; j < CHESS_WIDTH; j++){
            obj.push_back(Point3f(i*SQUARE_SIZE, j*SQUARE_SIZE, 0.0f));
        }
    }

    //Research
    int nb_rejected=0;
    for(cv::Mat source : sources_images)
    {
        cv::cvtColor(source, gray, CV_BGRA2GRAY);
        bool found = findChessboardCorners(gray, board_size, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FILTER_QUADS|  CALIB_CB_FAST_CHECK);

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
            nb_rejected++;
            ProjectDebuger::showMatrice("IMG_rejected_"+std::to_string(nb_rejected), source);
        }
    }
    return nb_rejected;
}

void CameraCalibration::calibrateFromImages(const std::vector<cv::Mat> &sources_images, std::string path_camera_file, MODE_CALIBRATION behaviour)
{
    using namespace cv;

    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> image_points;

    switch (behaviour)
    {
    case Chessboard:
        findChessBoard(sources_images, object_points, image_points);
        break;

    case Charuco:
        break;
    }

    //Parameters to save
    Mat intrinsic = Mat(3, 3, CV_64F);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    //Calibration
    calibrateCamera(object_points, image_points, sources_images[0].size(), intrinsic, dist_coeffs, rvecs, tvecs);

    //Save
    saveCameraParameters( path_camera_file, intrinsic, dist_coeffs, rvecs, tvecs);

    //Debug
    /*
    int i = 0;
    for (cv::Mat source : sources_images)
    {
        Mat undistorded;
        undistort(source, undistorded, intrinsic, dist_coeffs);
        ProjectDebuger::showMatrice("imagecalib_"+std::to_string(i++),undistorded);
    }
    */
}


void CameraCalibration::applyCameraParametersUndistorded(const std::string file_path, const cv::Mat &source, cv::Mat &out)
{
    std::ifstream instream;
    std::ifstream::openmode mode{std::fstream::in};

    instream.open(file_path, mode );

    if (!instream.is_open())
        throw cv::Exception ( 1, "Fail to open file \""+file_path+"\"." ,
                              "CameraCalibration::applyCameraParametersUndistorded(const std::__cxx11::string file_path, const cv::Mat &source, cv::Mat &out)", __FILE__, __LINE__);

    /*
    instream << "intrinsic" << std::endl;
    instream << intrinsic << std::endl;
    instream << "distCoeffs" << std::endl;
    instream << dist_coeffs << std::endl;
    */

    instream.close();

    if ( !instream.good())
        throw cv::Exception ( 2, "Fail to write in the file \""+file_path+"\".",
                              "CameraCalibration::findCalibrate(const cv::Mat &source, cv::Mat &out)", __FILE__, __LINE__);

    std::cout << "Apply read." << std::endl;

    cv::Mat undistorded;
    //undistort(source, undistorded, intrinsic, dist_coeffs);

    undistorded.copyTo(out);
}


/*A faire pour set 1
 *
 * https://docs.opencv.org/3.2.0/da/d13/tutorial_aruco_calibration.html
*/

void CameraCalibration::saveCameraParameters(const std::string file_path,
                                             const cv::Mat intrinsic,
                                             const  cv::Mat dist_coeffs,
                                             const std::vector<cv::Mat> rvecs,
                                             const std::vector<cv::Mat> tvecs )
{
    std::ofstream outstream;
    std::ofstream::openmode mode{std::fstream::out};
    mode |= std::ofstream::trunc;
    outstream.open(file_path, mode );

    if (!outstream.is_open())
        throw cv::Exception ( 1, "Fail to open file \""+file_path+"\"." ,
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
        throw cv::Exception ( 2, "Fail to write in the file \""+file_path+"\".",
                              "CameraCalibration::findCalibrate(const cv::Mat &source, cv::Mat &out)", __FILE__, __LINE__);

    std::cout << "File done." << std::endl;
}
