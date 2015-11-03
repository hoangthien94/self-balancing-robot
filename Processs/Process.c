/*
 * Process.c
 *
 *  Created on: Oct 27, 2015
 *      Author:
 */


#include  "../include.h"
#include "Process.h"

#define 	_DEBUG_QEI
//#define     DEBUG_POS
//#define 	_DEBUG_SPEED
//#define     _CONFIG_PID
#define     _CONFIG_PID_SPEED


//**********************************************
//
//	@Global Variable:	Kalman
//
//**********************************************
extern int16_t AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ;
extern double accelRoll, accelPitch;
extern double gyroRoll, gyroPitch;
extern double compRoll, compPitch;
extern double kalRoll, kalPitch, kalYaw = 0;
extern Kalman RollKalman, PitchKalman;
extern bool KalmanStarted;

//**********************************************
//
//	@Global Variable:	Encoder
//
//**********************************************
extern int32_t EncLeft = 0, EncRight = 0;
float test=20.0f;


//**********************************************
//
//	@Private Variable
//
//**********************************************
PIDType PIDPosLeft = {.Enable = 1,.Kp = 0.000002, .Ki = 0.0000, . Kd =0.000 };
PIDType PIDPosRight;
PIDType PIDSpeedLeft = {.Enable = 1, .Kp = 0.001, .Ki =0.00005,.Kd = 0.0001};
PIDType PIDSpeedRight;
int32_t speedLeft,speedRight;
int32_t PIDPosLeftResult = 0, PIDPosRightResult, PIDSpeedLeftResult = 0, PIDSpeedRightResult;

//**********************************************
//
//!  brief:     read data from mpu6050 and calulate it
//!
//!  function   :  CalulateData
//!  Description:
//!
//!  \return     :
//
//**********************************************

void CalulateData()
{
    if (KalmanStarted)
    {
//      mpu6050_Read_AccelXYZ(&AccelX, &AccelY, &AccelZ);
//      mpu6050_Read_GyroXYZ(&GyroX, &GyroY, &GyroZ);
        mpu6050_Read_All(&AccelX, &AccelY, &AccelZ,&GyroX, &GyroY, &GyroZ);
        double roll  = atan2(AccelY, AccelZ) * RAD_TO_DEG;
        double pitch = atan(-AccelX / sqrt(AccelY * AccelY + AccelZ * AccelZ)) * RAD_TO_DEG;
        double gyroXrate = GyroX / 131.0; // Convert to deg/s
        double gyroYrate = GyroY / 131.0; // Convert to deg/s

        // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
        if ((roll < -90 && kalRoll > 90) || (roll > 90 && kalRoll < -90))
        {
            setAngle(&RollKalman,roll);
            compRoll = roll;
            kalRoll = roll;
            gyroRoll = roll;
        }
        else
            kalRoll = getAngle(&RollKalman, roll, gyroXrate, KALMAN_PERIOD); // Calculate the angle using a Kalman filter

        if (abs(kalRoll) > 90)
            gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading

        kalPitch = getAngle(&PitchKalman, pitch, gyroYrate, KALMAN_PERIOD);

        gyroRoll += gyroXrate * KALMAN_PERIOD; // Calculate gyro angle without any filter
        gyroPitch += gyroYrate * KALMAN_PERIOD;
        //gyroRoll += getRate(RollKalman) * KALMAN_PERIOD; // Calculate gyro angle using the unbiased rate
        //gyroPitch += getRate(PitchKalman) * KALMAN_PERIOD;

        compRoll = 0.9 * (compRoll + gyroXrate * KALMAN_PERIOD) + 0.1 * roll; // Calculate the angle using a Complimentary filter
        compPitch = 0.9 * (compPitch + gyroYrate * KALMAN_PERIOD) + 0.1 * pitch;

        // Reset the gyro angle when it has drifted too much
        if (gyroRoll < -180 || gyroRoll > 180)
            gyroRoll = kalRoll;
        if (gyroPitch < -180 || gyroPitch > 180)
            gyroPitch = kalPitch;

//      Chuyển đổi dữ liệu giữa VĐK với Processing:
//      tiva truyền Uart kiểu này:
//      "roll,pitch,yaw!", sổ x 1000 (/1000 bên Processing)
//      ví dụ: 0.16234,0.20715,15.6253 --> 1620,2070,15625!

//      UARTPut_int32(UART0_BASE,kalPitch);
//      UARTPut_int32(UART0_BASE,compPitch);
#ifdef  DEBUG
        int temp = kalRoll * 1000;
        UARTPutNum(UART0_BASE, temp);
        UARTprintf(",");
        temp = kalPitch * 1000;
        UARTPutNum(UART0_BASE, temp);
        UARTprintf(",");
        temp = kalYaw * 1000;
        UARTPutNum(UART0_BASE, temp);
        UARTprintf("!\0");
#endif
    }
}

//**********************************************
//
//!  brief		:  Process balacing
//!
//!  function   :  Balacing_Process
//!  Description:
//!
//!  \return     :
//
//**********************************************

void Balacing_Process()
{
	if (getControlFlag())
	{
	    setControlFlag(false);
	    CalulateData();

#ifdef  _CONFIG_PID
	    configPID( &PIDSpeedLeft);
#endif

		qei_getVelocity(1, &speedLeft);
		qei_getVelocity(0, &speedRight);
#ifdef _DEBUG_QEI
		UARTPrint_num(speedLeft);
#endif

#ifdef  _CONFIG_PID_SPEED
        //set   speed 300 to test
        PIDSpeedSet(&PIDSpeedLeft, 200);
		PIDVerCalc(&PIDSpeedLeft, &speedLeft, 600);
		setSpeedMotor(MOTOR2, (int32_t)(PIDSpeedLeft.PIDResult));
#else
		EncLeft += speedLeft;
		PIDPosCalc( &PIDPosLeft, EncLeft, 20000);
		PIDPosLeftResult = (int32_t)( PIDPosLeft.PIDResult);
		PIDSpeedSet(&PIDSpeedLeft, PIDPosLeftResult);
		PIDVerCalc(&PIDSpeedLeft, &speedLeft, 600);
		setSpeedMotor(MOTOR2, (int32_t)(PIDSpeedLeft.PIDResult));
#endif
#ifdef  DEBUG_POS
		UARTPrint_num(EncLeft);
#endif
	}

}
