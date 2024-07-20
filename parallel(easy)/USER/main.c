#include "stm32f10x.h"

#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"
#include "adc.h"
#include "usart.h"
#include "encoder.h"
#include "pstwo.h"

 

int TargetVelocity_A = 0;
int TargetVelocity_B = 0;
int TargetVelocity_C = 0;
int TargetVelocity_D = 0;

float A;
float B;
float C;
float D;

static void calculate(void);

int main(void)
 {
	 u8 key;	
	 
	 int encoder_A,encoder_B,encoder_C,encoder_D;
	 int Velocity_PWMA=0,Velocity_PWMB=0,Velocity_PWMC=0,Velocity_PWMD=0;
	 u16 adcx;
	 float vcc;
	 
   SystemInit(); //配置系统时钟为72M   
   delay_init();    //延时函数初始化
   Gpio_Init();    //初始化gpioB pin_0/1/8/9/12/13/14/15
   uart_init(115200);		//串口初始化

   adc_Init();				//ADC1的初始化 
	 
   PWM_Int(7199,0);      //初始化pwm输出 72000 000 /7199+1=10000    
   Encoder_Init_Tim8();
   Encoder_Init_Tim2();
   Encoder_Init_Tim3();
   Encoder_Init_Tim4();
	 
	 PS2_Init();
	while(1)
	{
		key=PS2_DataKey();
		calculate();
		
		adcx=Get_adc_Average(ADC_Channel_5,10);  //获取adc的值
		vcc=(float)adcx*(3.3*11/4096);     				//求当前电压
		
		encoder_A=Read_Encoder(8);		//读取编码器数值
		encoder_B=Read_Encoder(2);
		encoder_C=Read_Encoder(3);
		encoder_D=Read_Encoder(4);
		A += encoder_A; 
		B += encoder_B;
		C += encoder_C;
		D += encoder_D;
		A = A/300;
		B = B/300;
		C = C/300;
		D = D/300;
    Velocity_PWMA=Velocity_A(TargetVelocity_A,encoder_A);
		Velocity_PWMB=Velocity_B(TargetVelocity_B,encoder_B);
		Velocity_PWMC=Velocity_C(TargetVelocity_C,encoder_C);
		Velocity_PWMD=Velocity_D(TargetVelocity_D,encoder_D);
		
		Set_PWM(Velocity_PWMA,Velocity_PWMB,Velocity_PWMC,Velocity_PWMD);
		printf("当前电压=%6.2f V  A = %d  B=%d\r\n   C = %d  D=%d\r\n",vcc,(int)A,(int)B,(int)C,(int)D);				
	}
 }

 static void calculate(void)
 {
			if(PS2_AnologData(PSB_PAD_LEFT) == 1)
			{
					motor_A(1);
					TargetVelocity_A = 500;
			}
			else if(PS2_AnologData(PSB_PAD_RIGHT) == 1)
			{
					motor_A(0);
					TargetVelocity_A = 500;
			}
			else
			{
					TargetVelocity_A = 0;
			}
			
			if(PS2_AnologData(PSB_PAD_UP) == 1)
			{
					motor_B(1);
					TargetVelocity_B = 500;
			}
			else if(PS2_AnologData(PSB_PAD_DOWN) == 1)
			{
					motor_B(0);
					TargetVelocity_B = 500;
			}
			else
			{
					TargetVelocity_B = 0;
			}
			
			if(PS2_AnologData(PSB_SQUARE) == 1)
			{
					motor_C(1);
					TargetVelocity_C = 500;
			}
			else if(PS2_AnologData(PSB_CIRCLE) == 1)
			{
					motor_C(0);
					TargetVelocity_C = 500;
			}
			else
			{
					TargetVelocity_C = 0;
			}
			
			if(PS2_AnologData(PSB_TRIANGLE) == 1)
			{
					motor_D(1);
					TargetVelocity_D = 500;
			}
			else if(PS2_AnologData(PSB_CROSS) == 1)
			{
					motor_D(0);
					TargetVelocity_D = 500;
			}
			else
			{
					TargetVelocity_D = 0;
			}
 }

