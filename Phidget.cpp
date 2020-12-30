#include "Phidget.h"
#include "mainwindow.h"
//#include "Definitions.h"
#include <stdio.h>
#include <cstring>

static int CCONV AttachHandler(CPhidgetHandle h, void *userptr);
static int CCONV DetachHandler(CPhidgetHandle h, void *userptr);
static int CCONV ErrorHandler(CPhidgetHandle h, void *userptr, int ErrorCode, const char *Description);
static int CCONV InputChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int State);
static int CCONV PositionChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int Time, int RelativePosition);
static int CCONV InputChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
static int CCONV OutputChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
static int CCONV SensorChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value);

Phidget* phdg;

//*/////////////////////////////////////////////
//*/////////// Phidget Board ///////////////////
//*/////////////////////////////////////////////

Phidget::Phidget() {

    phdg = this;
}

Phidget::~Phidget() {

    if (!bClosed) Close();

    for(int i=0; i<NUM_BOARD_ENC; i++) {
        CPhidget_delete((CPhidgetHandle)enc[i]);
    }
    
    for(int i=0; i<NUM_BOARD_INT; i++) {
        CPhidget_delete((CPhidgetHandle)ifkit[i]);
    }
}

bool Phidget::Init()
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
        me[i].hBoard = NULL;
        me[i].nPos[0] = 0;
        me[i].nPos[1] = 0;
        me[i].nPos[2] = 0;
        me[i].nPos[3] = 0;
        me[i].nDIO[0] = 0;
        me[i].nDIO[1] = 0;
        me[i].nDIO[2] = 0;
        me[i].nDIO[3] = 0;
        me[i].nID = 99999;
    }

    for(int i=0; i<NUM_BOARD_INT; i++)
    {
        mi[i].hBoard = NULL;
        mi[i].nADC[0] = 0;
        mi[i].nADC[1] = 0;
        mi[i].nADC[2] = 0;
        mi[i].nADC[3] = 0;
        mi[i].nADC[4] = 0;
        mi[i].nADC[5] = 0;
        mi[i].nADC[6] = 0;
        mi[i].nADC[7] = 0;
        mi[i].nID = 99999;
    }

//    me[0].nSerialNo = 344213; // Left - Yaw, Pitch, Yaw2, Rotation
//    me[1].nSerialNo = 485427; // Right - Yaw, Pitch, Yaw2, Rotation
//    me[2].nSerialNo = 344112; // Left, Right - Translation
//    mi[0].nSerialNo = 443239; // Left, Right - grasper (ADC)

    me[0].nSerialNo = 507142; // Bo phidget board serial

    int result;
    const char *err;

    

    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        //create the encoder object
        CPhidgetEncoder_create(&enc[i]);

        //Set the handlers to be run when the device is plugged in or opened from software,
        //unplugged or closed from software, or generates an error.
        CPhidget_set_OnAttach_Handler((CPhidgetHandle)enc[i], AttachHandler, NULL);
        CPhidget_set_OnDetach_Handler((CPhidgetHandle)enc[i], DetachHandler, NULL);
        CPhidget_set_OnError_Handler((CPhidgetHandle)enc[i], ErrorHandler, NULL);

        //Registers a callback that will run if an input changes.
        //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
        CPhidgetEncoder_set_OnInputChange_Handler(enc[i], InputChangeHandler_Encoder, NULL);

        //Registers a callback that will run if the encoder changes.
        //Requires the handle for the Encoder, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
        CPhidgetEncoder_set_OnPositionChange_Handler (enc[i], PositionChangeHandler_Encoder, NULL);

        CPhidget_open((CPhidgetHandle)enc[i], -1);

        //get the program to wait for an encoder device to be attached
        if((result = CPhidget_waitForAttachment((CPhidgetHandle)enc[i], 10000)))
        {
            CPhidget_getErrorDescription(result, &err);
            return false;
        }
    }

    for(int i=0; i<NUM_BOARD_INT; i++)
    {
        //create the InterfaceKit object
        CPhidgetInterfaceKit_create(&ifkit[i]);

        //Set the handlers to be run when the device is plugged in or opened from software,
        //unplugged or closed from software, or generates an error.
        CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifkit[i], AttachHandler, NULL);
        CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifkit[i], DetachHandler, NULL);
        CPhidget_set_OnError_Handler((CPhidgetHandle)ifkit[i], ErrorHandler, NULL);

        //Registers a callback that will run if an input changes.
        //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
        CPhidgetInterfaceKit_set_OnInputChange_Handler (ifkit[i], InputChangeHandler_InterfaceKit, NULL);

        //Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
        //Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
        CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifkit[i], SensorChangeHandler_InterfaceKit, NULL);

        //Registers a callback that will run if an output changes.
        //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
        CPhidgetInterfaceKit_set_OnOutputChange_Handler (ifkit[i], OutputChangeHandler_InterfaceKit, NULL);

        //open the interfacekit for device connections
        CPhidget_open((CPhidgetHandle)ifkit[i], -1);

        //get the program to wait for an interface kit device to be attached
        if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifkit[i], 10000)))
        {
            CPhidget_getErrorDescription(result, &err);
            return false;
        }
    }

    bClosed = false;

    return true;
}

