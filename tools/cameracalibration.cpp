#include "cameracalibration.h"


int CameraCalibration::chessBoardCalibration(const std::vector<cv::Mat> &sources_images, std::string path_camera_file)
{
    using namespace cv;

    Mat gray;
    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> image_points;

    //FIND CHESSBOARD
    //Initialisation
    Size board_size = Size(CHESS_WIDTH, CHESS_HEIGHT);
    std::vector<Point2f> corners;
    std::vector<Point3f> obj;
    for (double i = 0; i < CHESS_HEIGHT; i++){
        for (double j = 0; j < CHESS_WIDTH; j++){
            obj.push_back(Point3f((float)(i*SQUARE_SIZE), (float)(j*SQUARE_SIZE), 0.0f));
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
            //drawChessboardCorners(gray, board_size, corners, found);

            image_points.push_back(corners);
            object_points.push_back(obj);
        }
        else
        {
            nb_rejected++;
            ProjectDebuger::showMatrice("IMG_rejected_"+std::to_string(nb_rejected), source);
        }
    }
    //END FIND CHESSBOARD

    //Parameters to save
    Mat camera_matrix = Mat::zeros(3, 3, CV_64F);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    camera_matrix.ptr<float>(0)[0] = 1;
    camera_matrix.ptr<float>(1)[1] = 1;

    //Calibration
    calibrateCamera(object_points, image_points, sources_images[0].size(), camera_matrix, dist_coeffs, rvecs, tvecs, 0, TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 60, DBL_EPSILON) );

    //Save
    saveCameraParameters( path_camera_file, camera_matrix, dist_coeffs, rvecs, tvecs);

    return nb_rejected;
}

int CameraCalibration::charucoCalibration(const std::vector<cv::Mat> &sources_images, std::string path_camera_file)
{
    using namespace cv;

    Mat gray;
    Mat camera_matrix = Mat(3, 3, CV_64F);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);

    std::vector<std::vector<cv::Point2f>> allCharucoCorners;
    std::vector<std::vector<int>> allCharucoIds;

    //FIND CHARUCO
    //Initialisation

    Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();
    detectorParams->doCornerRefinement = true;

    detectorParams->adaptiveThreshWinSizeMin = 3;
    detectorParams->adaptiveThreshWinSizeMax = 23;
    detectorParams->adaptiveThreshWinSizeStep = 10;
    detectorParams->adaptiveThreshConstant = 7;
    detectorParams->minMarkerPerimeterRate = 0.03;
    detectorParams->maxMarkerPerimeterRate = 4.0;
    detectorParams->polygonalApproxAccuracyRate = 0.05;
    detectorParams->minCornerDistanceRate = 0.05;
    detectorParams->minDistanceToBorder = 3;
    detectorParams->minMarkerDistanceRate = 0.05;
    detectorParams->cornerRefinementWinSize = 5;
    detectorParams->cornerRefinementMaxIterations = 120;//30
    detectorParams->cornerRefinementMinAccuracy = 0.1;
    detectorParams->markerBorderBits = 1;
    detectorParams->perspectiveRemovePixelPerCell = 8;
    detectorParams->perspectiveRemoveIgnoredMarginPerCell = 0.13;
    detectorParams->maxErroneousBitsInBorderRate = 0.04;
    detectorParams->minOtsuStdDev = 5.0;
    detectorParams->errorCorrectionRate = 0.6;

    Ptr< aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_ARUCO_ORIGINAL);

    cv::Ptr<aruco::CharucoBoard> board = aruco::CharucoBoard::create(CHARU_WIDHT, CHARU_HEIGHT, SQUARE_SIZE, MARKER_SIZE, dictionary);

    int nb_rejected = 0;
    int nb_sucess = 0;
    for(cv::Mat source : sources_images)
    {
        cv::cvtColor(source, gray, CV_BGRA2GRAY);
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners, rejected;

        cv::aruco::detectMarkers(gray, dictionary, corners, ids, detectorParams);
        cv::aruco::refineDetectedMarkers(gray, board, corners, ids, rejected);

        // if at least one marker detected
        if (ids.size() > 0) {
            //cv::aruco::drawDetectedMarkers(gray, corners, ids);
            std::vector<cv::Point2f> charucoCorners;
            std::vector<int> charucoIds;

            //ProjectDebuger::showMatrice("Charuco"+std::to_string(nb_sucess++), gray);



            aruco::interpolateCornersCharuco(corners, ids, gray, board, charucoCorners, charucoIds);

            // if at least one charuco corner detected
            if(charucoIds.size() > 0){
                nb_sucess++;

                cv::aruco::drawDetectedCornersCharuco(gray, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));
                ProjectDebuger::showMatrice("Charuco"+std::to_string(nb_sucess++), gray);
            }
            else
            {
                nb_rejected++;
                //ProjectDebuger::showMatrice("IMG_NoCharuco_"+std::to_string(nb_rejected), gray);
            }
        }
        else
        {
            nb_rejected++;
            //ProjectDebuger::showMatrice("IMG_rejected_"+std::to_string(nb_rejected), gray);
        }
    }
    //END FIND CHARUCO

    /*
    //Parameters to save
    Mat camera_matrix = Mat(3, 3, CV_64F);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    camera_matrix.ptr<float>(0)[0] = 1;
    camera_matrix.ptr<float>(1)[1] = 1;

    //Calibration
    double repError = cv::aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds, board, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs, calibrationFlags);

    //Save
    saveCameraParameters( path_camera_file, camera_matrix, dist_coeffs, rvecs, tvecs);
*/
    return 0;
}

void CameraCalibration::calibrateFromImages(const std::vector<cv::Mat> &sources_images, std::string path_camera_file, MODE_CALIBRATION behaviour)
{
    using namespace cv;


    switch (behaviour)
    {
    case Chessboard:
        if(chessBoardCalibration(sources_images, path_camera_file)>0)
        {
            ProjectDebuger::messageDebug("Some images have been not taken for calibration.", false);
        }
        break;

    case Charuco:
        if(charucoCalibration(sources_images, path_camera_file)>0)
        {
            ProjectDebuger::messageDebug("Some images have been not taken for calibration.", false);
        }
        break;
    }
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
                                             const std::vector<cv::Mat>& tvecs)
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
