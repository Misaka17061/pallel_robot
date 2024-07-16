#include "encoder.h"

Motor motor[4];

void Motor_Init(void)
{
    HAL_TIM_Encoder_Start(&ENCODER1_TIM, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&ENCODER1_TIM,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
	
	  HAL_TIM_Encoder_Start(&ENCODER2_TIM, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&ENCODER2_TIM,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
																
	  HAL_TIM_Encoder_Start(&ENCODER3_TIM, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&ENCODER3_TIM,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
	
	  HAL_TIM_Encoder_Start(&ENCODER4_TIM, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&ENCODER4_TIM,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
	
	
    HAL_TIM_Base_Start_IT(&GAP_TIM);                       //开启100ms定时器中断
    HAL_TIM_PWM_Start(&PWM_TIM, TIM_CHANNEL_ALL);            //开启PWM
	
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器回调函数，用于计算速度
{
    if(htim->Instance==ENCODER1_TIM.Instance)//编码器输入定时器溢出中断，用于防溢出                   
    {      
        if( COUNTERNUM1  < 10000) motor[0].overflowNum++;       //如果是向上溢出
        else if(COUNTERNUM1 >= 10000) motor[0].overflowNum--; //如果是向下溢出
        __HAL_TIM_SetCounter(&ENCODER1_TIM, 10000);             //重新设定初始值
    }
		else if(htim->Instance==ENCODER2_TIM.Instance)//编码器输入定时器溢出中断，用于防溢出                   
    {      
        if( COUNTERNUM2  < 10000) motor[1].overflowNum++;       //如果是向上溢出
        else if(COUNTERNUM2 >= 10000) motor[0].overflowNum--; //如果是向下溢出
        __HAL_TIM_SetCounter(&ENCODER2_TIM, 10000);             //重新设定初始值
    }
		else if(htim->Instance==ENCODER3_TIM.Instance)//编码器输入定时器溢出中断，用于防溢出                   
    {      
        if( COUNTERNUM3  < 10000) motor[2].overflowNum++;       //如果是向上溢出
        else if(COUNTERNUM3 >= 10000) motor[0].overflowNum--; //如果是向下溢出
        __HAL_TIM_SetCounter(&ENCODER3_TIM, 10000);             //重新设定初始值
    }
		else if(htim->Instance==ENCODER4_TIM.Instance)//编码器输入定时器溢出中断，用于防溢出                   
    {      
        if( COUNTERNUM4  < 10000) motor[3].overflowNum++;       //如果是向上溢出
        else if(COUNTERNUM4 >= 10000) motor[0].overflowNum--; //如果是向下溢出
        __HAL_TIM_SetCounter(&ENCODER4_TIM, 10000);             //重新设定初始值
    }
		
    else if(htim->Instance==GAP_TIM.Instance)//间隔定时器中断，是时候计算速度了
    {
				motor[0].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER1_TIM);//如果向上计数（正转），返回值为0，否则返回值为1
				motor[0].totalCount = COUNTERNUM1 + motor[0].overflowNum * 10000;//一个周期内的总计数值等于目前计数值加上溢出的计数值
				motor[0].speed = (float)(motor[0].totalCount - motor[0].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//算得每秒多少转
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
				motor[0].lastCount = motor[0].totalCount; //记录这一次的计数值
			
				motor[1].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER2_TIM);//如果向上计数（正转），返回值为0，否则返回值为1
				motor[1].totalCount = COUNTERNUM1 + motor[1].overflowNum * 10000;//一个周期内的总计数值等于目前计数值加上溢出的计数值
				motor[1].speed = (float)(motor[1].totalCount - motor[1].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//算得每秒多少转
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
				motor[1].lastCount = motor[1].totalCount; //记录这一次的计数值

				motor[2].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER3_TIM);//如果向上计数（正转），返回值为0，否则返回值为1
				motor[2].totalCount = COUNTERNUM1 + motor[2].overflowNum * 10000;//一个周期内的总计数值等于目前计数值加上溢出的计数值
				motor[2].speed = (float)(motor[2].totalCount - motor[2].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//算得每秒多少转
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
				motor[2].lastCount = motor[2].totalCount; //记录这一次的计数值
			
				motor[3].direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(&ENCODER4_TIM);//如果向上计数（正转），返回值为0，否则返回值为1
				motor[3].totalCount = COUNTERNUM1 + motor[3].overflowNum * 10000;//一个周期内的总计数值等于目前计数值加上溢出的计数值
				motor[3].speed = (float)(motor[3].totalCount - motor[3].lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10;//算得每秒多少转
				//motor1.speed = (float)(motor1.totalCount - motor1.lastCount) / (4 * MOTOR_SPEED_RERATIO * PULSE_PRE_ROUND) * 10 * LINE_SPEED_C//算得车轮线速度每秒多少毫米
				motor[3].lastCount = motor[3].totalCount; //记录这一次的计数值
    }
}