void Phidget::Close() {

    if (!bClosed) {
        for(int i=0; i<NUM_BOARD_ENC; i++) {
            CPhidget_close((CPhidgetHandle)enc[i]);
        }
        
        for(int i=0; i<NUM_BOARD_INT; i++) {
            CPhidget_close((CPhidgetHandle)ifkit[i]);
        }
        bClosed = true;
    }
}

int CCONV AttachHandler(CPhidgetHandle h, void *userptr)
{
    int serialNo;
    CPhidget_DeviceID deviceID;
    int inputcount;

    CPhidget_getSerialNumber(h, &serialNo);

    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        if(serialNo == phdg->me[i].nSerialNo)
        {
            phdg->me[i].hBoard = (CPhidgetEncoderHandle)h;
            phdg->me[i].nID = i;

            //Retrieve the device ID and number of encoders so that we can set the enables if needed
            CPhidget_getDeviceID(h, &deviceID);
            CPhidgetEncoder_getEncoderCount((CPhidgetEncoderHandle)h, &inputcount);
            qDebug("Encoder %10d attached! \n", serialNo);

            //the 1047 requires enabling of the encoder inputs, so enable them if this is a 1047
            if (deviceID == PHIDID_ENCODER_HS_4ENCODER_4INPUT)
            {
                    //qDebug("Encoder requires Enable. Enabling inputs....\n");
                    for (i = 0 ; i < inputcount ; i++)
                            CPhidgetEncoder_setEnabled((CPhidgetEncoderHandle)h, i, 1);
            }
            break;
        }
    }

    for(int i=0; i<NUM_BOARD_INT; i++)
    {
        if(serialNo == phdg->mi[i].nSerialNo)
        {
            phdg->mi[i].hBoard = (CPhidgetInterfaceKitHandle)h;
            phdg->mi[i].nID = i;
            break;
        }
    }
    return 1;
}

int CCONV DetachHandler(CPhidgetHandle h, void *userptr)
{
    int serialNo;
    CPhidget_getSerialNumber(h, &serialNo);
    //qDebug("Encoder %10d detached! \n", serialNo);

    return 0;
}

int CCONV ErrorHandler(CPhidgetHandle h, void *userptr, int ErrorCode, const char *Description)
{
    //qDebug("Error handled. %d - %s \n", ErrorCode, Description);

    return 0;
}

int CCONV InputChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int State)
{
    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        if(ENC == phdg->me[i].hBoard)
        {
            switch(Index)
            {
            case 0:  // left foot clutch
                if (State == 0)
                    phdg->me[1].nDIO[0] = false;
                else if (State ==1)
                    phdg->me[1].nDIO[0] = true;
                break;
            case 1: // right foot clutch
                if (State == 0)
                    phdg->me[1].nDIO[1] = false;
                else if (State ==1)
                    phdg->me[1].nDIO[1] = true;
                break;
            }
        }
    }

    return 0;
}

int CCONV PositionChangeHandler_Encoder(CPhidgetEncoderHandle ENC, void *usrptr, int Index, int Time, int RelativePosition)
{
    for(int i=0; i<NUM_BOARD_ENC; i++)
    {
        if(ENC == phdg->me[i].hBoard)
        {
            int position;
            CPhidgetEncoder_getPosition(ENC, Index, &position);
            phdg->me[i].nPos[Index] = position;
            break;
        }
    }

    //qDebug("Encoder #%i - Position: %5d -- Relative Change %2d -- Elapsed Time: %5d \n", Index, Position, RelativePosition, Time);
    return 0;
}

//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int CCONV InputChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
    //printf("Digital Input: %d > State: %d\n", Index, State);
    return 0;
}

//callback that will run if an output changes.
//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
int CCONV OutputChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
    //printf("Digital Output: %d > State: %d\n", Index, State);
    return 0;
}

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int CCONV SensorChangeHandler_InterfaceKit(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
    //printf("Sensor: %d > Value: %d\n", Index, Value);
    for(int i=0; i<NUM_BOARD_INT; i++)    {
        if(IFK == phdg->mi[i].hBoard)        {
            phdg->mi[i].nADC[Index] = Value;
            break;
        }
    }
    return 0;
}

