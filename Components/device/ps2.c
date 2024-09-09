/*******************************************************************************
 * 
 * File name:ps2 
 * Author:Misaka17061         Version:1.0        Date:2024/9/6 
 * Description: 
 * Function List:
 *   1. 
 * History:
 *      <author> <time>  <version > <desc>
*******************************************************************************/

/* 包含头文件 ----------------------------------------------------------------*/
#include "ps2.h"
#include "gpio.h"
#include "bsp_init.h"

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
uint16_t Handkey;
uint8_t Comd[2]={0x01,0x42};	//开始命令。请求数据
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint8_t scan[9]={0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//{0x01,0x42,0x00,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A};	// 类型读取

uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK,
	PSB_TRIANGLE,
    PSB_CIRCLE,
	PSB_CROSS,
	PSB_SQUARE
	};	//按键值与按键明
/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/
void ps2_Init(void)
{
	uint8_t enter_config[9]={0x01,0x43,0x00,0x01,0x00,0x00,0x00,0x00,0x00};
	uint8_t config[9]={0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};
	uint8_t vibration_on[5]={0x01,0x4D,0x00,0x00,0x01};
	uint8_t exit_config[9]={0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
	uint8_t i;
    HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
    BSP_DelayUs(16);
    for(i=0; i<9; i++)
	{
		Data[i] = PS2_ReadData(enter_config[i]);
	}
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	BSP_DelayUs(16);
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
	BSP_DelayUs(16);
	for(i=0; i<9; i++)
	{
		Data[i] = PS2_ReadData(enter_config[i]);
	}
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	BSP_DelayUs(16);
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
	BSP_DelayUs(16);
	for(i=0; i<9; i++)
	{
		Data[i] = PS2_ReadData(config[i]);
	}
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	BSP_DelayUs(16);
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
	BSP_DelayUs(16);
	for(i=0; i<5; i++)
	{
		Data[i] = PS2_ReadData(vibration_on[i]);
	}
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	BSP_DelayUs(16);
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
	BSP_DelayUs(16);
	for(i=0; i<9; i++)
	{
		Data[i] = PS2_ReadData(exit_config[i]);
	}
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	BSP_DelayUs(16);
}

uint8_t PS2_ReadData(uint8_t command)
{

	uint8_t i,j=1;
	uint8_t res=0; 
    for(i=0; i<=7; i++)          
    {
		if(command&0x01)
			HAL_GPIO_WritePin(GPIOC, CMD_Pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOC, CMD_Pin, GPIO_PIN_RESET);
		command = command >> 1;
		BSP_DelayUs(6);//10
		HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_RESET);
		BSP_DelayUs(6); //10
		if(HAL_GPIO_ReadPin(GPIOC, DAT_Pin)) 
			res = res + j;
		j = j << 1; 
		HAL_GPIO_WritePin(GPIOC, CLK_Pin, GPIO_PIN_SET);
		BSP_DelayUs(6);//10	 
    }
    HAL_GPIO_WritePin(GPIOC, CMD_Pin, GPIO_PIN_SET);
	  BSP_DelayUs(55);//50
    return res;	
}

unsigned char PS2_DataKey()
{
	uint8_t index = 0, i = 0;

	PS2_ClearData();
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);
	for(i=0;i<9;i++)	//更新扫描按键
	{
		Data[i] = PS2_ReadData(scan[i]);	
	} 
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);
	

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
			return index+1;
	}
	return 0;          //没有任何按键按下
}

uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	uint8_t i=0;
	uint8_t vibration[9]={0x01,0x42,0x00,motor1,motor2,0x00,0x00,0x00,0x00};
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_RESET);;
	BSP_DelayUs(16);
	for(i=0; i<9; i++)
	{
		Data[i] = PS2_ReadData(vibration[i]);
	}
	HAL_GPIO_WritePin(GPIOC, CS_Pin, GPIO_PIN_SET);;
	BSP_DelayUs(16);  
}
