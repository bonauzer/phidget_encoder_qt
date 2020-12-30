#include "phidgetRead.h"


phidgetRead::phidgetRead()
{
    pPhidget = new Phidget();
    init();
}

phidgetRead::~phidgetRead()
{
    delete pPhidget;
}


int phidgetRead::init(){

    for (int i=0; i<4; i++) encoderVal[i] = 0;

    if(!pPhidget->Init())
        return -1;

    return 0;
}



void phidgetRead::UpdateStates()
{
    encoderVal[0]    = static_cast<double>(pPhidget->me[0].nPos[0])*TIC_TO_ANGLE;
    encoderVal[1]    = static_cast<double>(pPhidget->me[0].nPos[1])*TIC_TO_ANGLE;
    encoderVal[2]    = static_cast<double>(pPhidget->me[0].nPos[2])*TIC_TO_ANGLE;
    encoderVal[3]    = static_cast<double>(pPhidget->me[0].nPos[3])*TIC_TO_ANGLE;
//    qDebug("%f, %f, %f, %f", encoderVal[0], encoderVal[1], encoderVal[2], encoderVal[3]);

}
