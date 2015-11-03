//*****************************************************************************
//
// Raise Your Arm 2013_ Micro Mouse robot.
//
// MYUART.h - Prototypes for InterruptHandler functions.
//
// This is part of revision 1.2 of the RYA Micro Mouse Library.
//      Happy coding.
//           Support Team RYA!
//*****************************************************************************
#ifndef PID_H_
#define PID_H_

//*****************************************************************************
//
// Structure of all PID type: speed, position, wall...
//
//*****************************************************************************
typedef struct
{
		double Kp;
		double Ki;
		double Kd;
		double pPart;
		double iPart;
		double dPart;
		int32_t SetPoint;
		double PIDResult;
		double PIDResultTemp;
		int32_t PIDError;
		int32_t PIDErrorTemp1;
		int32_t PIDErrorTemp2;
		uint8_t Enable;
} PIDType;

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern  void PIDSpeedSet(PIDType *p_PIDVer, int32_t SpeedSet);
extern  void PIDPositionSet(PIDType *p_PID, int32_t SetPoint);
extern  void PIDPosCalc(PIDType *p_PIDPos, int32_t Position, int32_t MaxResponse);
extern  void PIDVerCalc(PIDType *p_PIDVer, int32_t *Speed, int32_t MaxResponse);
extern  bool configPID(PIDType* _PID );

#endif /* PID_H_ */
