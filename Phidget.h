#ifndef Phidget_HMH
#define Phidget_HMH

#include "HD/hd.h"
#include "HDU/hduError.h"
#include "HDU/hduVector.h"
#include "HDU/hduMatrix.h"
#include "main.h"
#include "phidget21.h"

// Phidget
#define TIC_TO_ANGLE_GRASPER         (PI/3/130)

#define NUM_BOARD_ENC          1    // number of phidget encoder board  (encoder, clutch)
#define NUM_BOARD_INT          1    // number of phidget interface board (ADC, grasper)
#define MAX_BOARD              (NUM_BOARD_ENC+NUM_BOARD_INT)

struct enc_module
{
    int nID;
    int nSerialNo;
    int nPos[4];
    int nDIO[4];
    CPhidgetEncoderHandle hBoard;
};

struct interface_module
{
    int nID;
    int nSerialNo;
    int nADC[8];
    CPhidgetInterfaceKitHandle hBoard;
};

class Phidget
{
public:
    Phidget();
    ~Phidget();

    bool Init();
    void Close();

    enc_module me[NUM_BOARD_ENC];
    interface_module mi[NUM_BOARD_INT];

private:
    CPhidgetEncoderHandle enc[NUM_BOARD_ENC] = {0,};
    CPhidgetInterfaceKitHandle ifkit[NUM_BOARD_INT] = {0,};
    bool bClosed = false;

};

#endif
