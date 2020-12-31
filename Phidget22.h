#ifndef Phidget22_HMH
#define Phidget22_HMH

#include "main.h"
#include "phidget22.h"

// Phidget

#define NUM_BOARD_ENC          1    // number of phidget encoder board  (encoder, clutch)
#define NUM_BOARD_INT          1    // number of phidget interface board (ADC, grasper)
#define MAX_BOARD              (NUM_BOARD_ENC+NUM_BOARD_INT)

struct enc_module
{
    int nID;
    int nSerialNo;
    int nPos[4];
    int nDIO[4];
    PhidgetEncoderHandle hEncoder[4];
};

struct interface_module
{
    int nID;
    int nSerialNo;
    double nADC[8]; // bo FIXED
    PhidgetVoltageInputHandle hIFBoard[8];
};

class Phidget22
{
public:
    Phidget22();
    ~Phidget22();

    bool Init();
    void Close();

    PhidgetReturnCode ret;
    PhidgetReturnCode errorCode;
    const char *errorString;
    char errorDetail[100];
    size_t errorDetailLen = 100;

    int numEncPort = 4;
    int numIfkPort = 8;

    int meSerial[NUM_BOARD_ENC] = {507142};
    int miSerial[NUM_BOARD_INT] = {443254};

    enc_module me[NUM_BOARD_ENC];
    interface_module mi[NUM_BOARD_INT];

private:
    bool bClosed = false;

};

#endif
