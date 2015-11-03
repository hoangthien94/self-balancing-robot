//*****************************************************************************
//
// Raise Your Arm 2013_ Micro Mouse robot.
//
// PID.c - PID calculator
//
// This is part of revision 1.2 of the RYA Micro Mouse Library.
//      Happy coding.
//           Support Team RYA!
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup PID_api
//! @{
//
//*****************************************************************************
#include "../include.h"

#define  SIZE_RX        9

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************

uint8_t     recv[SIZE_RX];
//*****************************************************************************
//
//! Write speed set point to PID structure.
//!
//!
//! \param *p_PIDVer is the pointer of PID structure (velocity left or right).
//! \param SpeedSet is the speed set point.
//!
//! \return None.
//
//*****************************************************************************
void PIDSpeedSet(PIDType *p_PIDVer, int32_t SpeedSet)
{
    (*p_PIDVer).Enable = 1;
    (*p_PIDVer).SetPoint = SpeedSet;
    (*p_PIDVer).iPart = 0;
}

//*****************************************************************************
//
//! Write Position set point to PID structure
//!
//!
//! \param *p_PID is the pointer of PID structure (position left or right).
//! \param SetPoint is the position set point.
//!
//! \return None.
//
//*****************************************************************************
void PIDPositionSet(PIDType *p_PID, int32_t SetPoint)
{
    (*p_PID).Enable = 1;
    (*p_PID).iPart = 0;
    (*p_PID).PIDErrorTemp1 = 0;
    (*p_PID).SetPoint = SetPoint;
}

//*****************************************************************************
//
//! PID Position Calculate control signal (.result) base on set point and real value
//! with Kp Ki Kd constant.
//!
//! \param *p_PIDPos is the pointer of PID structure (position left or right).
//! \param Position is the real position.
//! \param MaxResponse is the maximum result of PID.
//!
//! \return None.
//
//*****************************************************************************
void PIDPosCalc(PIDType *p_PIDPos, int32_t Position, int32_t MaxResponse)
{
    (*p_PIDPos).PIDError = (*p_PIDPos).SetPoint - Position;
    (*p_PIDPos).pPart = (*p_PIDPos).Kp * (*p_PIDPos).PIDError;
    (*p_PIDPos).iPart += (*p_PIDPos).Ki * (*p_PIDPos).PIDError;
    (*p_PIDPos).dPart = (*p_PIDPos).Kd * ((*p_PIDPos).PIDError - (*p_PIDPos).PIDErrorTemp1);
    /*
    //Uncomment to enable iPart-limit
    if ((*p_PIDPos).iPart > 40)
        (*p_PIDPos).iPart = 40;
    else if ((*p_PIDPos).iPart < -40)
        (*p_PIDPos).iPart = -40;
    */
    (*p_PIDPos).PIDResult = (*p_PIDPos).pPart + (*p_PIDPos).iPart + (*p_PIDPos).dPart;
    if ((*p_PIDPos).PIDResult > MaxResponse)
        (*p_PIDPos).PIDResult = (double)(MaxResponse);
    if ((*p_PIDPos).PIDResult < -1 * MaxResponse)
        (*p_PIDPos).PIDResult = (double)(-1 * MaxResponse);
}

//*****************************************************************************
//
//! PID velocity (of motor) Calculate control signal (.result) base on set
//! point and real value with Kp Ki Kd constant.
//!
//! \param *p_PIDVer is the pointer of PID structure (velocity left or right).
//! \param *Speed is the real speed of robot measurement by encoder
//! (EncRightCount or EncLeftCount)
//! \param MaxResponse is the maximum Duty Cycle.
//!
//! \return None.
//
//*****************************************************************************
void PIDVerCalc(PIDType *p_PIDVer, int32_t *Speed, int32_t MaxResponse)
{
    (*p_PIDVer).PIDError = (*p_PIDVer).SetPoint - (*Speed);
    *Speed = 0;

    (*p_PIDVer).pPart = (*p_PIDVer).Kp * (*p_PIDVer).PIDError;
    (*p_PIDVer).iPart += (*p_PIDVer).Ki * (*p_PIDVer).PIDError;
    (*p_PIDVer).dPart = (*p_PIDVer).Kd * ((*p_PIDVer).PIDError - (*p_PIDVer).PIDErrorTemp1);

    (*p_PIDVer).PIDResult += ((*p_PIDVer).pPart + (*p_PIDVer).iPart + (*p_PIDVer).dPart) ;

    if ((*p_PIDVer).PIDResult > MaxResponse)
        (*p_PIDVer).PIDResult = (double)MaxResponse;
    if ((*p_PIDVer).PIDResult < -1 * MaxResponse)
        (*p_PIDVer).PIDResult = (double)(-1 * (MaxResponse));
    (*p_PIDVer).PIDErrorTemp1 = (*p_PIDVer).PIDError;
}

//*****************************************************************************
//
//! Make robot move a little bit by set some pulses to two encoder.
//!
//! \param PositionLeft is pulse of left encoder.
//! \param PositionRight is pulse of right encoder.
//! \return None.
//
//*****************************************************************************



bool configPID(PIDType* _PID )
{
    uint16_t size,i;
    uint32_t itemp;
    float ftemp;
    size =  GetRxData(recv, SIZE_RX);
    if ( size != SIZE_RX)
    {
        while( GetRxData(recv, 1));
        return 0;
    }
#ifdef  DEBUG_RECV
    for (i = 0; i < SIZE_RX; i++ )
    {
        UARTCharPut(UART0_BASE,recv[i]);
    }
#endif
    for (i = 2; i < SIZE_RX; i++ )
    {
        recv[i] -= 0x30;
    }
    itemp    = recv[2] * 100000 + recv[3] * 10000 + \
            recv[4] * 1000 + recv[5] *100 + recv[6] *10 + recv[7];
    ftemp = (float)(itemp) / 100000;
    switch(recv[1])
    {
        case 'P':
            _PID->Kp = ftemp;
            break;
        case 'D':
            _PID->Kd = ftemp;
            break;
        case 'I':
            _PID->Ki = ftemp;
            break;
        case 'S':           // set speed point
            _PID->SetPoint = itemp;
            break;
        default:
            break;
    }
    return 1;
}
