/**
 * -----------------------------------------------------------------------------
 * @file   : bsp_rocker.h
 * @author      : PSYM
 * Copyright(C): 2024 PSYM. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef __bsp_rocker_H_
#define __bsp_rocker_H_

/* --------------------------------- ����ͷ�ļ� --------------------------------- */
#include "main.h"
#include "adc.h"

/* --------------------------------- ���Ͷ��� --------------------------------- */

/* --------------------------------- �궨�� --------------------------------- */
#define ROCKER_HADCx hadc1
#define VCC 3300           // ��Դ��ѹmv
#define ADC_RES_12BIT 4096 // 12λ�����ֱ���

// ҡ�˲���
#define ROCKER_NONE 0
#define ROCKER_PRESS 1
#define ROCKER_UP 2
#define ROCKER_DOWN 3
#define ROCKER_LEFT 4
#define ROCKER_RIGHT 5

// ҡ�˲ɼ��߽�ֵ
#define X_LEFTEDGE 1000
#define X_RIGHTEDGE 3800
#define Y_UPEDGE 1000
#define Y_DOWNEDGE 3800

/* --------------------------------- ��չ���� --------------------------------- */
/* None. */

/* --------------------------------- �������� --------------------------------- */
void Rocker_Init(void);
uint8_t Rocker_Get_Direction(uint8_t mode);
#endif /* __bsp_rocker_H_ */

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
