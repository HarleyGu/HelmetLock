#include "Main.h"

// �����ת
void Motor_Forward(void)
{
    GPIOA_SET_H(FI_PIN);
    GPIOA_SET_L(BI_PIN);
}

// �����ת
void Motor_Reverse(void)
{
    GPIOA_SET_H(BI_PIN);
    GPIOA_SET_L(FI_PIN);
}

// ���ֹͣ
void Motor_Stop(void)
{
    GPIOA_SET_L(BI_PIN);
    GPIOA_SET_L(FI_PIN);
}

// ��������
void Voice_Printf(uint8_t *buf)
{
    uint16_t len = strlen(buf);
    UART1_SendString(buf, len);
}

int main(void)
{
    // ��ʱ��
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    // PA10->BI PA12->FI
    GPIOA_ModeCfg(BI_PIN, GPIO_ModeOut_PP_20mA);
    GPIOA_ModeCfg(FI_PIN, GPIO_ModeOut_PP_20mA);
    // ��������λ�ź�
    GPIOA_ModeCfg(LOCK_PIN, GPIO_ModeIN_PU);
    // ��������λ�ź�
    GPIOA_ModeCfg(UNLOCK_PIN, GPIO_ModeIN_PU);
    // ���ɻɹ��ź�
    GPIOA_ModeCfg(REEDSWITCH_PIN, GPIO_ModeIN_PU);

    // ���ڳ�ʼ�� PA8->RX(����) PA9->TX(����)
    GPIOA_SET_H(UART1_TX_PIN);
    GPIOA_ModeCfg(UART1_RX_PIN, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(UART1_TX_PIN, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();

    while (1)
    {
        if (GPIOA_READ_PIN(REEDSWITCH_PIN) == 0) // �����ɻɹ��ź�Ϊ0 ��ʾͷ������
        {
            uint8_t i;
            // ������
            Motor_Forward();
            Motor_State_Flag = 0;

            for (i = 1; i <= TIMEOUT; i++)
            {
                if ((GPIOA_READ_PIN(LOCK_PIN) == 0) && (Motor_State_Flag == 0))
                {
                    Correct_Count += 1;
                    Motor_Stop();
                    // printf("�����ɹ���ֹͣ\r\n");
                    Voice_Printf("<G>�����ɹ�");
                    DelayMs(3000);
                    break;
                }
                DelayMs(5);
            }

            if (i >= TIMEOUT)
            {
                Motor_Stop();
                // printf("����ʧ�ܣ�ֹͣ\r\n");
                Voice_Printf("<G>����ʧ��");
                DelayMs(3000);
            }

            // �ٽ���
            Motor_Reverse();
            Motor_State_Flag = 1;

            for (i = 1; i <= TIMEOUT; i++)
            {
                if ((GPIOA_READ_PIN(UNLOCK_PIN) == 0) && (Motor_State_Flag == 1))
                {
                    Correct_Count += 2;
                    Motor_Stop();
                    // printf("�����ɹ���ֹͣ\r\n");
                    Voice_Printf("<G>�����ɹ�");
                    DelayMs(3000);
                    break;
                }
                DelayMs(5);
            }

            if (i >= TIMEOUT)
            {
                Motor_Stop();
                //printf("����ʧ�ܣ�ֹͣ\r\n");
                Voice_Printf("<G>����ʧ��");
                DelayMs(3000);
            }

            if (Correct_Count == 0)
            {
                //printf("����������ʧ��\r\n");
                Voice_Printf("<G>����������ʧ��");
                DelayMs(4000);
            }
            else if (Correct_Count == 1)
            {
                //printf("�����ɹ�������ʧ��\r\n");
                Voice_Printf("<G>�����ɹ�������ʧ��");
                DelayMs(4000);
            }
            else if (Correct_Count == 2)
            {
                //printf("�����ɹ�������ʧ��\r\n");
                Voice_Printf("<G>�����ɹ�������ʧ��");
                DelayMs(4000);
            }
            else
            {
                //printf("�����������ɹ�\r\n");
                Voice_Printf("<G>�����������ɹ�");
                DelayMs(4000);
            }

            Correct_Count = 0;

            // �ص��м�λ��
            Motor_Forward();
            DelayMs(50);
            Motor_Stop();
            DelayMs(1000);
        }
        else
        {
            Motor_Stop();
            // printf("δ��⵽ͷ��\r\n");
            Voice_Printf("<G>δ��⵽ͷ��");
            DelayMs(3000);
        }
    }
}
