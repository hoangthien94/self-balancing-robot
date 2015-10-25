#ifndef PID_H_
#define PID_H_

typedef struct
{
		float Kp;
		float Ki;
		float Kd;
		float pPart;
		float iPart;
		float dPart;
		int32_t SetPoint;
		float PIDResult;
		float PIDResultTemp;
		int32_t PIDError;
		int32_t PIDErrorTemp1;
		int32_t PIDErrorTemp2;
		uint8_t Enable;
		int32_t MaxSetpoint;
} PIDType;

void PIDSpeedSet(int32_t SpeedSet);
void PIDPositionSet(int32_t SetPoint);
void PIDPosCalc(int32_t Position, int32_t MaxResponse);
void PIDVerCalc(int32_t Speed, int32_t MaxResponse);

#endif /* PID_H_ */
