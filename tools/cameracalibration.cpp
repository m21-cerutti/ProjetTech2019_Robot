#include "cameracalibration.h"


bool CameraCalibration::calibrateFromImages(std::vector<cv::Mat> &sources_images, cv::Mat &out)
{

    return false;
}

bool CameraCalibration::calibrateCamera(cv::Mat source, cv::Mat &out)
{
    /*
    cv::findChessboardCorners 	(
            cv::Mat	image, //Image source
            Size  	patternSize, //Nb corners
            vector<vector<Point2f>>  	corners,
            int  	flags = CALIB_CB_ADAPTIVE_THRESH+CALIB_CB_NORMALIZE_IMAGE
        )
    */

    /*
    double cv::calibrateCamera 	( 	vector<vector<Point3f>>  	objectPoints, //Theory, unit cann be arbitrary
    vector<vector<Point2f>>  	imagePoints, //Real, find chess
            Size  	imageSize, //Size board height x width
            InputOutputArray  	cameraMatrix,
            InputOutputArray  	distCoeffs,
            OutputArrayOfArrays  	rvecs,
            OutputArrayOfArrays  	tvecs,
            OutputArray  	stdDeviationsIntrinsics,
            OutputArray  	stdDeviationsExtrinsics,
            OutputArray  	perViewErrors,
            int  	flags = 0,
            TermCriteria  	criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON)
        )*/
    return false;
}
