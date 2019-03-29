#ifndef IA_H
#define IA_H

#define REFRESH 30

#include "custom_controller.h"

#include "tools/cvqtinterface.h"
#include "subWindows/sgbmparamdialog.h"
#include "subWindows/bmparamdialog.h"
#include "subWindows/cameraparamdialog.h"


using namespace cerutti;

class IARobot
{
public:

    IARobot();

    virtual ~IARobot();

    void simulate();
};

#endif // IA_H
