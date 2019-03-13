#pragma once

#include "controller.h"
#include "analyser/iarobot.h"

namespace cerutti
{

#define NAME_FILE_CONFIG "robot.xml"

class CustomController : public Controller, public IARobot
{
public:

  CustomController();

  virtual ~CustomController();

  virtual void process(const cv::Mat & left_img,
                       const cv::Mat & right_img,
                       float * vx, float * vy, float * omega) override;

  virtual void load() override;

};

}
