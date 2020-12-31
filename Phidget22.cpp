#include "Phidget22.h"
#include "mainwindow.h"
//#include "Definitions.h"
#include <stdio.h>
#include <cstring>

//static int CCONV AttachHandler(CPhidgetHandle h, void *userptr);
//static int CCONV DetachHandler(CPhidgetHandle h, void *userptr);
//static int CCONV ErrorHandler(CPhidgetHandle h, void *userptr, int ErrorCode, const char *Description);
//static int CCONV InputChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int State);
//static int CCONV PositionChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int Time, int RelativePosition);
//static int CCONV InputChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
//static int CCONV OutputChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
//static int CCONV SensorChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value);
//static int CCONV PhidgetInterfaceKit_getSensorRawValue (CPhidgetInterfaceKitHandle IFK, int Index, int RawValue);
//static void CCONV onVoltageChange(CPhidgetInterfaceKitHandle ch, void * ctx, double voltage);


static void CCONV onPositionChange(PhidgetEncoderHandle ch, void * ctx, int positionChange, double timeChange, int indexTriggered);
static void CCONV onAttach(PhidgetHandle ch, void * ctx);
static void CCONV onDetach(PhidgetHandle ch, void * ctx);
static void CCONV onError(PhidgetHandle ch, void * ctx, Phidget_ErrorEventCode code, const char * description);
static void CCONV onVoltageChange(PhidgetVoltageInputHandle ch, void * ctx, double voltage);

Phidget22* phdg;
int getValue[8];

//*/////////////////////////////////////////////
//*/////////// Phidget Board ///////////////////
//*/////////////////////////////////////////////

Phidget22::Phidget22() {

    phdg = this;
}

Phidget22::~Phidget22() {

    if (!bClosed) Close();

    for(int i=0; i<NUM_BOARD_ENC; i++) {
        for(int j=0; j<numEncPort; j++) {
            PhidgetEncoder_delete(&(me[i].hEncoder[j]));
        }
    }
    
    for(int i=0; i<NUM_BOARD_INT; i++) {
        for(int j=0; j<numEncPort; j++) {
            PhidgetVoltageInput_delete(&(mi[i].hIFBoard[j]));
        }
    }
}

bool Phidget22::Init()
{
    //Declare an encoder handle
//    if (pWnd->left_master_ID == 2 || pWnd->right_master_ID == 2) {
//        #undef NUM_BOARD_ENC
//        #undef MAX_BOARD
//        #define NUM_BOARD_ENC 3    // number of phidget encoder board  (encoder, clutch)
//        #define MAX_BOARD (NUM_BOARD_ENC+NUM_BOARD_INT)
//    }
    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        me[i].hEncoder[0] = NULL;
        me[i].hEncoder[1] = NULL;
        me[i].hEncoder[2] = NULL;
        me[i].hEncoder[3] = NULL;

        me[i].nPos[0] = 0;
        me[i].nPos[1] = 0;
        me[i].nPos[2] = 0;
        me[i].nPos[3] = 0;

        me[i].nDIO[0] = 0;
        me[i].nDIO[1] = 0;
        me[i].nDIO[2] = 0;
        me[i].nDIO[3] = 0;

        me[i].nID = 99999;

        me[i].nSerialNo = meSerial[i];
    }

    for(int i=0; i<NUM_BOARD_INT; i++)
    {
        mi[i].hIFBoard[0] = NULL;
        mi[i].hIFBoard[1] = NULL;
        mi[i].hIFBoard[2] = NULL;
        mi[i].hIFBoard[3] = NULL;
        mi[i].hIFBoard[4] = NULL;
        mi[i].hIFBoard[5] = NULL;
        mi[i].hIFBoard[6] = NULL;
        mi[i].hIFBoard[7] = NULL;

        mi[i].nADC[0] = 0.0;
        mi[i].nADC[1] = 0.0;
        mi[i].nADC[2] = 0.0;
        mi[i].nADC[3] = 0.0;
        mi[i].nADC[4] = 0.0;
        mi[i].nADC[5] = 0.0;
        mi[i].nADC[6] = 0.0;
        mi[i].nADC[7] = 0.0;
        mi[i].nID = 99999;

        mi[i].nSerialNo = miSerial[i];
    }

//    me[0].nSerialNo = 344213; // Left - Yaw, Pitch, Yaw2, Rotation
//    me[1].nSerialNo = 485427; // Right - Yaw, Pitch, Yaw2, Rotation
//    me[2].nSerialNo = 344112; // Left, Right - Translation
//    mi[0].nSerialNo = 443239; // Left, Right - grasper (ADC)

