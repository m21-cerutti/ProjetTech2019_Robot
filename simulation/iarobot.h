#ifndef IA_H
#define IA_H

#include "windows/bmparamdialog.h"
#include "windows/cameraparamdialog.h"
#include "windows/sgbmparamdialog.h"
#include "windows/singleimagedialog.h"


#define REFRESH (0.5*1000)

#include "custom_controller.h"

using namespace cerutti;

class IARobot : CustomController
{
public:

    IARobot();

    virtual ~IARobot();

    /**
     * @brief simulate call process of custom_controller
     *
     * @param left_img the left stereo image.
     * @param right_img the right stereo image.
     */
    void simulate(const Mat & left_img, const Mat & right_img);

    float vx, vy, omega;

};

#endif // IA_H
