#ifndef IA_H
#define IA_H

#define REFRESH 30

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
};

#endif // IA_H
