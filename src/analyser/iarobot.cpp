#include "analyser/iarobot.h"

IARobot::IARobot()
{
    step = IARobot::Wait;
}

IARobot::~IARobot()
{

}

void IARobot::InitMaster()
{
    if(nb_frame_init > NB_FRAME_INIT)
    {
        step = IARobot::Move;
        nb_frame_init = 0;
        return;
    }

    cv::Mat disparity, depth_map;
    StereoMap::computeBMDisparityStereo(img_left, img_right, disparity, bm_state);
    StereoMap::computeDepthMap(disparity, Q, depth_map, (START_DISTANCE - EPSILON_START), (START_DISTANCE + EPSILON_START));

    float average = 0;
    int pixel_interest = 0;

    for(int i = depth_map.rows * CUBE_START_X1; i < depth_map.rows * CUBE_START_X2; i++) {
        for(int j = depth_map.cols * CUBE_START_Y1; j < depth_map.cols * CUBE_START_Y2; j++)
        {
            float z = depth_map.at<float>(i,j);
            if( z >= (START_DISTANCE - EPSILON_START) && z <= (START_DISTANCE + EPSILON_START)){
                average+=z;
                depth_map.at<float>(i,j) = z;
                pixel_interest++;
            }
        }
    }
    average/=pixel_interest;

    if((pixel_interest/(depth_map.rows * (CUBE_START_X2-CUBE_START_X1) * depth_map.cols * (CUBE_START_Y2-CUBE_START_Y1)* 1.0)) < PERCENT_PRESENCE)
    {
        return;
    }
    nb_frame_init ++;
}

void IARobot::setNewImages(const cv::Mat &frame_left, const cv::Mat &frame_right)
{
    this->img_left = frame_left;
    this->img_right = frame_right;
}

float IARobot::rotate()
{

}

float IARobot::moveY()
{
    /*
    if (step == IARobot::Calibration)
    {
        return 0;
    }

    if(tracker.updateTracker(img_left))
    {
       cv::Mat vect = tracker.getNewDirection(100);
       float y =vect.at<float>(1);
       if( y > 0)
       {
           return y;
       }
    }
    else
    {
        ProjectUtilities::messageDebug( "Master lost.", true);
    }
    return 0;
    */
}

float IARobot::moveX()
{
    if (step == IARobot::Wait)
    {
        InitMaster();
        return 0;
    }

    cv::Mat disparity, depth_map;
    StereoMap::computeBMDisparityStereo(img_left, img_right, disparity, bm_state);
    StereoMap::computeDepthMap(disparity, Q, depth_map, (START_DISTANCE - EPSILON_START), (START_DISTANCE + EPSILON_START));

    float average = 0;
    int pixel_interest = 0;
    for(int i = 0; i < depth_map.rows; i++) {
        for(int j = 0; j < depth_map.cols; j++) {
            float z = depth_map.at<float>(i,j);
            if( z >= (START_DISTANCE - EPSILON_START) && z <= (START_DISTANCE + EPSILON_START)){
                average+=z;
                depth_map.at<float>(i,j) = z;
                pixel_interest++;
            }
        }
    }
    average/=pixel_interest;

    if((pixel_interest/(depth_map.rows * depth_map.cols * 1.0)) < PERCENT_PRESENCE)
    {
        step = IARobot::Wait;
        return 0;
    }

    float move_y = START_DISTANCE - average;

    if(DISTANCE_REFRESH> move_y)
        return 0;

    return move_y / MOVE_Y_SPEED_DIV;
}
