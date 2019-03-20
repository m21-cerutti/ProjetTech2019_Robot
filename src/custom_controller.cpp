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
    //setNewImages(left_img, right_img);

    //cv::normalize(image_3d, image_3d, 0, 255, CV_MINMAX, CV_8UC1);
    //cv::threshold( image_3d, image_3d, 0, 1, CV_THRESH_TRUNC );


    /*
     * initUndistortRectifyMap(left_cam_matrix, left_dist_coeffs, R1, P1, frame_size,CV_32FC1, left_undist_rect_map_x, left_undist_rect_map_y);
initUndistortRectifyMap(right_cam_matrix, right_dist_coeffs, R2, P2, frame_size, CV_32FC1, right_undist_rect_map_x, right_undist_rect_map_y);
cv::remap(left_frame, left_undist_rect, left_undist_rect_map_x, left_undist_rect_map_y, CV_INTER_CUBIC, BORDER_CONSTANT, 0);
cv::remap(right_frame, right_undist_rect, right_undist_rect_map_x, right_undist_rect_map_y, CV_INTER_CUBIC, BORDER_CONSTANT, 0);

 */


    // Just keep static
    *vx = 0;
    *vy = 0;
    *omega = 0;

}

void CustomController::load()
{
    using namespace cv;
    /*
    ProjectUtilities::messageDebug( "Starting load robot file.", false);

    FileStorage fs(NAME_FILE_CONFIG, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);
    if(!fs.isOpened()) {
        ProjectUtilities::messageDebug("Can't open file for robot.", true);
        return;
    }

    std::string date;
    fs["calibration_date"] >> date;
    ProjectUtilities::messageDebug("File creation date : " +  date, false);

    fs["width"] >> size_width;
    fs["height"] >> size_height;

    fs["camera_matrix_l"] >> camera_matrix_l;
    fs["dist_coeffs_l"] >> dist_coeffs_l;
    fs["camera_matrix_r"] >> camera_matrix_r;
    fs["dist_coeffs_r"] >> dist_coeffs_r;

    fs["Q"] >> Q;

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

    cv::Ptr<cv::StereoBM> bm_state = cv::StereoBM::create();
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

    fs.release();

    ProjectUtilities::messageDebug("Read file sucessfull.", false);
    */
}

}
