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

    Utilities::showMatrice("Normal_l_"+std::to_string(nb_frame), left_img);
    Utilities::showMatrice("Normal_r_"+std::to_string(nb_frame), right_img);
    Utilities::messageDebug("Normal images save.", false);

    cv::Mat disparity, depth_map;

    Size image_size(size_width, size_height);

    Mat left_img_undist,
        right_img_undist
            ;

    //UNDISTORD
    undistort(left_img, left_img_undist, camera_matrix_l, dist_coeffs_l);
    undistort(right_img, right_img_undist, camera_matrix_r, dist_coeffs_r);

    Utilities::showMatrice("Undistord_l_"+std::to_string(nb_frame), left_img_undist);
    Utilities::showMatrice("Undistord_r_"+std::to_string(nb_frame), right_img_undist);
    Utilities::messageDebug("Undistord images save.", false);

    //DISPARITY
    StereoMap::computeBMDisparityStereo( left_img_undist, right_img_undist, disparity, bm_state);

    Utilities::showMatrice("Disparity_"+std::to_string(nb_frame), disparity);
    Utilities::messageDebug("Disparity images save.", false);

    //DEPTH
    StereoMap::computeDepthMap(disparity, Q, depth_map);
    threshold(depth_map, depth_map, -200, 0, CV_THRESH_TRUNC);

    Utilities::showMatrice("Depth_"+std::to_string(nb_frame), depth_map);
    Utilities::messageDebug("Depth images save.", false);

    //BLOB
    cvtColor( left_img_undist, left_img_undist,CV_BGR2GRAY);
    //normalize(left_img, left_img, 0, 255, NORM_MINMAX, CV_8UC1);
    std::vector<KeyPoint> keypoints;
    blob_detector->detect( left_img_undist, keypoints);

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob

    Mat im_with_keypoints;

    drawKeypoints( left_img_undist, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    Utilities::showMatrice("Blob_"+std::to_string(nb_frame), im_with_keypoints);
    Utilities::messageDebug("Blob images save.", false);

    // Just keep static
    if(nb_frame < 5)
    {
        *vx = 0;
        *vy = 0;
        *omega = 0;
    }
    else if(nb_frame < 10)
    {
        *vx = 1*MOVE_SPEED_MULT;
        *vy = 0;
        *omega = 0;
    }
    else if(nb_frame < 15)
    {
        *vx = 0;
        *vy = 1*MOVE_SPEED_MULT;
        *omega = 0;
    }
    else if(nb_frame < 20)
    {
        *vx = -1*MOVE_SPEED_MULT;
        *vy = 0;
        *omega = 0;
    }
    else if(nb_frame < 25)
    {
        *vx = 0;
        *vy = -1*MOVE_SPEED_MULT;
        *omega = 0;
    }
    else
    {
        *vx = 0;
        *vy = 0;
        *omega = 1*MOVE_SPEED_MULT;
    }


}

