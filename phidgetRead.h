#ifndef PHIDGETREAD_H
#define PHIDGETREAD_H

#include "Phidget.h"
#include "main.h"

#define TIC_TO_ANGLE            (2*PI/(360.0*4.0))

class phidgetRead
{
public:
    phidgetRead();
    ~phidgetRead();

public:
    void	UpdateStates();
    int init();
    Phidget*    pPhidget ;

    double encoderVal[4];

};

#endif // PHIDGETREAD_H
