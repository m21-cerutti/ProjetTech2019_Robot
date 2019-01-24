#include "cameracalibration.h"


/*A faire pour set 1 Charuco
 *
 * https://docs.opencv.org/3.2.0/da/d13/tutorial_aruco_calibration.html
*/

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
    Mat camera_matrix = Mat(3, 3, CV_64F);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    camera_matrix.ptr<float>(0)[0] = 1;
    camera_matrix.ptr<float>(1)[1] = 1;

    //Calibration
    calibrateCamera(object_points, image_points, sources_images[0].size(), camera_matrix, dist_coeffs, rvecs, tvecs);

    //Save
    saveCameraParameters( path_camera_file, camera_matrix, dist_coeffs, rvecs, tvecs);

    //Debug
    /*
    int i = 0;
    for (cv::Mat source : sources_images)
    {
        Mat undistorded;
        undistort(source, undistorded, camera_matrix, dist_coeffs);
        ProjectDebuger::showMatrice("imagecalib_"+std::to_string(i++),undistorded);
    }
    */
}


void CameraCalibration::applyUndistordedFromFile(const std::string file_path, const cv::Mat &source, cv::Mat &out)
{
    using namespace cv;

    Mat camera_matrix, dist_coeffs;
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    if(!loadCameraParemeters(file_path, camera_matrix, dist_coeffs, rvecs, tvecs))
        return;

    cv::Mat undistorded;
    undistort(source, undistorded, camera_matrix, dist_coeffs);

    undistorded.copyTo(out);
}

bool CameraCalibration::loadCameraParemeters(const std::string file_path, cv::Mat& camera_matrix, cv::Mat& dist_coeffs, std::vector<cv::Mat>& rvecs, std::vector<cv::Mat>& tvecs)
{
    using namespace cv;

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);
    if(!fs.isOpened()) {
        ProjectDebuger::messageDebug("Can't open file for read calibration.", true);
        return false;
    }

    std::string date;
    fs["calibration_date"] >> date;
    std::cout << "Calibration date : " <<  date;
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;
    fs["rvecs"] >> rvecs;
    fs["tvecs"] >> tvecs;
    fs.release();

    std::cout << "Read camera parameter sucessfull." << std::endl;
    return true;
}


bool CameraCalibration::saveCameraParameters(const std::string file_path,
                                             const cv::Mat& camera_matrix,
                                             const  cv::Mat& dist_coeffs,
                                             const std::vector<cv::Mat>& rvecs,
                                             const std::vector<cv::Mat>& tvecs )
{
    using namespace cv;

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::WRITE);
    if(!fs.isOpened()) {
        ProjectDebuger::messageDebug("Can't open file for write calibration.", true);
        return false;
    }

    time_t rawtime; time(&rawtime);
    std::string date =  asctime(localtime(&rawtime));
    fs << "calibration_date" << date;
    fs << "camera_matrix" << camera_matrix;
    fs << "dist_coeffs" << dist_coeffs;
    fs << "rvecs" << rvecs;
    fs << "tvecs" << tvecs;
    fs.release();

    std::cout << "File done. Date : " << date <<std::endl;
    return true;
}
