#include "tools/cameracalibration.h"


int CameraCalibration::chessBoardCalibration(const std::vector<cv::Mat> &sources_images, const std::string path_camera_file)
{
    using namespace cv;

    //Parameters to save
    Mat camera_matrix = Mat::zeros(3, 3, CV_64F);
    Mat dist_coeffs = Mat::zeros(8, 1, CV_64F);
    std::vector<Mat> rvecs;
    std::vector<Mat> tvecs;
    camera_matrix.ptr<float>(0)[0] = 1;
    camera_matrix.ptr<float>(1)[1] = 1;

    return chessBoardCalibration(sources_images, path_camera_file, camera_matrix, dist_coeffs, rvecs, tvecs);
}

int CameraCalibration::chessBoardCalibration(const std::vector<cv::Mat> &sources_images,
                                             const std::string path_camera_file,
                                             cv::Mat& camera_matrix,
                                             cv::Mat& dist_coeffs,
                                             std::vector<cv::Mat>& rvecs,
                                             std::vector<cv::Mat>& tvecs)
{
    using namespace cv;

    std::vector<std::vector<Point3f>> object_points;
    std::vector<std::vector<Point2f>> image_points;

    ProjectUtilities::messageDebug( "Starting chessboard calibration.", false);

    //FIND CHESSBOARD
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
    Mat gray;
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

            image_points.push_back(corners);
            object_points.push_back(obj);
        }
        else
        {
            nb_rejected++;
        }
    }
    //END FIND CHESSBOARD
    ProjectUtilities::messageDebug( std::to_string(nb_rejected) + " images rejected.", false);

    //Calibration
    double rmserror = calibrateCamera(object_points, image_points, sources_images[0].size(), camera_matrix, dist_coeffs, rvecs, tvecs, 0, TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 60, DBL_EPSILON) );

    ProjectUtilities::messageDebug( "Calibration finish with " + std::to_string(rmserror) + " of error.", false);

    //Save
    ProjectUtilities::messageDebug( "Saving calibration in "+path_camera_file+ "...", false);
    ProjectFiles::saveIntrinsicCamera( path_camera_file, sources_images[0].size(), camera_matrix, dist_coeffs, rvecs, tvecs);

    return nb_rejected;
}

int CameraCalibration::charucoCalibration(const std::vector<cv::Mat> &sources_images,
                                          const std::string path_camera_file)
{
    cv::Mat camera_matrix = cv::Mat(3, 3, CV_64F);
    cv::Mat dist_coeffs = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    camera_matrix.ptr<float>(0)[0] = 1;
    camera_matrix.ptr<float>(1)[1] = 1;


    return charucoCalibration(sources_images,path_camera_file, camera_matrix, dist_coeffs, rvecs, tvecs);
}


int CameraCalibration::charucoCalibration(const std::vector<cv::Mat> &sources_images,
                                          const std::string path_camera_file,
                                          cv::Mat& camera_matrix,
                                          cv::Mat& dist_coeffs,
                                          std::vector<cv::Mat>& rvecs,
                                          std::vector<cv::Mat>& tvecs)
{
    using namespace cv;

    Mat gray;

    ProjectUtilities::messageDebug( "Starting charuco calibration.", false);

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
    detectorParams->cornerRefinementMaxIterations = 30;
    detectorParams->cornerRefinementMinAccuracy = 0.1;
    detectorParams->markerBorderBits = 1;
    detectorParams->perspectiveRemovePixelPerCell = 8;
    detectorParams->perspectiveRemoveIgnoredMarginPerCell = 0.13;
    detectorParams->maxErroneousBitsInBorderRate = 0.04;
    detectorParams->minOtsuStdDev = 5.0;
    detectorParams->errorCorrectionRate = 0.6;

    Ptr< aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_ARUCO_ORIGINAL);

    cv::Ptr<aruco::CharucoBoard> board = aruco::CharucoBoard::create(CHARU_WIDHT, CHARU_HEIGHT, (float)(SQUARE_SIZE), (float)(MARKER_SIZE), dictionary);

    int nb_rejected = 0;
    for(cv::Mat source : sources_images)
    {
        cv::cvtColor(source, gray, CV_BGRA2GRAY);
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners, rejected;

        cv::aruco::detectMarkers(gray, dictionary, corners, ids, detectorParams, rejected);
        cv::aruco::refineDetectedMarkers(gray, board, corners, ids, rejected);

        // if at least one marker detected
        if (ids.size() > 0) {
            std::vector<cv::Point2f> charucoCorners;
            std::vector<int> charucoIds;
            aruco::interpolateCornersCharuco(corners, ids, gray, board, charucoCorners, charucoIds, camera_matrix, dist_coeffs, MIN_MARKERS);
            // if at least one charuco corner detected
            if(charucoIds.size() > MIN_MARKERS){
                cv::aruco::drawDetectedCornersCharuco(gray, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));
                allCharucoCorners.push_back(charucoCorners);
                allCharucoIds.push_back(charucoIds);
            }
            else
            {
                nb_rejected++;
            }
        }
        else
        {
            nb_rejected++;
        }
    }
    ProjectUtilities::messageDebug( std::to_string(nb_rejected) + " images rejected.", false);
    //END FIND CHARUCO

    //Calibration
    double rmserror = cv::aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds, board, sources_images[0].size(),
            camera_matrix, dist_coeffs, rvecs, tvecs, CV_CALIB_FIX_INTRINSIC);

    ProjectUtilities::messageDebug( "Calibration finish with " + std::to_string(rmserror) + " of error.", false);

    //Save
    ProjectUtilities::messageDebug( "Saving calibration in "+path_camera_file+ "...", false);
    ProjectFiles::saveIntrinsicCamera( path_camera_file, sources_images[0].size(), camera_matrix, dist_coeffs, rvecs, tvecs);

    return nb_rejected;
}

