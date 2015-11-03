#ifndef _TIMER_H_
#define _TIMER_H_


#include "../include.h"

extern void timerInit(uint32_t ms_Timebase);
void Timer2AIntHandler(void);
bool getControlFlag();
void setControlFlag(bool flag);
void TimerMPUInterrupt();
#endif /* SEFT_BACLANCING_ROBOT_CONFIGTIMER_H_ */
