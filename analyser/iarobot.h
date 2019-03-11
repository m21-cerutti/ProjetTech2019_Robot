#ifndef IA_H
#define IA_H

#define REFRESH 30

#include "tools/projectutilities.h"
#include "tools/cvqtinterface.h"
#include "tools/imagefilter.h"
#include "tools/cameracalibration.h"
#include "tools/videoextractor.h"
#include "subWindows/sgbmparamdialog.h"
#include "subWindows/bmparamdialog.h"
#include "subWindows/cameraparamdialog.h"

class IARobot
{
public:

    enum state
    {
        Wait,
        Calibration,
        Move,
    };

    IARobot();

    void update(cv::Mat& img_left, cv::Mat&  img_right);

    void onDisplay();

private:
    cv::Mat img_left, img_right;
};

#endif // IA_H
