#include "tools/files.h"


bool Files::getMatrixCalibrationFileStorage(const std::string file_path, const std::string name_parameter, cv::Mat &out)
{
    using namespace cv;

    Utilities::messageDebug( "Starting load matrix in file: " +file_path, false);

    FileStorage fs(file_path, FileStorage::Mode::FORMAT_XML|FileStorage::Mode::READ);
    if(!fs.isOpened()) {
        Utilities::messageDebug("Can't open file for read the parameter of calibration.", true);
        return false;
    }

    std::string date;
    fs["calibration_date"] >> date;
    Utilities::messageDebug("Calibration date : " +  date, false);

    fs[name_parameter] >> out;
    fs.release();

    Utilities::messageDebug("Read file parameter sucessfull.", false);
    return true;
}

bool Files::saveSetImages(const std::string prefix_folder, const std::vector<cv::Mat> images_left, const std::vector<cv::Mat> images_right)
{
    using namespace cv;

    std::string folder_cmd = "mkdir -p "+prefix_folder;
    if (std::system(folder_cmd.c_str()) == 0)
    {

        Utilities::messageDebug("Create folder done.", false);
    }
    else
    {
        Utilities::messageDebug("Error create folder.", true);
    }

    Utilities::messageDebug( "Starting save set camera: " +prefix_folder, false);

    if(images_left.size() != images_right.size())
    {
        Utilities::messageDebug("Not the same number of images.", true);
        return false;
    }

    for(int i =0; i < images_left.size(); i++ )
    {
        imwrite(prefix_folder+"/"+std::to_string(i)+"_left.png", images_left[i]);
        imwrite(prefix_folder+"/"+std::to_string(i)+"_right.png", images_right[i]);
    }

    time_t rawtime; time(&rawtime);
    std::string date =  asctime(localtime(&rawtime));
    Utilities::messageDebug("Set done. Date : " + date, false);
    return true;
}

bool Files::loadIntrinsicCamera(const std::string file_path,
                                       cv::Size &img_size,
                                       cv::Mat& camera_matrix,
                                       cv::Mat& dist_coeffs,
                                       std::vector<cv::Mat>& rvecs,
                                       std::vector<cv::Mat>& tvecs)
{
    using namespace cv;

    Utilities::messageDebug( "Starting load intrinsic camera: " +file_path, false);

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
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;
    fs["rvecs"] >> rvecs;
    fs["tvecs"] >> tvecs;
    fs.release();

    Utilities::messageDebug("Read camera file sucessfull.", false);
    return true;
}

bool Files::loadIntrinsicCamera(const std::string file_path, cv::Size &img_size, cv::Mat &camera_matrix, cv::Mat &dist_coeffs)
{
    std::vector<cv::Mat> rvecs, tvecs;
    return loadIntrinsicCamera(file_path, img_size, camera_matrix, dist_coeffs, rvecs, tvecs);
}

bool Files::saveIntrinsicCamera(const std::string file_path,
                                       const cv::Size img_size,
                                       const cv::Mat& camera_matrix,
                                       const  cv::Mat& dist_coeffs,
                                       const std::vector<cv::Mat>& rvecs,
                                       const std::vector<cv::Mat>& tvecs)
{
    using namespace cv;

    Utilities::messageDebug( "Starting save intrinsic camera: " +file_path, false);

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
    fs << "camera_matrix" << camera_matrix;
    fs << "dist_coeffs" << dist_coeffs;
    fs << "rvecs" << rvecs;
    fs << "tvecs" << tvecs;
    fs.release();

    Utilities::messageDebug("File done. Date : " + date, false);
    return true;
}

bool Files::loadCameraStereoParameters(const std::string file_path,
                                              cv::Mat &camera_matrix_l,
                                              cv::Mat &dist_coeffs_l,
                                              cv::Mat &camera_matrix_r,
                                              cv::Mat &dist_coeffs_r,
                                              cv::Size &img_size,
                                              cv::Mat &R,
                                              cv::Mat &F,
                                              cv::Mat &E,
                                              cv::Vec3d &T,
                                              cv::Mat &R1,
                                              cv::Mat &R2,
                                              cv::Mat &P1,
                                              cv::Mat &P2,
                                              cv::Mat &Q)
{
    using namespace cv;

    Utilities::messageDebug( "Starting load intrinsic camera : " +file_path, false);

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

bool Files::saveCameraStereoParameters(const std::string file_path,
                                              const cv::Mat &camera_matrix_l,
                                              const cv::Mat &dist_coeffs_l,
                                              const cv::Mat &camera_matrix_r,
                                              const cv::Mat &dist_coeffs_r,
                                              const cv::Size &img_size,
                                              const cv::Mat &R,
                                              const cv::Mat &F,
                                              const cv::Mat &E,
                                              const cv::Vec3d &T,
                                              const cv::Mat &R1,
                                              const cv::Mat &R2,
                                              const cv::Mat &P1,
                                              const cv::Mat &P2,
                                              const cv::Mat &Q)
{
    using namespace cv;

    Utilities::messageDebug( "Starting stereo save extrinsic : " +file_path, false);

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
