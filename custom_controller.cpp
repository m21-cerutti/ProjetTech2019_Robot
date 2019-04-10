#include "custom_controller.h"


namespace cerutti
{

CustomController::CustomController()
{

}

CustomController::~CustomController() {
    // Do not forget to move any memory owned by your controller
}

void CustomController::process(const cv::Mat & left_img,
                               const cv::Mat & right_img,
                               float * vx, float * vy, float * omega)
{
    using namespace cv;

    nb_frame++;

    Utilities::showMatrice(std::to_string(nb_frame)+ "_left", left_img);
    Utilities::showMatrice(std::to_string(nb_frame)+ "_right", right_img);
    Utilities::messageDebug("Normal images save.", false);

    cv::Mat disparity, depth_map;

    Mat left_img_undist,
            right_img_undist;

    //UNDISTORD
    undistort(left_img, left_img_undist, calib.camera_matrix_l, calib.dist_coeffs_l);
    undistort(right_img, right_img_undist, calib.camera_matrix_r, calib.dist_coeffs_r);

    //Utilities::showMatrice("Undistord_l_"+std::to_string(nb_frame), left_img_undist);
    //Utilities::showMatrice("Undistord_r_"+std::to_string(nb_frame), right_img_undist);
    //Utilities::messageDebug("Undistord images save.", false);

    //DISPARITY
    StereoMap::computeSGBMDisparity( left_img_undist, right_img_undist, disparity, sgbm_state);

    //Utilities::showMatrice("Disparity_"+std::to_string(nb_frame), disparity);
    //Utilities::messageDebug("Disparity images save.", false);

    //DEPTH
    StereoMap::computeDepthMap(disparity, calib.Q, depth_map, THRESHOLD_MIN, THRESHOLD_MAX);

    Utilities::showMatrice("Depth_"+std::to_string(nb_frame), depth_map);
    Utilities::messageDebug("Depth images save.", false);

    *vx = 0;
    *vy = 0;
    *omega = 0;


}

void CustomController::load()
{
    using namespace cv;

    Utilities::messageDebug( "Starting load robot files.", false);
    nb_frame = 0;

    if(!calib.load(DEFAULT_CALIB_FILE))
    {
        Utilities::messageDebug("Load calibration fail.");
    }
    if(!StereoMap::loadSGBMParameters(sgbm_state))
    {
        Utilities::messageDebug("Load disparity parameter fail.");
    }

    Utilities::messageDebug("Load sucessfull.", false);
}

//////////////////////////////////

void Utilities::messageDebug(std::string message, bool error)
{
#ifdef IS_COMPUTER
    if(error)
    {
        qWarning()<<"Error : " << message.c_str() << "\n";
        return;
    }
    qDebug()<<"Debug : " << message.c_str() << "\n";
#else
    if(error)
    {
        std::cerr << "Error : " << message <<std::endl;
        return;
    }
    std::cout << "Debug : " << message <<std::endl;
#endif
}

void Utilities::showMatrice(std::string name, const cv::Mat &mat)
{
    if(mat.empty())
    {
        Utilities::messageDebug("Matrice empty.", true);
        return;
    }

#ifdef IS_COMPUTER

    cv::Mat tmp;
    if(mat.depth() != CV_8U)
    {
        cv::normalize(mat, tmp, 0, 255, CV_MINMAX, CV_8UC1);
    }
    else
    {
        tmp = mat;
    }

    namedWindow(name, cv::WINDOW_NORMAL);
    cv::imshow(name, tmp);

#else

    using namespace cv;

    std::string folder = DEBUG_FOLDER;

    std::string folder_cmd = "mkdir -p \""+folder+"\"";
    if (std::system(folder_cmd.c_str()) == 0)
    {

        Utilities::messageDebug("Create folder done.", false);
    }
    else
    {
        Utilities::messageDebug("Error create folder.", true);
    }
    Mat tmp;
    if(mat.type() == CV_32F)
    {
        double min;
        double max;
        cv::minMaxIdx(mat, &min, &max);
        double scale = 255 / (max-min);
        mat.convertTo(tmp, CV_8UC1, scale, -min*scale);
        applyColorMap(tmp, tmp, cv::COLORMAP_HOT);
    }
    else
    {
        mat.copyTo(tmp);
    }

    Utilities::messageDebug( "Starting save image: " +name, false);
    imwrite(folder+"/"+name+".png", tmp);

    time_t rawtime; time(&rawtime);
    std::string date(asctime(localtime(&rawtime)));
    Utilities::messageDebug("Image saved."+date, false);

#endif

}

//////////////////////////////////

void Filters::separateImage(const cv::Mat& mat, cv::Mat &mat_left, cv::Mat &mat_right)
{
    /// Parity of the image
    int offset = 0;
    if(mat.empty())
    {
        return;
    }
    /// left img from otigin to src's middle
    mat_left = mat.colRange(0, mat.cols/2);
    /// Verify odd size
    offset = mat.cols % 2;
    mat_right = mat.colRange((mat.cols/2) + offset, mat.cols);
}

void Filters::computeGaussianBlur(const cv::Mat &src, cv::Mat &out)
{
    double sigma_x = 0, sigma_y = 0;
    cv::Size size(3,3);
    cv::GaussianBlur( src, out, size, sigma_x, sigma_y, cv::BORDER_DEFAULT );
}

void Filters::computeGradient(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, grad_x, grad_y;

    double scale = 1;
    double delta = 0;

    if(src.type() != CV_8UC1)
    {
        cv::cvtColor(src, gray, CV_BGRA2GRAY);
    }
    else
    {
        src.copyTo(gray);
    }

    computeGaussianBlur(gray, gray);

    /// Gradient X
    Sobel( gray, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, grad_x );

    /// Gradient Y
    Sobel( gray, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, grad_y );

    /// Total Gradient
    addWeighted( grad_x, 1, grad_y, 1, 0, gray, CV_8UC1);
    gray.convertTo(out, CV_8UC1);

}

void Filters::computeLaplacian(const cv::Mat &src, cv::Mat &out)
{
    cv::Mat gray, dest;

    ///Transform to Gray and smooth
    if(src.type() != CV_8UC1)
    {
        cv::cvtColor(src, gray, CV_BGRA2GRAY);
    }
    else
    {
        src.copyTo(gray);
    }
    computeGaussianBlur(gray, gray);

    /// Apply Laplace function
    cv::Laplacian( gray, dest, CV_16S, 3, 1, 1, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );
    dest.convertTo(out, CV_8UC1);
}

//////////////////////////////////

bool StereoMap::saveBMParameters(Ptr<StereoBM> &bm_state, std::string file_path)
{
    Utilities::messageDebug( "Starting save BM file : " +file_path, false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::WRITE);
    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for save BM file.", true);
        return false;
    }

