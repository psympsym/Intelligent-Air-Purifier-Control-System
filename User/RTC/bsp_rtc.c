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

/* ---------------------------------- 包含头文件 ---------------------------------- */
#include "./RTC/bsp_rtc.h"
#if DEBUG
#include "./USART/bsp_usart.h"
#endif
/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有变量 ---------------------------------- */
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5}; // 月修正数据表

/* ---------------------------------- 扩展变量 ---------------------------------- */
RTC_HandleTypeDef hrtc;
_calendar_obj calendar;

/* ---------------------------------- 私有函数原型 ---------------------------------- */
/* None. */

/* ---------------------------------- 函数体 ---------------------------------- */

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

    // 如果开发版从未初始化过RTC, 则需要初始化, 否则不需要
    // if (HAL_RTC_Init(&hrtc) != HAL_OK)
    // {
    //     Error_Handler();
    // }

    // 是否第一次配置
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0X000B)
    {
        // 设置日期和时间
        RTC_SET(2023, 1, 12, 11, 52, 30);
        // 设置好年月日之后写标识给备份寄存器,标识一定与上个相同,否则不会从备份寄存器中读取时间
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0X000B);
#if DEBUG
        printf("FIRST TIME3\n"); // first time 开始的时间。
#endif
    }

    // 使能秒中断
    HAL_NVIC_SetPriority(RTC_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
    // 设置完之后更新一下数据
    RTC_Get();
    // 开启秒中断
    __HAL_RTC_SECOND_ENABLE_IT(&hrtc, RTC_IT_SEC);
}

/* USER CODE BEGIN 1 */
/**
 * @brief 设置RTC时间
 *
 * @param syear 年
 * @param smonth 月
 * @param sday 日
 * @param shour 时
 * @param sminute 分
 * @param ssecond 秒
 * @retval None
 * @note 该函数会将时间戳转换成对应的时间并设置到RTC中
 *
 */
void RTC_SET(uint16_t syear, uint8_t smonth, uint8_t sday, uint8_t shour, uint8_t sminute, uint8_t ssecond)
{
    uint32_t time = 0; // 2022年1月1日与1970 年1月1日相差时间戳
    // 对齐年
    for (uint16_t i = 1970; i < syear; i++)
    {
        if ((0 == i % 4 && i % 100 != 0) || (0 == i % 400))
        {
            time += 31622400; /*闰年*/
        }
        else
        {
            time += 31536000; /*平年*/
        }
    }
    // 对齐月

    for (uint16_t j = 1; j < smonth; j++)
    {
        if (j == 2 && ((0 == syear % 4 && syear % 100 != 0) || (0 == syear % 400)))
        { // 设置年是闰年
            time += 29 * 24 * 3600;
        }
        else
        {
            time += mon_table[j - 1] * 24 * 3600; /*月份比实际下标大1*/
        }
    }
    // 对齐日和分
    time += (sday - 1) * 24 * 3600;
    time += shour * 3600;
    time += sminute * 60;
    time += ssecond;
    //	return real_time;
    // 设置时钟
    RCC->APB1ENR |= 1 << 28; // 使能电源时钟
    RCC->APB1ENR |= 1 << 27; // 使能备份时钟
    PWR->CR |= 1 << 8;       // 取消备份区写保护
    RTC->CRL |= 1 << 4;      // 允许配置
    RTC->CNTL = time & 0xffff;
    RTC->CNTH = time >> 16;
    RTC->CRL &= ~(1 << 4); // 配置更新
    while (!(RTC->CRL & (1 << 5)))
        ;      // 等待RTC寄存器操作完成
    RTC_Get(); // 设置完之后更新一下数据
}

/**
 * @brief 将时间戳换算成对应时间存到结构体中
 */
void RTC_Get(void)
{
    static uint16_t daycnt = 0; // 日
    uint32_t timecount = 0;     // 秒
    uint32_t temp = 0;          // 天
    uint16_t temp1 = 0;         // 年
    timecount = RTC->CNTH;      // 得到计数器中的值(秒钟数)
    timecount <<= 16;
    timecount += RTC->CNTL;
    temp = timecount / 86400; // 得到天数(秒钟数对应的)
    if (daycnt != temp)       // 超过一天了
    {
        daycnt = temp;
        temp1 = 1970; // 从1970年开始
        while (temp >= 365)
        {
            if ((temp1 % 4 == 0 && temp1 % 100 != 0) || (temp1 % 400 == 0)) // 是闰年
            {
                if (temp >= 366)
                    temp -= 366; // 闰年的秒钟数
                else
                    break;
            }
            else
                temp -= 365; // 平年
            temp1++;
        }
        calendar.w_year = temp1; // 得到年份
        temp1 = 0;
        while (temp >= 28) // 超过了一个月
        {
            if (((calendar.w_year % 4 == 0 && calendar.w_year % 100 != 0) || (calendar.w_year % 400 == 0)) && temp1 == 1) // 当年是不是闰年/2月份
            {
                if (temp >= 29)
                    temp -= 29; // 闰年的秒钟数
                else
                    break;
            }
            else
            {
                if (temp >= mon_table[temp1])
                    temp -= mon_table[temp1]; // 平年
                else
                    break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1; // 得到月份
        calendar.w_date = temp + 1;   // 得到日期
    }
    temp = timecount % 86400;          // 得到秒钟数
    calendar.hour = temp / 3600;       // 小时
    calendar.min = (temp % 3600) / 60; // 分钟
    calendar.sec = (temp % 3600) % 60; // 秒钟
    // calendar.week = RTC_GetWeek(calendar.w_year, calendar.w_month, calendar.w_date); // 获取星期
}
/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