//    me[0].nSerialNo = 507142; // Bo phidget encoder board serial num
//    mi[0].nSerialNo = 443254; // Bo phidget interfase board serial num

    int result;

//    PhidgetReturnCode ret;


    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        //create the encoder object
        for(int j=0; j<numEncPort; j++) {
            PhidgetEncoder_create(&(me[i].hEncoder[j]));

            //Set addressing parameters to specify which channel to open (if any)
            ret = Phidget_setDeviceSerialNumber((PhidgetHandle)me[i].hEncoder[j], me[i].nSerialNo);
            if (ret != EPHIDGET_OK) {
                Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                printf("Error (%d): %s", errorCode, errorString);
                exit(1);
            }
            ret = Phidget_setChannel((PhidgetHandle)me[i].hEncoder[j], j);
            if (ret != EPHIDGET_OK) {
                Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                printf("Error (%d): %s", errorCode, errorString);
                exit(1);
            }
        }

        //Assign any event handlers you need before calling open so that no events are missed.
        for(int j=0; j<numEncPort; j++) {
            PhidgetEncoder_setOnPositionChangeHandler(me[i].hEncoder[j], onPositionChange, NULL);
            Phidget_setOnAttachHandler((PhidgetHandle)me[i].hEncoder[j], onAttach, NULL);
            Phidget_setOnDetachHandler((PhidgetHandle)me[i].hEncoder[j], onDetach, NULL);
            Phidget_setOnErrorHandler((PhidgetHandle)me[i].hEncoder[j], onError, NULL);
        }

        //Open your Phidgets and wait for attachment
        for(int j=0; j<numEncPort; j++) {
            ret = Phidget_openWaitForAttachment((PhidgetHandle)me[i].hEncoder[j], 5000);
            if (ret != EPHIDGET_OK) {
                Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                printf("Error (%d): %s", errorCode, errorString);
                exit(1);
            }
        }

    }

    for(int i=0; i<NUM_BOARD_INT; i++)
    {
        for(int j=0; j<numIfkPort; j++) {
            PhidgetVoltageInput_create(&(mi[i].hIFBoard[j]));

            //Set addressing parameters to specify which channel to open (if any)
            ret = Phidget_setDeviceSerialNumber((PhidgetHandle)mi[i].hIFBoard[j], mi[i].nSerialNo);
            if (ret != EPHIDGET_OK) {
                Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                printf("Error (%d): %s", errorCode, errorString);
                exit(1);
            }
            ret = Phidget_setChannel((PhidgetHandle)mi[i].hIFBoard[j], j);
            if (ret != EPHIDGET_OK) {
                Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                printf("Error (%d): %s", errorCode, errorString);
                exit(1);
            }
        }

        for(int j=0; j<numIfkPort; j++) {
            PhidgetVoltageInput_setOnVoltageChangeHandler(mi[i].hIFBoard[j], onVoltageChange, NULL);
            Phidget_setOnAttachHandler((PhidgetHandle)mi[i].hIFBoard[j], onAttach, NULL);
            Phidget_setOnDetachHandler((PhidgetHandle)mi[i].hIFBoard[j], onDetach, NULL);
            Phidget_setOnErrorHandler((PhidgetHandle)mi[i].hIFBoard[j], onError, NULL);
        }

        //Open your Phidgets and wait for attachment
        for(int j=0; j<numIfkPort; j++) {
            ret = Phidget_openWaitForAttachment((PhidgetHandle)mi[i].hIFBoard[j], 5000);
            if (ret != EPHIDGET_OK) {
                Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                printf("Error (%d): %s", errorCode, errorString);
                exit(1);
            }
        }
    }

    bClosed = false;

    return true;
}

void Phidget22::Close() {

    if (!bClosed) {
        for(int i=0; i<NUM_BOARD_ENC; i++) {
            for(int j=0; j<numEncPort; j++) {
                ret = Phidget_close((PhidgetHandle)me[i].hEncoder[j]);
                if (ret != EPHIDGET_OK) {
                    Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                    printf("Error (%d): %s", errorCode, errorString);
                    exit(1);
                }
            }
        }
        
        for(int i=0; i<NUM_BOARD_INT; i++) {
//            CPhidget_close((CPhidgetHandle)ifkit[i]);
            for(int j=0; j<numIfkPort; j++) {
                ret = Phidget_close((PhidgetHandle)mi[i].hIFBoard[j]);
                if (ret != EPHIDGET_OK) {
                    Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
                    printf("Error (%d): %s", errorCode, errorString);
                    exit(1);
                }
            }
        }
        bClosed = true;
    }
}