    fs << "preFilterCap" << bm_state->getPreFilterCap();
    fs << "preFilterSize" << bm_state->getPreFilterSize();
    fs << "preFilterType" << bm_state->getPreFilterType();
    fs << "textureThreshold" << bm_state->getTextureThreshold();
    fs << "uniquenessRatio" << bm_state->getUniquenessRatio();
    fs << "blockSize" << bm_state->getBlockSize();
    fs << "disp12MaxDiff" << bm_state->getDisp12MaxDiff();
    fs << "minDisparity" << bm_state->getMinDisparity();
    fs << "numDisparities" << bm_state->getNumDisparities();

    fs.release();

    Utilities::messageDebug("Save file sucessfull.", false);
    return true;
}


bool StereoMap::saveSGBMParameters(Ptr<StereoSGBM> &sgbm_state, std::string file_path)
{
    Utilities::messageDebug( "Starting save SGBM file : " +file_path, false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::WRITE);
    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for save SGBM file.", true);
        return false;
    }

    fs << "minDisparity" << sgbm_state->getMinDisparity();
    fs << "numDisparities" << sgbm_state->getNumDisparities();
    fs << "blockSize" << sgbm_state->getBlockSize();
    fs << "P1" << sgbm_state->getP1();
    fs << "P2" << sgbm_state->getP2();
    fs << "disp12MaxDiff" << sgbm_state->getDisp12MaxDiff();
    fs << "preFilterCap" << sgbm_state->getPreFilterCap();
    fs << "uniquenessRatio" << sgbm_state->getUniquenessRatio();
    fs << "speckleWindowSize" << sgbm_state->getSpeckleWindowSize();
    fs << "speckleRange" << sgbm_state->getSpeckleRange();
    fs << "modeSGBM" << sgbm_state->getMode();

    fs.release();

    Utilities::messageDebug("Save file sucessfull.", false);
    return true;
}

