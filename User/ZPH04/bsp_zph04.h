/* -----------------------------------------------------------------------------
 * File name   : ZPH04.h
 * Author      : PSYM
 * Copyright(C): 2024 PSYM. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef __ZPH04_H_
#define __ZPH04_H_

/* --------------------------------- 包含头文件 --------------------------------- */
#include "main.h"
#include "tim.h"

/* --------------------------------- 类型定义 --------------------------------- */
typedef struct
{
    uint8_t flg;       // 0为未开始，1已经开始，2为结束
    uint16_t num_high; // 高电平时间
    uint16_t num_low;  // 低电平时间
} COUNT_TEMP;

/* --------------------------------- 宏定义 --------------------------------- */
#define ZPH04_TIMx TIM6
#define ZPH04_HTIMx htim6

#define ZPH04_ISLOW() HAL_GPIO_ReadPin(ZPH04_GPIO_Port, ZPH04_Pin) == GPIO_PIN_RESET
#define ZPH04_ISHIGH() HAL_GPIO_ReadPin(ZPH04_GPIO_Port, ZPH04_Pin) == GPIO_PIN_SET
/* --------------------------------- 扩展变量 --------------------------------- */

/* --------------------------------- 函数声明 --------------------------------- */
void ZPH04_Init(void);
uint16_t ZPH04_GetPM2_5(void);
#endif /* __ZPH04_H_ */

    /*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
