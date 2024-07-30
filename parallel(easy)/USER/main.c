#include "stm32f10x.h"

#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"
#include "adc.h"
#include "usart.h"
#include "encoder.h"
#include "pstwo.h"

 typedef struct key
{
		uint8_t key1;
		uint8_t key2;
		uint8_t key3;
		uint8_t key4;
		uint8_t key5;
		uint8_t key6;
		uint8_t key7;
		uint8_t key8;

}key_t;

typedef struct step
{
		uint16_t step1;
		uint16_t step2;
		uint16_t step3;
}step_t;

void step(step_t* step,uint16_t step1,uint16_t step2,uint16_t step3)
{
		step->step1++;
		if (step->step1 <= step1) return;
		
		step->step2++;
		if(step->step2 <= step2) return;
	
		step->step3++;
		if(step->step3 <= step3) return;

}

int TargetVelocity_A = 100;
int TargetVelocity_B = 100;
int TargetVelocity_C = 100;
int TargetVelocity_D = 100;

float A;
float B;
float C;
float D;



static void calculate(uint8_t key1,uint8_t key2,uint8_t key3,uint8_t key4,uint8_t key5,uint8_t key6,uint8_t key7,uint8_t key8);





void dicision(key_t* key,uint8_t key1,uint8_t key2,uint8_t key3,uint8_t key4,uint8_t key5,uint8_t key6,uint8_t key7,uint8_t key8)
{
		key->key1 = key1;
		key->key2 = key2;
	
		key->key3 = key3;		
		key->key4 = key4;
	
		key->key5 = key5;
		key->key6 = key6;
	
		key->key7 = key7;
		key->key8 = key8;
}
	

int main(void)
 {
	 key_t* key;	
	 
	 
	 int encoder_A = 0,encoder_B = 0,encoder_C = 0,encoder_D = 0;
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
	 
	// PS2_Init();
	while(1)
	{
		
		//key=PS2_DataKey();
		calculate(1,0,1,0,1,0,1,0);
		
			
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

//    Velocity_PWMA=Velocity_A(TargetVelocity_A,encoder_A);
//		Velocity_PWMB=Velocity_B(TargetVelocity_B,encoder_B);
//		Velocity_PWMC=Velocity_C(TargetVelocity_C,encoder_C);
//		Velocity_PWMD=Velocity_D(TargetVelocity_D,encoder_D);
		
		Set_PWM(1000,1000,1000,1000);
		printf("当前电压=%6.2f V  A = %d  B=%d\r\n   C = %d  D=%d\r\n",vcc,(int)A/600,(int)B/600,(int)C/600,(int)D/600);	
		//printf("当前电压=%6.2f V  A = %d  B=%d\r\n   C = %d  D=%d\r\n",vcc,PS2_AnologData(PSB_PAD_LEFT),PS2_AnologData(PSB_PAD_RIGHT),PS2_AnologData(PSB_PAD_UP),PS2_AnologData(PSB_PAD_DOWN));	
	}
 }

 static void calculate(uint8_t key1,uint8_t key2,uint8_t key3,uint8_t key4,uint8_t key5,uint8_t key6,uint8_t key7,uint8_t key8)
 {
			if( key1 == 1)
			{
					motor_A(1);
			}
			else if(key2 == 1)
			{
					motor_A(0);
			}

			if(key3 == 1)
			{
					motor_B(1);
			}
			else if(key4 == 1)
			{
					motor_B(0);
			}
			
			if(key5 == 1)
			{
					motor_C(1);
			}
			else if(key6 == 1)
			{
					motor_C(0);
			}
			
			if(key7 == 1)
			{
					motor_D(1);
			}
			else if(key8 == 1)
			{
					motor_D(0);
			}
 }

