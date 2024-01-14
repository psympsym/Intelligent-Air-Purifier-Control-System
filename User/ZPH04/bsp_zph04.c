/*-------------------------------------------------------------------------------
 *       (C) Copyright 2024, PSYM. All Rights Reserved
 *-------------------------------------------------------------------------------
 * File name   : ZPH04.c
 * Description :
 * Author      : PSYM
 *-------------------------------------------------------------------------------
 * Update note:
 * -----------   -----------   --------------------------------------------------
 *    Date         Author                              Note
 * -----------   -----------   --------------------------------------------------
 * 2024-01-07       PSYM
 *
 **/

/* ---------------------------------- 包含头文件 ---------------------------------- */
#include ".\ZPH04\bsp_zph04.h"

/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 扩展变量 ---------------------------------- */
COUNT_TEMP count_temp = {0};

/* ---------------------------------- 私有函数原型 ---------------------------------- */

/* ---------------------------------- 函数体 ---------------------------------- */

void ZPH04_Init(void)
{
    count_temp.flg = 0;
    count_temp.num_high = 0;
    count_temp.num_low = 0;
    HAL_TIM_Base_Start_IT(&ZPH04_HTIMx);
}

/**
 * @brief 获取一次PM2.5值
 * @param None
 * @retval PM2.5值
 * @note 该函数会以非阻塞模式获取一次PM2.5值
 *       LT 一个周期内低电平的脉宽（单位：ms）;
 *       UT 一个周期的脉宽（单位：ms）;
 *       如果未获取到返回0xFF，否则返回一个周期内低电平的比例
 *       由于低脉冲率RT：RT = LT / UT × 100%;
 *       UT = 1000ms, 单次溢出周期为10ms，所以直接返回溢出次数即可。
 *       低脉冲率与浓度成10倍关系。例：低脉冲率为10%，对应浓度为100ug/m^3
 */
uint16_t ZPH04_GetPM2_5(void)
{
    uint16_t cnt = 0;
    if (count_temp.flg == 1)
    {
        cnt = count_temp.num_low;
        count_temp.flg = 0;
        count_temp.num_low = 0;
        count_temp.num_high = 0;
        return cnt;
    }
    return 0xFF;
}

/**
 * @brief 定时器溢出中断回调函数
 *
 * @param htim
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ZPH04_TIMx)
    {
        if (ZPH04_ISLOW())
        {
            count_temp.num_low++;
        }
        else if (ZPH04_ISHIGH())
        {
            count_temp.num_high++;
        }
        if (count_temp.num_high + count_temp.num_low >= 1000)
        {
            count_temp.flg = 1;
        }
    }
}

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
