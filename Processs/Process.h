#ifndef		_PROCESS_H_
#define 	_PROCESS_H_

typedef enum
{
    SLEEP_MODE = 0,
    WAKEUP_MODE,
    BALANCING_MODE,
    MOVE_MODE,
} MODE;

extern void Balacing_Process();
void CalulateData();
void Implement_Process();
void setMode( MODE  mode);
MODE getMode();
void setkalman_start();

#endif
