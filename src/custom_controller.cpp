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

    Utilities::showMatrice("Normal_"+std::to_string(nb_frame), left_img);
    Utilities::showMatrice("Normal_"+std::to_string(nb_frame), right_img);

    Mat disparity, depth_map;

    Size image_size(size_width, size_height);

    Mat left_undist_rect_map_x,
        left_undist_rect_map_y,

         right_undist_rect_map_x,
         right_undist_rect_map_y,

         left_img_undist,
         right_img_undist
         ;

    //UNDISTORD
    initUndistortRectifyMap(camera_matrix_l, dist_coeffs_l, R1, P1, image_size, CV_32FC1, left_undist_rect_map_x, left_undist_rect_map_y);
    initUndistortRectifyMap(camera_matrix_r, dist_coeffs_r, R2, P2,  image_size, CV_32FC1, right_undist_rect_map_x, right_undist_rect_map_y);

    remap(left_img, left_img_undist, left_undist_rect_map_x, left_undist_rect_map_y, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar());
    remap(right_img, right_img_undist, right_undist_rect_map_x, right_undist_rect_map_y, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar());

    Utilities::showMatrice("Undistord_"+std::to_string(nb_frame), left_img_undist);
    Utilities::showMatrice("Undistord_"+std::to_string(nb_frame), right_img_undist);

    //DISPARITY
    StereoMap::computeBMDisparityStereo( left_img_undist, right_img_undist, disparity, bm_state);

    Utilities::showMatrice("Disparity_"+std::to_string(nb_frame), left_img_undist);

    //DEPTH
    StereoMap::computeDepthMap(disparity, Q, depth_map);
    threshold(depth_map, depth_map, -200, 0, CV_THRESH_TRUNC);

    Utilities::showMatrice("Depth_"+std::to_string(nb_frame), depth_map);

    //BLOB
    std::vector<KeyPoint> keypoints;
    blob_detector->detect( depth_map, keypoints);

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    Mat im_with_keypoints;
    drawKeypoints( depth_map, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

    Utilities::showMatrice("Blob_"+std::to_string(nb_frame), depth_map);

    // Just keep static
    *vx = 0;
    *vy = 0;
    *omega = 0;

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

    Ptr<StereoBM> bm_state = StereoBM::create();
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
    params.minThreshold = 10;
    params.maxThreshold = 200;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 800;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.1;

    // Filter by Convexity
    params.filterByConvexity = true;
    params.minConvexity = 0.87;

    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.01;

    // Set up detector with params
    Ptr<SimpleBlobDetector> blob_detector = SimpleBlobDetector::create(params);

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

    std::string folder_cmd = "mkdir -p ./DEBUG_IMAGES_CERUTTI/"+name;
    if (std::system(folder_cmd.c_str()) == 0)
    {

        Utilities::messageDebug("Create folder done.", false);
    }
    else
    {
        Utilities::messageDebug("Error create folder.", true);
    }

    Utilities::messageDebug( "Starting save image: " +name, false);

    time_t rawtime; time(&rawtime);
    std::string date =  asctime(localtime(&rawtime));
    imwrite("./DEBUG_IMAGES_CERUTTI/"+name+"_"+date, mat);

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



void StereoMap::computeBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
{
    cv::Mat left_mat, right_mat;

    ///Separate
    Filters::separateImage(src, left_mat, right_mat);

    computeBMDisparityStereo(left_mat, right_mat, out, bm_state);
}

void StereoMap::computeBMDisparityStereo(const cv::Mat &src_left, const cv::Mat &src_right, cv::Mat &out, cv::Ptr<cv::StereoBM> bm_state)
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
    bm_state->compute(left_mat, right_mat, disparity);
    cv::Mat invert(disparity.rows, disparity.cols, disparity.type(), cvScalar(255, 255, 255, 255));
    cv::subtract(invert, disparity, disparity);

    disparity.convertTo(disparity, CV_32F);


    disparity.copyTo(out);
}

void StereoMap::computeSGBMDisparity(const cv::Mat &src, cv::Mat &out, cv::Ptr<cv::StereoSGBM> sgbm_state)
{
    cv::Mat left_mat, right_mat;
    Filters::separateImage(src, left_mat, right_mat);

    computeSGBMDisparityStereo(left_mat, right_mat, out, sgbm_state);
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


