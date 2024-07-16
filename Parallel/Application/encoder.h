#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "tim.h"

//���1�ı�������������
#define MOTO1_ENCODER1_PORT GPIOA
#define MOTO1_ENCODER1_PIN  GPIO_PIN_0
#define MOTO1_ENCODER2_PORT GPIOA
#define MOTO1_ENCODER2_PIN  GPIO_PIN_1

//��ʱ����
#define ENCODER1_TIM htim2
#define ENCODER2_TIM htim3
#define ENCODER3_TIM htim4
#define ENCODER4_TIM htim8
#define PWM_TIM     htim5
#define GAP_TIM     htim6

#define MOTOR_SPEED_RERATIO 45u    //������ٱ�
#define PULSE_PRE_ROUND 11 //һȦ���ٸ�����
#define RADIUS_OF_TYRE 34 //��̥�뾶����λ����
#define LINE_SPEED_C RADIUS_OF_TYRE * 2 * 3.14
#define RELOADVALUE __HAL_TIM_GetAutoreload(&ENCODER_TIM)    //��ȡ�Զ�װ��ֵ,������Ϊ20000 

#define COUNTERNUM1 __HAL_TIM_GetCounter(&ENCODER1_TIM)        //��ȡ��������ʱ���еļ���ֵ
#define COUNTERNUM2 __HAL_TIM_GetCounter(&ENCODER2_TIM)        //��ȡ��������ʱ���еļ���ֵ
#define COUNTERNUM3 __HAL_TIM_GetCounter(&ENCODER3_TIM)        //��ȡ��������ʱ���еļ���ֵ
#define COUNTERNUM4 __HAL_TIM_GetCounter(&ENCODER4_TIM)        //��ȡ��������ʱ���еļ���ֵ

typedef struct _Motor
{
    int32_t lastCount;   //��һ�μ���ֵ
    int32_t totalCount;  //�ܼ���ֵ
    int16_t overflowNum; //�������
    float speed;         //���ת��
    uint8_t direct;      //��ת����
}Motor;



void Motor_Init(void);

#endif
