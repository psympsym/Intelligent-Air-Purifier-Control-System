/**
 * -----------------------------------------------------------------------------
 * @file   : rtc.h
 * @author      : PSYM
 * Copyright(C): 2024 PSYM. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#ifndef __rtc_H_
#define __rtc_H_

/* --------------------------------- ����ͷ�ļ� --------------------------------- */
#include "main.h"

/* --------------------------------- ���Ͷ��� --------------------------------- */
typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    // ������������
    uint16_t w_year;
    uint8_t w_month;
    uint8_t w_date;
    uint8_t week;
} _calendar_obj;

/* --------------------------------- �궨�� --------------------------------- */
/* None. */

/* --------------------------------- ��չ���� --------------------------------- */
extern RTC_HandleTypeDef hrtc;
extern _calendar_obj calendar; // ���ʱ��ṹ��ı���

/* --------------------------------- �������� --------------------------------- */
void RTC_Init(void);
void RTC_SET(uint16_t syear, uint8_t smonth, uint8_t sday, uint8_t shour, uint8_t sminute, uint8_t ssecond);
void RTC_Get(void);

#endif /* __rtc_H_ */

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