bool StereoMap::loadBMParameters(Ptr<StereoBM> &bm_state, std::string file_path)
{
    Utilities::messageDebug( "Starting load disparity BM file.", false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);

    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file load disparity BM file.", true);
        return false;
    }

    //bgm parameters
    int preFilterCap;
    int preFilterSize;
    int preFilterType;
    int textureThreshold;
    int uniquenessRatio;
    int blockSize;
    int disp12MaxDiff;
    int minDisparity;
    int numDisparities;

    bm_state = StereoBM::create();

    fs["preFilterCap"] >> preFilterCap;
    bm_state->setPreFilterCap(preFilterCap);
    fs["preFilterSize"] >> preFilterSize;
    bm_state->setPreFilterSize(preFilterSize);
    fs["preFilterType"] >> preFilterType;
    bm_state->setPreFilterType(preFilterType);
    fs["textureThreshold"] >> textureThreshold;
    bm_state->setTextureThreshold(textureThreshold);
    fs["uniquenessRatio"] >> uniquenessRatio;
    bm_state->setUniquenessRatio(uniquenessRatio);
    fs["blockSize"] >> blockSize;
    bm_state->setBlockSize(blockSize);
    fs["disp12MaxDiff"] >> disp12MaxDiff;
    bm_state->setDisp12MaxDiff(disp12MaxDiff);
    fs["minDisparity"] >> minDisparity;
    bm_state->setMinDisparity(minDisparity);
    fs["numDisparities"] >> numDisparities;
    bm_state->setNumDisparities(numDisparities);

    fs.release();

    Utilities::messageDebug("Read file sucessfull.", false);
    return true;

}

bool StereoMap::loadSGBMParameters(Ptr<StereoSGBM> &sgbm_state, std::string file_path)
{
    Utilities::messageDebug( "Starting load disparity SGBM file.", false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);

    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for load disparity SGBM file.", true);
        return false;
    }

    //sgbgm parameters
    int minDisparity;
    int numDisparities;
    int blockSize;
    int P1;
    int P2;
    int disp12MaxDiff;
    int preFilterCap;
    int uniquenessRatio;
    int speckleWindowSize;
    int speckleRange;
    int modeSGBM;

    fs["minDisparity"] >> minDisparity;
    fs["numDisparities"] >> numDisparities;
    fs["blockSize"] >> blockSize;
    fs["P1"] >> P1;
    fs["P2"] >> P2;
    fs["disp12MaxDiff"] >> disp12MaxDiff;
    fs["preFilterCap"] >> preFilterCap;
    fs["uniquenessRatio"] >> uniquenessRatio;
    fs["speckleWindowSize"] >> speckleWindowSize;
    fs["speckleRange"] >> speckleRange;
    fs["modeSGBM"] >> modeSGBM;

    sgbm_state = cv::StereoSGBM::create(minDisparity,
                                        numDisparities,
                                        blockSize,
                                        P1,
                                        P2,
                                        disp12MaxDiff,
                                        preFilterCap,
                                        uniquenessRatio,
                                        speckleWindowSize,
                                        speckleRange,
                                        modeSGBM);

    fs.release();

    Utilities::messageDebug("Read file sucessfull.", false);
    return true;
}

void StereoMap::computeBMDisparity(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat left_mat, right_mat, left_disparity, right_disparity, filtered_disp;

    ///Gray
    if(src_left.type() != CV_8UC1)
    {
        cv::cvtColor(src_left, left_mat, CV_BGRA2GRAY);
    }
    else
    {
        src_left.copyTo(left_mat);
    }

    if(src_right.type() != CV_8UC1)
    {
        cv::cvtColor(src_right, right_mat, CV_BGRA2GRAY);
    }
    else
    {
        src_right.copyTo(right_mat);
    }

    using namespace ximgproc;
    Ptr<DisparityWLSFilter> wls_filter = createDisparityWLSFilter(bm_state);
    Ptr<StereoMatcher> right_matcher = createRightMatcher(bm_state);

    ///Disparity map
    bm_state-> compute(left_mat, right_mat,left_disparity);
    right_matcher->compute(left_mat, right_mat, right_disparity);

    ///Filter
    wls_filter->setLambda(LAMBDA);
    wls_filter->setSigmaColor(SIGMA);
    wls_filter->filter(left_disparity, left_mat, filtered_disp, right_disparity);

    cv::Mat invert(filtered_disp.rows, filtered_disp.cols, filtered_disp.type(), cvScalar(255, 255, 255, 255));
    cv::subtract(invert, filtered_disp, filtered_disp);

    filtered_disp.convertTo(out, CV_32F, 1./16);
}

