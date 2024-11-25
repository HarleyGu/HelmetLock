#ifndef __MAIN_H
#define __MAIN_H

#include "CH58x_common.h"

#define GPIOA_SET_H    GPIOA_SetBits
#define GPIOA_SET_L    GPIOA_ResetBits
#define GPIOA_READ_PIN GPIOA_ReadPortPin
#define BI_PIN         GPIO_Pin_10
#define FI_PIN         GPIO_Pin_12
#define LOCK_PIN       GPIO_Pin_7
#define UNLOCK_PIN     GPIO_Pin_11
#define UNLOCK_PIN     GPIO_Pin_11
#define REEDSWITCH_PIN GPIO_Pin_0
#define UART1_TX_PIN   GPIO_Pin_9
#define UART1_RX_PIN   GPIO_Pin_8
#define TIMEOUT        40

BOOL Motor_State_Flag = 0; // 电机状态标志位 0:正转 1:反转
uint8_t Correct_Count = 0; // 校正计数

void Motor_Forward(void);
void Motor_Reverse(void);
void Motor_Stop(void);
void Voice_Printf(uint8_t *buf);

#endif
