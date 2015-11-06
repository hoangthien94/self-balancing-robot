#include "Timer.h"


#define     USE_TIMER2


//**********************************************
//
//  @Global Variable
//
//**********************************************


//**********************************************
//
//  @Private Function
//
//**********************************************

//**********************************************
//
//  @Private Variable
//
//**********************************************


static bool ControlFlag;


//**********************************************
//
//!  brief:     Timer init for control
//!
//!  function   :  timerInit
//!  Description:
//!  \param :  ui32Period.
//!
//!  \return     :
//
//**********************************************
void timerInit(uint32_t ms_Timebase)
{
#ifdef  USE_TIMER2
    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER2);
    //CONFIG TIMER2
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP );
    TimerLoadSet(TIMER2_BASE, TIMER_A, ROM_SysCtlClockGet() * ms_Timebase/ 1000);
    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerIntRegister(TIMER2_BASE,TIMER_A,&Timer2AIntHandler);
    TimerEnable(TIMER2_BASE, TIMER_A);
#endif
}
//**********************************************
//
//!  brief:      ISR timer control, read data from i2c ( mpu6050, kalman filter)
//!
//!  function   :  Timer2AIntHandler
//!  Description:
//!
//!  \return     :
//
//**********************************************
void Timer2AIntHandler(void)
{
    TimerIntClear(TIMER2_BASE,TimerIntStatus(TIMER2_BASE,true));
    ControlFlag = true;

}
//**********************************************
//
//!  brief:     Get control flag to process
//!
//!  function   :  getControlFlag
//!  Description:
//!
//!  \return     :  control flag
//
//**********************************************
bool getControlFlag()
{
    return  ControlFlag;
}

//**********************************************
//
//!  brief:     Set control flag when finish process
//!
//!  function   :  setControlFlag
//!  Description:
//!  \param :  flag.
//!
//!  \return     :
//
//**********************************************
void setControlFlag(bool flag)
{
    ControlFlag = flag;
}


