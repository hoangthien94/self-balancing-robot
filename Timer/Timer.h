/*
 * ConfigTimer.h
 *
 *  Created on: May 11, 2013
 *      Author: NHH
 */

#ifndef _TIMER_H_
#define _TIMER_H_

extern void timerInit(uint32_t ui32Period);
void Timer2AIntHandler(void);
bool getControlFlag();
void setControlFlag(bool flag);

#endif /* SEFT_BACLANCING_ROBOT_CONFIGTIMER_H_ */
