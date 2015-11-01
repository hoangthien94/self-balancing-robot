#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

void ConfigPIDTimer(uint32_t TimerIntervalms, uint32_t PIDVerlocityLoop);
void ConfigSystem(void);
void ConfigPWM(void);
void ConfigNetwork(void);
void SetPWM(uint32_t ulFrequency, int32_t ucDutyCycle);
void ConfigEncoder(void);
void ConfigBattSense(void);
void Config_Button1(void);
void Config_LCD(void);

#endif /* SYSTEMCONFIG_H_ */
