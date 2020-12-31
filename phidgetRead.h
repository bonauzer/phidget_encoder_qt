#ifndef PHIDGETREAD_H
#define PHIDGETREAD_H

#include "Phidget22.h"
#include "main.h"

#define TIC_TO_ANGLE            (2*PI/(360.0*4.0))
#define TIC_TO_ANGLE_GRASPER         (PI/3/130)

class phidgetRead
{
public:
    phidgetRead();
    ~phidgetRead();

public:
    void	UpdateStates();
    int init();
    Phidget22*    pPhidget ;

    double encoderVal[4];
    double interfaceVal[8];

};

#endif // PHIDGETREAD_H
