#include "encoder.h"

Motor motor[4];

void Motor_Init(void)
{
    HAL_TIM_Encoder_Start(&ENCODER1_TIM, TIM_CHANNEL_ALL);      //������������ʱ��
    __HAL_TIM_ENABLE_IT(&ENCODER1_TIM,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
	
	  HAL_TIM_Encoder_Start(&ENCODER2_TIM, TIM_CHANNEL_ALL);      //������������ʱ��
    __HAL_TIM_ENABLE_IT(&ENCODER2_TIM,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
																
	  HAL_TIM_Encoder_Start(&ENCODER3_TIM, TIM_CHANNEL_ALL);      //������������ʱ��
    __HAL_TIM_ENABLE_IT(&ENCODER3_TIM,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
	
	  HAL_TIM_Encoder_Start(&ENCODER4_TIM, TIM_CHANNEL_ALL);      //������������ʱ��
    __HAL_TIM_ENABLE_IT(&ENCODER4_TIM,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
	
	
    HAL_TIM_Base_Start_IT(&GAP_TIM);                       //����100ms��ʱ���ж�
    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_ALL);            //����PWM
	
    __HAL_TIM_SET_COUNTER(&ENCODER1_TIM, 10000); 
    __HAL_TIM_SET_COUNTER(&ENCODER2_TIM, 10000);
    __HAL_TIM_SET_COUNTER(&ENCODER3_TIM, 10000);
    __HAL_TIM_SET_COUNTER(&ENCODER4_TIM, 10000);	
	
		for(int i = 0 ; i < 4 ; i++)
		{
				motor[i].lastCount = 0;                                   
				motor[i].totalCount = 0;
				motor[i].overflowNum = 0;                                  
				motor[i].speed = 0;
				motor[i].direct = 0;
		}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//��ʱ���ص����������ڼ����ٶ�
{
    if(htim->Instance==ENCODER1_TIM.Instance)//���������붨ʱ������жϣ����ڷ����                   
    {      
        if( COUNTERNUM1  < 10000) motor[0].overflowNum++;       //������������
        else if(COUNTERNUM1 >= 10000) motor[0].overflowNum--; //������������
        __HAL_TIM_SetCounter(&ENCODER1_TIM, 10000);             //�����趨��ʼֵ
    }
		else if(htim->Instance==ENCODER2_TIM.Instance)//���������붨ʱ������жϣ����ڷ����                   
    {      
        if( COUNTERNUM2  < 10000) motor[1].overflowNum++;       //������������
        else if(COUNTERNUM2 >= 10000) motor[0].overflowNum--; //������������
        __HAL_TIM_SetCounter(&ENCODER2_TIM, 10000);             //�����趨��ʼֵ
    }
		else if(htim->Instance==ENCODER3_TIM.Instance)//���������붨ʱ������жϣ����ڷ����                   
    {      
        if( COUNTERNUM3  < 10000) motor[2].overflowNum++;       //������������
        else if(COUNTERNUM3 >= 10000) motor[0].overflowNum--; //������������
        __HAL_TIM_SetCounter(&ENCODER3_TIM, 10000);             //�����趨��ʼֵ
    }
		else if(htim->Instance==ENCODER4_TIM.Instance)//���������붨ʱ������жϣ����ڷ����                   
    {      
        if( COUNTERNUM4  < 10000) motor[3].overflowNum++;       //������������
        else if(COUNTERNUM4 >= 10000) motor[0].overflowNum--; //������������
        __HAL_TIM_SetCounter(&ENCODER4_TIM, 10000);             //�����趨��ʼֵ
    }
		
    else if(htim->Instance==GAP_TIM.Instance)//�����ʱ���жϣ���ʱ������ٶ���
    {
				motor[0].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER1_TIM);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
				motor[0].totalCount = COUNTERNUM1 + motor[0].overflowNum * 10000;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
				motor[0].speed = (float)(motor[0].totalCount - motor[0].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//���ÿ�����ת
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//��ó������ٶ�ÿ����ٺ���
				motor[0].lastCount = motor[0].totalCount; //��¼��һ�εļ���ֵ
			
				motor[1].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER2_TIM);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
				motor[1].totalCount = COUNTERNUM1 + motor[1].overflowNum * 10000;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
				motor[1].speed = (float)(motor[1].totalCount - motor[1].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//���ÿ�����ת
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//��ó������ٶ�ÿ����ٺ���
				motor[1].lastCount = motor[1].totalCount; //��¼��һ�εļ���ֵ

				motor[2].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER3_TIM);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
				motor[2].totalCount = COUNTERNUM1 + motor[2].overflowNum * 10000;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
				motor[2].speed = (float)(motor[2].totalCount - motor[2].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//���ÿ�����ת
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//��ó������ٶ�ÿ����ٺ���
				motor[2].lastCount = motor[2].totalCount; //��¼��һ�εļ���ֵ
			
				motor[3].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER4_TIM);//������ϼ�������ת��������ֵΪ0�����򷵻�ֵΪ1
				motor[3].totalCount = COUNTERNUM1 + motor[3].overflowNum * 10000;//һ�������ڵ��ܼ���ֵ����Ŀǰ����ֵ��������ļ���ֵ
				motor[3].speed = (float)(motor[3].totalCount - motor[3].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//���ÿ�����ת
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//��ó������ٶ�ÿ����ٺ���
				motor[3].lastCount = motor[3].totalCount; //��¼��һ�εļ���ֵ
    }
}
