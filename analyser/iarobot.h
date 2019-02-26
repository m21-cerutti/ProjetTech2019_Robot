#ifndef IA_H
#define IA_H

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
