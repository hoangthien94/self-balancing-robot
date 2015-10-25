#include "../include.h"

extern int32_t Position;
PIDType PIDVelocity, PIDPosition;
int32_t RisePulse, FallPulse;

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
