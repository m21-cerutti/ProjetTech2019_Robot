#ifndef IA_H
#define IA_H

#define REFRESH 30

#include "tools/projectutilities.h"

#include "tools/imagefilter.h"
#include "tools/cameracalibration.h"
#include "tools/videoextractor.h"
#include "tools/stereomap.h"
//#include "tools/trackerobjectstereo.h"

#ifdef IS_COMPUTER
#include "tools/cvqtinterface.h"
#include "subWindows/sgbmparamdialog.h"
#include "subWindows/bmparamdialog.h"
#include "subWindows/cameraparamdialog.h"
#endif

#define START_DISTANCE 200
#define EPSILON_START 40
#define PERCENT_PRESENCE 0.25
#define DISTANCE_REFRESH (EPSILON_START/4)

#define CUBE_START_X1 0.25
#define CUBE_START_Y1 0
#define CUBE_START_X2 0.75
#define CUBE_START_Y2 0.5

#define NB_FRAME_INIT 10

#define MOVE_Y_SPEED_DIV 2

class IARobot
{
public:

    enum State
    {
        Wait,
        Move,
    };

    IARobot();

    virtual ~IARobot();

    void InitMaster();

    void setNewImages(const cv::Mat& frame_left, const cv::Mat& frame_right);

    float rotate();

    float moveY();

    float moveX();

protected:

    int nb_frame_init;
    State step;

    //cm
    float depth_master;

    int size_width, size_height;

    cv:: Mat Q,
    camera_matrix_l, dist_coeffs_l,
    camera_matrix_r, dist_coeffs_r;

    cv::Mat img_left, img_right;

    cv::Ptr<cv::StereoBM> bm_state;

    //TrackerObjectStereo tracker;
};

#endif // IA_H