void StereoMap::computeSGBMDisparity(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat left_mat, right_mat, left_disparity, right_disparity, filtered_disp;

    ///Gray
    if(src_left.type() != CV_8UC1)
    {
        cv::cvtColor(src_left, left_mat, CV_BGRA2GRAY);
    }
    else
    {
        src_left.copyTo(left_mat);
    }

    if(src_right.type() != CV_8UC1)
    {
        cv::cvtColor(src_right, right_mat, CV_BGRA2GRAY);
    }
    else
    {
        src_right.copyTo(right_mat);
    }
    using namespace ximgproc;
    Ptr<DisparityWLSFilter> wls_filter = createDisparityWLSFilter(sgbm_state);
    Ptr<StereoMatcher> right_matcher = createRightMatcher(sgbm_state);

    ///Disparity map
    sgbm_state-> compute(left_mat, right_mat,left_disparity);
    right_matcher->compute(left_mat,right_mat, right_disparity);

    ///Filter
    wls_filter->setLambda(LAMBDA);
    wls_filter->setSigmaColor(SIGMA);
    wls_filter->filter(left_disparity, left_mat, filtered_disp,right_disparity);

    cv::Mat invert(filtered_disp.rows, filtered_disp.cols, filtered_disp.type(), cvScalar(255, 255, 255, 255));
    cv::subtract(invert, filtered_disp, filtered_disp);

    filtered_disp.convertTo(out, CV_32F, 1./16);
}

void StereoMap::computeDepthMap(const cv::Mat &disparity, cv::Mat &Q, cv::Mat &depth_map, float depth_min, float depth_max)
{
    using namespace cv;

    if(Q.type() != CV_32F)
    {
        Q.convertTo(Q, CV_32F, 1./16);
    }

    //Extract depth
    depth_map = cv::Mat::zeros(disparity.rows, disparity.cols, CV_32FC1);

    //float Q03 = Q.at<float>(0, 3);
    //float Q13 = Q.at<float>(1, 3);
    float Q23 = Q.at<float>(2, 3);
    float Q32 = Q.at<float>(3, 2);
    float Q33 = Q.at<float>(3, 3);

    for (int i = 0; i < disparity.rows; i++)
    {
        const float* disp_ptr = disparity.ptr<float>(i);

        for (int j = 0; j < disparity.cols; j++)
        {
            const float pw = 1.0f / (disp_ptr[j] * Q32 + Q33);

            //- if invert
            float z = - Q23 * pw;
            if( z >= depth_min && z <= depth_max){
                depth_map.at<float>(i,j) = z;
            }
            else if (z > depth_max)
            {
                depth_map.at<float>(i,j) = depth_max+1;
            }
            else if (z < depth_min)
            {
                depth_map.at<float>(i,j) = depth_min-1;
            }
        }
    }
}

//////////////////////////////////

Calibration::StereoCamera::StereoCamera(std::string file_path)
{
    if(!load(file_path))
    {
        Utilities::messageDebug("Camera uncalibrated.");
    }
}