// TODO: for foot pedal

//int CCONV InputChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int State)
//{
//    for(int i=0; i<NUM_BOARD_ENC; i++)
//    {
//        if(ENC == phdg->me[i].hBoard)
//        {
//            switch(Index)
//            {
//            case 0:  // left foot clutch
//                if (State == 0)
//                    phdg->me[1].nDIO[0] = false;
//                else if (State ==1)
//                    phdg->me[1].nDIO[0] = true;
//                break;
//            case 1: // right foot clutch
//                if (State == 0)
//                    phdg->me[1].nDIO[1] = false;
//                else if (State ==1)
//                    phdg->me[1].nDIO[1] = true;
//                break;
//            }
//        }
//    }

//    return 0;
//}


static void CCONV onPositionChange(PhidgetEncoderHandle ch, void * ctx, int positionChange, double timeChange, int indexTriggered) {
    PhidgetReturnCode ret;
    PhidgetReturnCode errorCode;
    const char * errorString;
    char errorDetail[100];
    size_t errorDetailLen = 100;
    int channel;

    //Getting the channel number to distinguish between Phidgets
    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        ret = Phidget_getChannel((PhidgetHandle)ch, &channel);
        if (ret != EPHIDGET_OK) {
            Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
            printf("Error (%d): %s", errorCode, errorString);
            exit(1);
        }

        phdg->me[i].nPos[channel] = positionChange; // indexTriggered OR channel?

//        printf("PositionChange [%d]: %d\n", channel, positionChange);
//        printf("TimeChange [%d]: %lf\n", channel, timeChange);
//        printf("IndexTriggered [%d]: %d\n", channel, indexTriggered);
//        printf("----------\n");
    }
}

static void CCONV onAttach(PhidgetHandle ch, void * ctx) {
    PhidgetReturnCode ret;
    PhidgetReturnCode errorCode;
    const char * errorString;
    char errorDetail[100];
    size_t errorDetailLen = 100;
    int channel;

    //Getting the channel number to distinguish between Phidgets
    ret = Phidget_getChannel(ch, &channel);
    if (ret != EPHIDGET_OK) {
        Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
        printf("Error (%d): %s", errorCode, errorString);
        exit(1);
    }
//    printf("Attach [%d]!", channel);
}

static void CCONV onDetach(PhidgetHandle ch, void * ctx) {
    PhidgetReturnCode ret;
    PhidgetReturnCode errorCode;
    const char * errorString;
    char errorDetail[100];
    size_t errorDetailLen = 100;
    int channel;

    //Getting the channel number to distinguish between Phidgets
    ret = Phidget_getChannel(ch, &channel);
    if (ret != EPHIDGET_OK) {
        Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
        printf("Error (%d): %s", errorCode, errorString);
        exit(1);
    }
//    printf("Detach [%d]!", channel);
}

static void CCONV onError(PhidgetHandle ch, void * ctx, Phidget_ErrorEventCode code, const char * description) {
    PhidgetReturnCode ret;
    PhidgetReturnCode errorCode;
    const char * errorString;
    char errorDetail[100];
    size_t errorDetailLen = 100;
    int channel;

    //Getting the channel number to distinguish between Phidgets
    ret = Phidget_getChannel(ch, &channel);
    if (ret != EPHIDGET_OK) {
        Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
        printf("Error (%d): %s", errorCode, errorString);
        exit(1);
    }
    printf("Description [%d]: %s\n", channel, description);
    printf("----------\n");
}

static void CCONV onVoltageChange(PhidgetVoltageInputHandle ch, void * ctx, double voltage) {
    PhidgetReturnCode ret;
    PhidgetReturnCode errorCode;
    const char * errorString;
    char errorDetail[100];
    size_t errorDetailLen = 100;
    int channel;

    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        //Getting the channel number to distinguish between Phidgets
        ret = Phidget_getChannel((PhidgetHandle)ch, &channel);
        if (ret != EPHIDGET_OK) {
            Phidget_getLastError(&errorCode, &errorString, errorDetail, &errorDetailLen);
            printf("Error (%d): %s", errorCode, errorString);
            exit(1);
        }
        phdg->mi[i].nADC[channel] = voltage; // OR channel?
//        printf("Voltage [%d]: %lf\n", channel, voltage);
    }
}
