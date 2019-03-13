#ifndef TRACKEROBJECT_H
#define TRACKEROBJECT_H


#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include "tools/projectutilities.h"

#define BOX_X 0.25
#define BOX_Y 0
#define BOX_WIDTH 0.5
#define BOX_HEIGHT 0.5

class TrackerObjectStereo
{
public:

    TrackerObjectStereo();

    bool updateTracker(cv::Mat depth_frame);

    cv::Mat getNewDirection(float pixel_min_move);

private:

    cv::Ptr<cv::Tracker> initTracker(cv::Mat depth_frame);

private:

    bool is_initialised;

    cv::Rect2d old_rect;
    cv::Rect2d rect;

    cv::Ptr<cv::Tracker> tracker;

};

#endif // TRACKEROBJECT_H
