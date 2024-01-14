/* -----------------------------------------------------------------------------
 * File name   : ZPH04.h
 * Author      : PSYM
 * Copyright(C): 2024 PSYM. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef __ZPH04_H_
#define __ZPH04_H_

/* --------------------------------- ����ͷ�ļ� --------------------------------- */
#include "main.h"
#include "tim.h"

/* --------------------------------- ���Ͷ��� --------------------------------- */
typedef struct
{
    uint8_t flg;       // 0Ϊδ��ʼ��1�Ѿ���ʼ��2Ϊ����
    uint16_t num_high; // �ߵ�ƽʱ��
    uint16_t num_low;  // �͵�ƽʱ��
} COUNT_TEMP;

/* --------------------------------- �궨�� --------------------------------- */
#define ZPH04_TIMx TIM6
#define ZPH04_HTIMx htim6

#define ZPH04_ISLOW() HAL_GPIO_ReadPin(ZPH04_GPIO_Port, ZPH04_Pin) == GPIO_PIN_RESET
#define ZPH04_ISHIGH() HAL_GPIO_ReadPin(ZPH04_GPIO_Port, ZPH04_Pin) == GPIO_PIN_SET
/* --------------------------------- ��չ���� --------------------------------- */

/* --------------------------------- �������� --------------------------------- */
void ZPH04_Init(void);
uint16_t ZPH04_GetPM2_5(void);
#endif /* __ZPH04_H_ */

    /*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
