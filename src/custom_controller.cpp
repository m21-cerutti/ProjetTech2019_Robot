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
    setNewImages(left_img, right_img);

    // Just keep static
    *vx = moveX();
    *vy = 0;
    *omega = 0;

}

void CustomController::load()
{
    using namespace cv;

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

}

}
