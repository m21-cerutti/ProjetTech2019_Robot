#pragma once

#include "controller.h"

#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

namespace cerutti
{

#define NAME_FILE_CONFIG "robot.xml"
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

class CustomController : public Controller
{
public:

  CustomController();

  virtual ~CustomController();

  virtual void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega) override;

  virtual void load() override;

private:
  int nb_frame_init;

  //cm
  float depth_master;

  int size_width, size_height;

  cv:: Mat Q, R1, P1,
  camera_matrix_l, dist_coeffs_l,
  camera_matrix_r, dist_coeffs_r;

  cv::Mat img_left, img_right;

  cv::Ptr<cv::StereoBM> bm_state;

};

}
