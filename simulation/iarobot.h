#ifndef IA_H
#define IA_H

#define REFRESH 30

#include "custom_controller.h"


using namespace cerutti;

class IARobot
{
public:

    IARobot();

    virtual ~IARobot();

    void simulate();
};

#endif // IA_H
