#include "phidgetRead.h"


phidgetRead::phidgetRead()
{
    pPhidget = new Phidget22();
    init();
}

phidgetRead::~phidgetRead()
{
    delete pPhidget;
}


int phidgetRead::init(){

    for (int i=0; i<4; i++) encoderVal[i] = 0;
    for (int i=0; i<8; i++) interfaceVal[i] = 0;

    if(!pPhidget->Init())
        return -1;

    return 0;
}



void phidgetRead::UpdateStates()
{
    for (int i=0; i<4; i++) encoderVal[i] = static_cast<double>(pPhidget->me[0].nPos[i])*TIC_TO_ANGLE;
    for (int i=0; i<8; i++) interfaceVal[i] = static_cast<double>(pPhidget->mi[0].nADC[i]);

//    qDebug("%f, %f, %f, %f", encoderVal[0], encoderVal[1], encoderVal[2], encoderVal[3]);
//    qDebug("%f, %f, %f, %f", interfaceVal[4], interfaceVal[5], interfaceVal[6], interfaceVal[7]);

}
