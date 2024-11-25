#include "Main.h"

// 电机正转
void Motor_Forward(void)
{
    GPIOA_SET_H(FI_PIN);
    GPIOA_SET_L(BI_PIN);
}

// 电机反转
void Motor_Reverse(void)
{
    GPIOA_SET_H(BI_PIN);
    GPIOA_SET_L(FI_PIN);
}

// 电机停止
void Motor_Stop(void)
{
    GPIOA_SET_L(BI_PIN);
    GPIOA_SET_L(FI_PIN);
}

// 语音播报
void Voice_Printf(uint8_t *buf)
{
    uint16_t len = strlen(buf);
    UART1_SendString(buf, len);
}

int main(void)
{
    // 开时钟
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    // PA10->BI PA12->FI
    GPIOA_ModeCfg(BI_PIN, GPIO_ModeOut_PP_20mA);
    GPIOA_ModeCfg(FI_PIN, GPIO_ModeOut_PP_20mA);
    // 读上锁到位信号
    GPIOA_ModeCfg(LOCK_PIN, GPIO_ModeIN_PU);
    // 读解锁到位信号
    GPIOA_ModeCfg(UNLOCK_PIN, GPIO_ModeIN_PU);
    // 读干簧管信号
    GPIOA_ModeCfg(REEDSWITCH_PIN, GPIO_ModeIN_PU);

    // 串口初始化 PA8->RX(接收) PA9->TX(发送)
    GPIOA_SET_H(UART1_TX_PIN);
    GPIOA_ModeCfg(UART1_RX_PIN, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(UART1_TX_PIN, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();

    while (1)
    {
        if (GPIOA_READ_PIN(REEDSWITCH_PIN) == 0) // 读到干簧管信号为0 表示头盔放下
        {
            uint8_t i;
            // 先上锁
            Motor_Forward();
            Motor_State_Flag = 0;

            for (i = 1; i <= TIMEOUT; i++)
            {
                if ((GPIOA_READ_PIN(LOCK_PIN) == 0) && (Motor_State_Flag == 0))
                {
                    Correct_Count += 1;
                    Motor_Stop();
                    // printf("上锁成功，停止\r\n");
                    Voice_Printf("<G>上锁成功");
                    DelayMs(3000);
                    break;
                }
                DelayMs(5);
            }

            if (i >= TIMEOUT)
            {
                Motor_Stop();
                // printf("上锁失败，停止\r\n");
                Voice_Printf("<G>上锁失败");
                DelayMs(3000);
            }

            // 再解锁
            Motor_Reverse();
            Motor_State_Flag = 1;

            for (i = 1; i <= TIMEOUT; i++)
            {
                if ((GPIOA_READ_PIN(UNLOCK_PIN) == 0) && (Motor_State_Flag == 1))
                {
                    Correct_Count += 2;
                    Motor_Stop();
                    // printf("解锁成功，停止\r\n");
                    Voice_Printf("<G>解锁成功");
                    DelayMs(3000);
                    break;
                }
                DelayMs(5);
            }

            if (i >= TIMEOUT)
            {
                Motor_Stop();
                //printf("解锁失败，停止\r\n");
                Voice_Printf("<G>解锁失败");
                DelayMs(3000);
            }

            if (Correct_Count == 0)
            {
                //printf("上锁解锁都失败\r\n");
                Voice_Printf("<G>上锁解锁都失败");
                DelayMs(4000);
            }
            else if (Correct_Count == 1)
            {
                //printf("上锁成功，解锁失败\r\n");
                Voice_Printf("<G>上锁成功，解锁失败");
                DelayMs(4000);
            }
            else if (Correct_Count == 2)
            {
                //printf("解锁成功，上锁失败\r\n");
                Voice_Printf("<G>解锁成功，上锁失败");
                DelayMs(4000);
            }
            else
            {
                //printf("上锁解锁都成功\r\n");
                Voice_Printf("<G>上锁解锁都成功");
                DelayMs(4000);
            }

            Correct_Count = 0;

            // 回到中间位置
            Motor_Forward();
            DelayMs(50);
            Motor_Stop();
            DelayMs(1000);
        }
        else
        {
            Motor_Stop();
            // printf("未检测到头盔\r\n");
            Voice_Printf("<G>未检测到头盔");
            DelayMs(3000);
        }
    }
}
