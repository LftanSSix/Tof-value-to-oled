#ifndef __VL6180X_H
#define __VL6180X_H
#include "./tof/sys.h"

u8 VL6180X_Init(void);
u8 VL6180X_Start_Range(void);
u8 VL6180X_Poll_Range(void);
u8 VL6180_Read_Range(void);
void VL6180X_Clear_Interrupt(void);



#endif