void CameraCalibration::applyUndistorded(const cv::Mat &source, cv::Mat &out,
                                         cv::Mat &camera_matrix, cv::Mat &dist_coeffs)
{
    using namespace cv;
    cv::Mat undistorded;
    undistort(source, undistorded, camera_matrix, dist_coeffs);
    undistorded.copyTo(out);
}


void CameraCalibration::applyUndistorded(const cv::Mat &source, cv::Mat &out, const std::string file_path)
{
    using namespace cv;

    Mat camera_matrix, dist_coeffs;
    cv::Size img_size;
    if(!ProjectFiles::loadIntrinsicCamera(file_path, img_size, camera_matrix, dist_coeffs))
        return;

    cv::Mat undistorded;
    undistort(source, undistorded, camera_matrix, dist_coeffs);

    undistorded.copyTo(out);
}

int CameraCalibration::stereoChessboard(const std::vector<cv::Mat> &sources_images_left,
                                        const std::vector<cv::Mat> &sources_images_right,
                                        const std::string path_camera_l,
                                        const std::string path_camera_r,
                                        std::vector<std::vector<cv::Point3f>> &object_points,
                                        std::vector<std::vector<cv::Point2f>> &left_img_points,
                                        std::vector<std::vector<cv::Point2f>> &right_img_points)
{
    using namespace cv;

    ProjectUtilities::messageDebug( "Starting chessboard stereo calibration.", false);

    //FIND CHESSBOARD
    //Initialisation
    Size board_size = Size(CHESS_WIDTH, CHESS_HEIGHT);
    std::vector<Point2f> corners_l;
    std::vector<Point2f> corners_r;
    std::vector<Point3f> obj;
    for (float i = 0; i < CHESS_HEIGHT; i++){
        for (float j = 0; j < CHESS_WIDTH; j++){
            obj.push_back(Point3f(i*SQUARE_SIZE, j*SQUARE_SIZE, 0.0f));
        }
    }

    //Research
    int nb_rejected=0;
    Mat gray_l, gray_r;
    for(int i =0; i<sources_images_left.size() && i<sources_images_right.size(); i++)
    {
        Mat source_l = sources_images_left.at(i);
        Mat source_r = sources_images_right.at(i);

        cv::cvtColor(source_l, gray_l, CV_BGRA2GRAY);
        cv::cvtColor(source_r, gray_r, CV_BGRA2GRAY);

        bool found1 = findChessboardCorners(gray_l, board_size, corners_l, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FILTER_QUADS|  CALIB_CB_FAST_CHECK);
        bool found2 = findChessboardCorners(gray_r, board_size, corners_r, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FILTER_QUADS|  CALIB_CB_FAST_CHECK);


        if(found1 && found2)
        {
            cornerSubPix(gray_l, corners_l,
                         Size(11, 11),
                         Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.01));
            cornerSubPix(gray_r, corners_r,
                         Size(11, 11),
                         Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.01));

            left_img_points.push_back(corners_l);
            right_img_points.push_back(corners_r);
            object_points.push_back(obj);
        }
        else
        {
            nb_rejected++;
        }
    }
    //END FIND CHESSBOARD
    ProjectUtilities::messageDebug( std::to_string(nb_rejected) + " images rejected.", false);

    //Calibration
    cv::Mat camera_matrix_l = cv::Mat(3, 3, CV_64F);
    cv::Mat dist_coeffs_l = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecs_l;
    std::vector<cv::Mat> tvecs_l;
    camera_matrix_l.ptr<float>(0)[0] = 1;
    camera_matrix_l.ptr<float>(1)[1] = 1;

    cv::Mat camera_matrix_r = cv::Mat(3, 3, CV_64F);
    cv::Mat dist_coeffs_r = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecs_r;
    std::vector<cv::Mat> tvecs_r;
    camera_matrix_r.ptr<float>(0)[0] = 1;
    camera_matrix_r.ptr<float>(1)[1] = 1;

    double rmserror1 = calibrateCamera(object_points, left_img_points, sources_images_left[0].size(), camera_matrix_l, dist_coeffs_l, rvecs_l, tvecs_l);
    ProjectUtilities::messageDebug( "Calibration finish with left :" + std::to_string(rmserror1) + " of error.", false);
    double rmserror2 = calibrateCamera(object_points, right_img_points, sources_images_right[0].size(), camera_matrix_r, dist_coeffs_r, rvecs_r, tvecs_r);
    ProjectUtilities::messageDebug( "And right :" + std::to_string(rmserror2) + " of error.", false);

    //Save
    ProjectUtilities::messageDebug( "Saving calibration in "+path_camera_l+ " and "+path_camera_r+"...", false);
    ProjectFiles::saveIntrinsicCamera( path_camera_l, sources_images_left[0].size(), camera_matrix_l, dist_coeffs_l, rvecs_l, tvecs_l);
    ProjectFiles::saveIntrinsicCamera( path_camera_r, sources_images_right[0].size(), camera_matrix_r, dist_coeffs_r, rvecs_r, tvecs_r);

    return nb_rejected;
}

