#ifndef	_MOTOR_CONTROL_H_
#define	_MOTOR_CONTROL_H_

#include <stdint.h>

typedef	enum
{
	MOTOR1,
	MOTOR2,
}	MOTOR;

extern void setSpeedMotor(MOTOR typeMotor, int32_t speed);
extern	void setMotor(MOTOR typeMotor, int32_t speed);
extern	void ProcessSpeedControl();

#endif	// end of	_MOTOR_CONTROL_H_
