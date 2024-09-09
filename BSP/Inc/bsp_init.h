#ifndef BSP_INIT_H
#define BSP_INIT_H

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "bsp_def.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "bsp_i2c.h"
#include "bsp_uart.h"

/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/
/* IO对象结构 */

/* 串口对象结构 */
extern UART_Object_t com1_obj;



/* 函数声明 ------------------------------------------------------------------*/
void BSP_Init(void);
uint8_t PS2_ReadData(uint8_t command);
unsigned char PS2_DataKey();
uint8_t PS2_AnologData(uint8_t button);
void PS2_ClearData();
void PS2_Vibration(uint8_t motor1, uint8_t motor2);

#endif  // BSP_INIT_H

