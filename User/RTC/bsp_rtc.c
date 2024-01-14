/**
 *-------------------------------------------------------------------------------
 *       (C) Copyright 2024, PSYM. All Rights Reserved
 *-------------------------------------------------------------------------------
 * @file        : rtc.c
 * Description  :
 * @author      : PSYM
 *-------------------------------------------------------------------------------
 * Update note:
 * -----------   -----------   --------------------------------------------------
 *    Date         Author                              Note
 * -----------   -----------   --------------------------------------------------
 * 2024-01-14       PSYM
 *
 **/

/* ---------------------------------- ����ͷ�ļ� ---------------------------------- */
#include "./RTC/bsp_rtc.h"
#if DEBUG
#include "./USART/bsp_usart.h"
#endif
/* ---------------------------------- ˽�к궨�� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�����Ͷ��� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�б��� ---------------------------------- */
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; // ���������ݱ�

/* ---------------------------------- ��չ���� ---------------------------------- */
RTC_HandleTypeDef hrtc;
_calendar_obj calendar;

/* ---------------------------------- ˽�к���ԭ�� ---------------------------------- */
/* None. */

/* ---------------------------------- ������ ---------------------------------- */

/**
 * @brief: None
 * @param: None
 * @return: None
 * @note: None
 */
void RTC_Init(void)
{
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;

    // ����������δ��ʼ����RTC, ����Ҫ��ʼ��, ������Ҫ
    // if (HAL_RTC_Init(&hrtc) != HAL_OK)
    // {
    //     Error_Handler();
    // }

    // �Ƿ��һ������
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0X000B)
    {
        // �������ں�ʱ��
        RTC_SET(2023, 1, 12, 11, 52, 30);
        // ���ú�������֮��д��ʶ�����ݼĴ���,��ʶһ�����ϸ���ͬ,���򲻻�ӱ��ݼĴ����ж�ȡʱ��
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0X000B);
#if DEBUG
        printf("FIRST TIME3\n"); // first time ��ʼ��ʱ�䡣
#endif
    }

    // ʹ�����ж�
    HAL_NVIC_SetPriority(RTC_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
    // ������֮�����һ������
    RTC_Get();
    // �������ж�
    __HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);
}

/* USER CODE BEGIN 1 */
/**
 * @brief ����RTCʱ��
 *
 * @param syear ��
 * @param smonth ��
 * @param sday ��
 * @param shour ʱ
 * @param sminute ��
 * @param ssecond ��
 * @retval None
 * @note �ú����Ὣʱ���ת���ɶ�Ӧ��ʱ�䲢���õ�RTC��
 *
 */
void RTC_SET(uint16_t syear, uint8_t smonth, uint8_t sday, uint8_t shour, uint8_t sminute, uint8_t ssecond)
{
    uint32_t time = 0; // 2022��1��1����1970 ��1��1�����ʱ���
    // ������
    for (uint16_t i = 1970; i < syear; i++)
    {
        if ((0 == i % 4 && i % 100 != 0) || (0 == i % 400))
        {
            time += 31622400; /*����*/
        }
        else
        {
            time += 31536000; /*ƽ��*/
        }
    }
    // ������

    for (uint16_t j = 1; j < smonth; j++)
    {
        if (j == 2 && ((0 == syear % 4 && syear % 100 != 0) || (0 == syear % 400)))
        { // ������������
            time += 29 * 24 * 3600;
        }
        else
        {
            time += mon_table[j - 1] * 24 * 3600; /*�·ݱ�ʵ���±��1*/
        }
    }
    // �����պͷ�
    time += (sday - 1) * 24 * 3600;
    time += shour * 3600;
    time += sminute * 60;
    time += ssecond;
    //	return real_time;
    // ����ʱ��
    RCC->APB1ENR |= 1 << 28; // ʹ�ܵ�Դʱ��
    RCC->APB1ENR |= 1 << 27; // ʹ�ܱ���ʱ��
    PWR->CR |= 1 << 8;       // ȡ��������д����
    RTC->CRL |= 1 << 4;      // ��������
    RTC->CNTL = time & 0xffff;
    RTC->CNTH = time >> 16;
    RTC->CRL &= ~(1 << 4); // ���ø���
    while (!(RTC->CRL & (1 << 5)))
        ;      // �ȴ�RTC�Ĵ����������
    RTC_Get(); // ������֮�����һ������
}

/**
 * @brief ��ʱ�������ɶ�Ӧʱ��浽�ṹ����
 */
void RTC_Get(void)
{
    static uint16_t daycnt = 0; // ��
    uint32_t timecount = 0;     // ��
    uint32_t temp = 0;          // ��
    uint16_t temp1 = 0;         // ��
    timecount = RTC->CNTH;      // �õ��������е�ֵ(������)
    timecount <<= 16;
    timecount += RTC->CNTL;
    temp = timecount / 86400; // �õ�����(��������Ӧ��)
    if (daycnt != temp)       // ����һ����
    {
        daycnt = temp;
        temp1 = 1970; // ��1970�꿪ʼ
        while (temp >= 365)
        {
            if ((temp1 % 4 == 0 && temp1 % 100 != 0) || (temp1 % 400 == 0)) // ������
            {
                if (temp >= 366)
                    temp -= 366; // �����������
                else
                    break;
            }
            else
                temp -= 365; // ƽ��
            temp1++;
        }
        calendar.w_year = temp1; // �õ����
        temp1 = 0;
        while (temp >= 28) // ������һ����
        {
            if (((calendar.w_year % 4 == 0 && calendar.w_year % 100 != 0) || (calendar.w_year % 400 == 0)) && temp1 == 1) // �����ǲ�������/2�·�
            {
                if (temp >= 29)
                    temp -= 29; // �����������
                else
                    break;
            }
            else
            {
                if (temp >= mon_table[temp1])
                    temp -= mon_table[temp1]; // ƽ��
                else
                    break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1; // �õ��·�
        calendar.w_date = temp + 1;   // �õ�����
    }
    temp = timecount % 86400;          // �õ�������
    calendar.hour = temp / 3600;       // Сʱ
    calendar.min = (temp % 3600) / 60; // ����
    calendar.sec = (temp % 3600) % 60; // ����
    // calendar.week = RTC_GetWeek(calendar.w_year, calendar.w_month, calendar.w_date); // ��ȡ����
}
/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