void CameraCalibration::stereoCalibration(const std::string path_file_stereo,
                                          const std::vector<cv::Mat> &sources_images_left,
                                          const std::vector<cv::Mat> &sources_images_right,
                                          const std::string path_camera_file_l,
                                          const std::string path_camera_file_r)
{
    using namespace cv;

    cv::Mat camera_matrix_l, dist_coeffs_l, camera_matrix_r, dist_coeffs_r;
    cv::Size img_size ;

    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> left_img_points;
    std::vector<std::vector<cv::Point2f>> right_img_points;

    stereoChessboard(sources_images_left,
                     sources_images_right,
                     path_camera_file_l,
                     path_camera_file_r,
                     object_points,
                     left_img_points,
                     right_img_points);

    ProjectFiles::loadIntrinsicCamera(path_camera_file_l, img_size, camera_matrix_l, dist_coeffs_l);
    ProjectFiles::loadIntrinsicCamera(path_camera_file_r, img_size, camera_matrix_r, dist_coeffs_r);

    ProjectUtilities::messageDebug("Starting Calibration.",false);
    ProjectUtilities::messageDebug("Read intrasic...",false);

    ProjectUtilities::messageDebug( "Starting stereo calibration.", false);

    Mat R, F, E;
    Vec3d T;
    int flag = CV_CALIB_FIX_INTRINSIC;

    ProjectUtilities::messageDebug("Stereo calibrate...", false);
    double rmserror = stereoCalibrate(object_points, left_img_points, right_img_points, camera_matrix_l, dist_coeffs_l,
                                      camera_matrix_r, dist_coeffs_r, img_size, R, T, E, F, flag);
    ProjectUtilities::messageDebug( "Calibration finish with " + std::to_string(rmserror) + " of error.", false);

    ProjectUtilities::messageDebug("Starting Rectification.", false);
    cv::Mat R1, R2, P1, P2, Q;
    stereoRectify(camera_matrix_l, dist_coeffs_l, camera_matrix_r, dist_coeffs_r, img_size, R, T, R1, R2, P1, P2, Q);
    ProjectUtilities::messageDebug("Done Rectification.", false);

    /*
    int i =0;
    for(cv::Mat img : sources_images_left)
    {
        cv::Mat tmp;
        undistort(img, tmp, camera_matrix_l, dist_coeffs_l);
        ProjectUtilities::showMatrice("left_" + std::to_string(i), tmp);
        i++;
    }
    i=0;
    for(cv::Mat img : sources_images_right)
    {
        cv::Mat tmp;
        undistort(img, tmp, camera_matrix_r, dist_coeffs_r);
        ProjectUtilities::showMatrice("right_" + std::to_string(i), tmp);
        i++;
    }
    */

    ProjectFiles::saveCameraStereoParameters(path_file_stereo, camera_matrix_l, dist_coeffs_l, camera_matrix_r, dist_coeffs_r, img_size, R, F, E, T, R1, R2, P1, P2, Q);
}