void CustomController::load()
{
    using namespace cv;

    Utilities::messageDebug( "Starting load robot file.", false);

    FileStorage fs(NAME_FILE_CONFIG, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);
    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for robot.", true);
        return;
    }

    std::string date;
    fs["calibration_date"] >> date;
    Utilities::messageDebug("File creation date : " +  date, false);

    fs["width"] >> size_width;
    fs["height"] >> size_height;

    fs["camera_matrix_l"] >> camera_matrix_l;
    fs["dist_coeffs_l"] >> dist_coeffs_l;
    fs["camera_matrix_r"] >> camera_matrix_r;
    fs["dist_coeffs_r"] >> dist_coeffs_r;

    fs["Q"] >> Q;
    fs["R1"] >> R1;
    fs["P1"] >> P1;
    fs["R2"] >> R2;
    fs["P2"] >> P2;

    //bgm parameters
    int preFilterCap;
    int PreFilterSize;
    int PreFilterType;
    int TextureThreshold;
    int uniquenessRatio;
    int blockSize;
    int disp12MaxDiff;
    int minDisparity;
    int numDisparities;

    bm_state = StereoBM::create();

    fs["preFilterCap"] >> preFilterCap;
    bm_state->setPreFilterCap(preFilterCap);
    fs["PreFilterSize"] >> PreFilterSize;
    bm_state->setPreFilterSize(PreFilterSize);
    fs["PreFilterType"] >> PreFilterType;
    bm_state->setPreFilterType(PreFilterType);
    fs["TextureThreshold"] >> TextureThreshold;
    bm_state->setTextureThreshold(TextureThreshold);
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

    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 50;
    params.maxThreshold = 1000;
    params.thresholdStep = 15;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 500;

    // Filter by Circularity
    params.filterByCircularity = false;
    //params.minCircularity = 0.1;

    // Filter by Convexity
        params.filterByConvexity = false;

    params.filterByColor = false;

    // Filter by Inertia
    params.filterByInertia = true;
    params.maxInertiaRatio = 0.3;

    // Set up detector with params
    blob_detector = SimpleBlobDetector::create(params);

    fs.release();

    Utilities::messageDebug("Read file sucessfull.", false);
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
        messageDebug("Matrice empty.", true);
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
    time_t rawtime; time(&rawtime);
    std::string date(asctime(localtime(&rawtime)));
    std::string folder = "DEBUG_IMAGES_CERUTTI";


    std::string folder_cmd = "mkdir -p \""+folder+"\"";
    if (std::system(folder_cmd.c_str()) == 0)
    {

        Utilities::messageDebug("Create folder done.", false);
    }
    else
    {
        Utilities::messageDebug("Error create folder.", true);
    }

    Utilities::messageDebug( "Starting save image: " +name, false);
    imwrite(folder+"/"+name+"_"+date+".png", mat);

    Utilities::messageDebug("Image saved.", false);

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

    computeGaussianBlur(gray, gray);

    /// Apply Laplace function
    cv::Laplacian( gray, dest, CV_16S, 3, 1, 1, cv::BORDER_DEFAULT );
    convertScaleAbs( dest, dest );
    dest.convertTo(out, CV_8UC1);
}

//////////////////////////////////


void StereoMap::computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat left_mat, right_mat, disparity;

    disparity =cv::Mat(src_left.size(), CV_32F, Scalar());
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

    ///Disparity map
    bm_state->compute(left_mat, right_mat, disparity);
    cv::Mat invert(disparity.rows, disparity.cols, disparity.type(), cvScalar(255, 255, 255, 255));
    cv::subtract(invert, disparity, disparity);

    disparity.convertTo(disparity, CV_32F);


    disparity.copyTo(out);
}

void StereoMap::computeSGBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat left_mat, right_mat, disparity;

    ///Gray
    if(src_left.type() != CV_8UC1)
    {
        cv::cvtColor(src_left, left_mat, CV_BGRA2GRAY);
    }
    if(src_right.type() != CV_8UC1)
    {
        cv::cvtColor(src_right, right_mat, CV_BGRA2GRAY);
    }

    ///Disparity map
    sgbm_state->compute(left_mat, right_mat, disparity);
    cv::Mat invert(disparity.rows, disparity.cols, disparity.type(), cvScalar(255, 255, 255, 255));
    cv::subtract(invert, disparity, disparity);

    disparity.convertTo(disparity, CV_32F);


    disparity.copyTo(out);
}

void StereoMap::computeDepthMap(const cv::Mat &disparity, const cv::Mat &Q, cv::Mat &depth_map)
{
    using namespace cv;
    Mat image_3d;
    reprojectImageTo3D(disparity, image_3d, Q, true, CV_32F);

    //Extract depth
    depth_map = cv::Mat::zeros(image_3d.rows, image_3d.cols, CV_32FC1);
    // image_3d[2] -> depth_map[0]
    int from_to[] = { 2,0 };
    mixChannels( &image_3d, 1, &depth_map, 1, from_to, 1 );

}

}