void Calibration::StereoCamera::calibrate(std::vector<Mat> &sources_images_left, std::vector<Mat> &sources_images_right)
{
    using namespace cv;

    if(sources_images_left.size() == 0 ||(sources_images_left.size() != sources_images_right.size()))
    {
        Utilities::messageDebug("Wrong number of images for calibration : "+std::to_string(sources_images_left.size())+" . "+std::to_string(sources_images_right.size()));
        return;
    }

    img_size = sources_images_left[0].size();

    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> left_img_points;
    std::vector<std::vector<cv::Point2f>> right_img_points;

    Utilities::messageDebug( "Starting chessboard stereo calibration.", false);

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
    int flag_find = CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FILTER_QUADS|  CALIB_CB_FAST_CHECK;
    int nb_rejected=0;
    Mat gray_l, gray_r;
    for(int i =0; i<sources_images_left.size() && i<sources_images_right.size(); i++)
    {
        Mat source_l = sources_images_left.at(i);
        Mat source_r = sources_images_right.at(i);

        cv::cvtColor(source_l, gray_l, CV_BGRA2GRAY);
        cv::cvtColor(source_r, gray_r, CV_BGRA2GRAY);

        bool found1 = findChessboardCorners(gray_l, board_size, corners_l, flag_find);
        bool found2 = findChessboardCorners(gray_r, board_size, corners_r, flag_find);


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
    Utilities::messageDebug( std::to_string(nb_rejected) + " images rejected.", false);

    //Calibration
    camera_matrix_l = cv::Mat(3, 3, CV_64F);
    dist_coeffs_l = cv::Mat::zeros(8, 1, CV_64F);

    //Not used later
    std::vector<cv::Mat> rvecs_l;
    std::vector<cv::Mat> tvecs_l;
    camera_matrix_l.ptr<float>(0)[0] = 1;
    camera_matrix_l.ptr<float>(1)[1] = 1;

    camera_matrix_r = cv::Mat(3, 3, CV_64F);
    dist_coeffs_r = cv::Mat::zeros(8, 1, CV_64F);

    //Not used later
    std::vector<cv::Mat> rvecs_r;
    std::vector<cv::Mat> tvecs_r;
    camera_matrix_r.ptr<float>(0)[0] = 1;
    camera_matrix_r.ptr<float>(1)[1] = 1;

    double rmserror1 = calibrateCamera(object_points, left_img_points, img_size, camera_matrix_l, dist_coeffs_l, rvecs_l, tvecs_l);
    Utilities::messageDebug( "Calibration finish with left :" + std::to_string(rmserror1) + " of error.", false);
    double rmserror2 = calibrateCamera(object_points, right_img_points, img_size, camera_matrix_r, dist_coeffs_r, rvecs_r, tvecs_r);
    Utilities::messageDebug( "And right :" + std::to_string(rmserror2) + " of error.", false);


    Utilities::messageDebug("Starting Calibration.",false);
    Utilities::messageDebug("Read intrasic...",false);

    Utilities::messageDebug( "Starting stereo calibration.", false);

    int flag = CV_CALIB_FIX_INTRINSIC;

    Utilities::messageDebug("Stereo calibrate...", false);
    double rmserror = stereoCalibrate(object_points, left_img_points, right_img_points, camera_matrix_l, dist_coeffs_l,
                                      camera_matrix_r, dist_coeffs_r, img_size, R, T, E, F, flag);
    Utilities::messageDebug( "Calibration finish with " + std::to_string(rmserror) + " of error.", false);

    Utilities::messageDebug("Starting Rectification.", false);
    stereoRectify(camera_matrix_l, dist_coeffs_l, camera_matrix_r, dist_coeffs_r, img_size, R, T, R1, R2, P1, P2, Q, 0, 0);
    Utilities::messageDebug("Done Rectification.", false);

    save();
}

bool Calibration::StereoCamera::save()
{
    using namespace cv;

    Utilities::messageDebug( "Starting save calib camera: " +file_path, false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::WRITE);
    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for write calibration.", true);
        return false;
    }

    time_t rawtime; time(&rawtime);
    std::string date =  asctime(localtime(&rawtime));;
    fs << "calibration_date" << date;
    fs << "width" << img_size.width;
    fs << "height" << img_size.height;
    fs << "camera_matrix_l" << camera_matrix_l;
    fs << "dist_coeffs_l" << dist_coeffs_l;
    fs << "camera_matrix_r" << camera_matrix_r;
    fs << "dist_coeffs_r" << dist_coeffs_r;
    fs << "R" << R;
    fs << "F" << F;
    fs << "E" << E;
    fs << "T" << T;
    fs << "R1" << R1;
    fs << "R2" << R2;
    fs << "P1" << P1;
    fs << "P2" << P2;
    fs << "Q" << Q;

    fs.release();

    Utilities::messageDebug("File done. Date : " + date, false);
    return true;
}

bool Calibration::StereoCamera::load()
{
    using namespace cv;

    Utilities::messageDebug( "Starting load calibration camera: " +file_path, false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);
    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for read calibration.", true);
        return false;
    }

    std::string date;
    fs["calibration_date"] >> date;
    Utilities::messageDebug("Calibration date : " +  date, false);

    fs["width"] >> img_size.width;
    fs["height"] >> img_size.height;
    fs["camera_matrix_l"] >> camera_matrix_l;
    fs["dist_coeffs_l"] >> dist_coeffs_l;
    fs["camera_matrix_r"] >> camera_matrix_r;
    fs["dist_coeffs_r"] >> dist_coeffs_r;
    fs["R"] >> R;
    fs["F"] >> F;
    fs["E"] >> E;
    fs["T"] >> T;
    fs["R1"] >> R1;
    fs["R2"] >> R2;
    fs["P1"] >> P1;
    fs["P2"] >> P2;
    fs["Q"] >> Q;

    fs.release();

    Utilities::messageDebug("Read camera file sucessfull.", false);
    return true;
}

bool Calibration::StereoCamera::load(std::string file_path)
{
    this->file_path = file_path;
    return load();
}





}


