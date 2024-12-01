#if !defined(APP_ALT_H_)
#define APP_ALT_H_

extern float ALT_Pressure;
extern float ALT_Altitude;

void ALT_Initialize(void);

void ALT_Update(void);

void ALT_SetQNH(float qnh);

#endif // APP_ALT_H_
