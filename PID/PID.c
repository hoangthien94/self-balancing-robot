#include "../include.h"
#include "PID.h"

#define     SIZE_RX 8
uint8_t recv[SIZE_RX];


extern PIDType PIDVelocity ={.Enable = 1,.Kp =0.0011,.Ki = 0.00001, .Kd=0.06,.MaxSetpoint =500, .PIDErrorTemp1 = 0};
extern PIDType PIDPosition  ={.Enable = 1,.Kp =0.005,.Ki= 0.05, .Kd=0.001,.MaxSetpoint = 50000};

#define     DEBUG_TEST_CONFIG_PID
//#define     DEBUG_RECV

int32_t RisePulse, FallPulse;
extern int32_t Position;

void PIDSpeedSet(int32_t SpeedSet)
{
	PIDVelocity.Enable = 1;
	PIDVelocity.SetPoint = SpeedSet;
}

void PIDPositionSet(int32_t SetPoint)
{
	PIDPosition.Enable = 1;
	PIDPosition.iPart = 0;
	PIDPosition.PIDErrorTemp1 = 0;
	PIDPosition.SetPoint += SetPoint;
	PIDVelocity.iPart = 0;
//	ROM_QEIPositionSet(QEI0_BASE, 0);
//	Position += SetPoint;
}

void PIDPosCalc(int32_t Position, int32_t MaxResponse)
{
	PIDPosition.PIDError = PIDPosition.SetPoint - Position;
	PIDPosition.pPart = PIDPosition.Kp * PIDPosition.PIDError;
	PIDPosition.iPart += PIDPosition.Ki * PIDPosition.PIDError;
	PIDPosition.dPart = PIDPosition.Kd * (PIDPosition.PIDError - PIDPosition.PIDErrorTemp1);

//	Uncomment to enable iPart-limit
	if (PIDPosition.iPart > 15)
		PIDPosition.iPart = 15;
	else if (PIDPosition.iPart < -15)
		PIDPosition.iPart = -15;

	PIDPosition.PIDResult = PIDPosition.pPart + PIDPosition.iPart + PIDPosition.dPart;
	if (PIDPosition.PIDResult > MaxResponse)
		PIDPosition.PIDResult = (float)(MaxResponse);
	if (PIDPosition.PIDResult < -1 * MaxResponse)
		PIDPosition.PIDResult = (float)(-1 * MaxResponse);
	PIDPosition.PIDErrorTemp1 = PIDPosition.PIDError;
}

//MaxResponse: Max Duty Cycle
void PIDVerCalc(int32_t Speed, int32_t MaxResponse)
{
	PIDVelocity.PIDError = PIDVelocity.SetPoint - Speed;

	PIDVelocity.pPart = PIDVelocity.Kp * PIDVelocity.PIDError;
	PIDVelocity.iPart += PIDVelocity.Ki * PIDVelocity.PIDError;
	PIDVelocity.dPart = PIDVelocity.Kd * (PIDVelocity.PIDError - PIDVelocity.PIDErrorTemp1);
/*
	if (PIDVelocity.iPart > 40)
		PIDVelocity.iPart = 40;
	else if (PIDVelocity.iPart < -40)
		PIDVelocity.iPart = -40;
*/
	PIDVelocity.PIDResult += (PIDVelocity.pPart + PIDVelocity.iPart + PIDVelocity.dPart) ;

	if (PIDVelocity.PIDResult > MaxResponse)
		PIDVelocity.PIDResult = (float)MaxResponse;
	if (PIDVelocity.PIDResult < -1 * MaxResponse)
		PIDVelocity.PIDResult = (float)(-1 * (MaxResponse));
	PIDVelocity.PIDErrorTemp1 = PIDVelocity.PIDError;
}
void PIDReset(PIDType* pid_parameter)
{
	pid_parameter->PIDError = 0;
	pid_parameter->PIDErrorTemp1 = 0;
	pid_parameter->PIDErrorTemp2 = 0;
	pid_parameter->PIDResult = 0;
	pid_parameter->PIDResultTemp = 0;
}
float getPIDVer()
{
	return PIDVelocity.PIDResult;
}
float getPIDPos()
{
	return PIDPosition.PIDResult;
}

bool configPID()
{
    uint16_t size;
    size =  GetRxData(recv, SIZE_RX);
    if ( size != SIZE_RX)
    {
        while( GetRxData(recv, 1));
        return 0;
    }
    uint32_t temp;
#ifdef  DEBUG_RECV
    for (size = 0; size < SIZE_RX; size++ )
    {
        UARTCharPut(UART0_BASE,recv[size]);
    }
#endif
    for (size = 2; size < SIZE_RX; size++ )
    {
        recv[size] -= 0x30;
    }
    temp    = recv[2] * 100000 + recv[3] * 10000 + \
            recv[4] * 1000 + recv[5] *100 + recv[6] *10 + recv[7];
    switch( recv[0])
    {
        case 'V':
        {
            switch(recv[1])
            {
                case 'p':
#ifdef DEBUG_TEST_CONFIG_PID
    LED3_SWITCH();
#endif
                    PIDVelocity.Kp = temp / 100000;
                    break;

            }
        }
            break;
        case 'P':
            break;
        default:
            break;
            //return 0;       // not PID
    }
    return 1;
}

