#include "tools/trackerobjectstereo.h"


TrackerObjectStereo::TrackerObjectStereo()
{
    cv::Rect2d(BOX_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT);
    tracker = cv::Tracker::create("KCF");
}

cv::Ptr<cv::Tracker> TrackerObjectStereo::initTracker(cv::Mat depth_frame)
{
    tracker->init(depth_frame, rect);
    is_initialised = true;
    old_rect = cv::Rect(rect);
}

bool TrackerObjectStereo::updateTracker(cv::Mat depth_frame)
{
    if(!is_initialised)
    {
        initTracker(depth_frame);
    }
    return tracker->update(depth_frame, rect);
}

cv::Mat TrackerObjectStereo::getNewDirection(float pixel_min_move)
{
    if(!is_initialised)
    {
        ProjectUtilities::messageDebug("Tracker not initialised.", true);
        return cv::Mat();
    }

    cv::Mat vect(rect.br()-old_rect.br());
    double magn = cv::norm(vect);
    if((double)pixel_min_move > magn)
    {
        return cv::Mat();
    }
    old_rect = rect;
    return vect;
}

