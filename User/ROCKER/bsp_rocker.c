/**
 *-------------------------------------------------------------------------------
 *       (C) Copyright 2024, PSYM. All Rights Reserved
 *-------------------------------------------------------------------------------
 * @file        : bsp_rocker.c
 * Description  :
 * @author      : PSYM
 *-------------------------------------------------------------------------------
 * Update note:
 * -----------   -----------   --------------------------------------------------
 *    Date         Author                              Note
 * -----------   -----------   --------------------------------------------------
 * 2024-01-10       PSYM
 *
 **/

/* ---------------------------------- 包含头文件 ---------------------------------- */
#include "bsp_rocker.h"
#include "USART/bsp_usart.h"

/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有变量 ---------------------------------- */
uint16_t x_coordinate = 0; // 摇杆X轴坐标
uint16_t y_coordinate = 0; // 摇杆Y轴坐标

/* ---------------------------------- 扩展变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有函数原型 ---------------------------------- */
/* None. */

/* ---------------------------------- 函数体 ---------------------------------- */

/**
 * @brief 将ADC采样值转换为电压值
 *
 * @param reference_voltage 参考电压值，单位mv
 * @param sampling_value ADC采样值
 * @return uint32_t 采样值对应的电压值，单位mv
 */
uint32_t ADC_VoltageValue_Calculate(uint32_t reference_voltage, uint32_t sampling_value)
{
    return (uint32_t)((float)sampling_value / ADC_RES_12BIT * reference_voltage);
}

/**
 * @brief: 摇杆初始化
 * @param: None
 * @return: None
 * @note: 对ADC进行校准，并且ADC转换
 */
void Rocker_Init(void)
{
    // 停止ADC转换
    HAL_ADC_Stop(&ROCKER_HADCx);

    // 校准ADC
    HAL_ADCEx_Calibration_Start(&ROCKER_HADCx);

    // 再次停止ADC转换
    HAL_ADC_Stop(&ROCKER_HADCx);
}

/**
 * @brief 采集一次ADC值
 *
 * @param adc_value 要存放ADC值的变量指针
 */
void Get_ADC_Once(void)
{
    // 启动ADC转换
    HAL_ADC_Start(&ROCKER_HADCx);

    // 等待ADC转换完成（轮询10毫秒）
    HAL_ADC_PollForConversion(&ROCKER_HADCx, 10);

    // 将ADC值写入adc_value
    y_coordinate = HAL_ADC_GetValue(&ROCKER_HADCx);

    // 启动ADC转换
    HAL_ADC_Start(&ROCKER_HADCx);

    // 等待ADC转换完成（轮询10毫秒）
    HAL_ADC_PollForConversion(&ROCKER_HADCx, 10);
    x_coordinate = HAL_ADC_GetValue(&ROCKER_HADCx);

    // 停止ADC转换
    HAL_ADC_Stop(&ROCKER_HADCx);
}

/**
 * @brief: 获取摇杆的方向
 * @param: None
 * @return: ROCKER_DIR 摇杆方向
 * @note: None
 */
uint8_t Rocker_Get_Once_Direction(void)
{
    uint8_t i;
    uint16_t sumX = 0;
    uint16_t sumY = 0;

    // 多次采样并累加
    for (i = 0; i < 10; ++i)
    {
        Get_ADC_Once();
        sumX += x_coordinate;
        sumY += y_coordinate;
    }

    // 计算平均值
    uint16_t avgX = sumX / 10;
    uint16_t avgY = sumY / 10;

    printf("x_coordinate = %d\r\n", avgX);
    printf("y_coordinate = %d\r\n", avgY);

    // 摇杆按键按下
    if (HAL_GPIO_ReadPin(ROCKER_SW_GPIO_Port, ROCKER_SW_Pin) == GPIO_PIN_RESET)
    {
        return ROCKER_PRESS;
    }

    // 判断摇杆方向
    if (avgX > X_RIGHTEDGE)
    {
        return ROCKER_RIGHT;
    }
    else if (avgX < X_LEFTEDGE)
    {
        return ROCKER_LEFT;
    }
    else if (avgY < Y_UPEDGE)
    {
        return ROCKER_UP;
    }
    else if (avgY > Y_DOWNEDGE)
    {
        return ROCKER_DOWN;
    }

    return ROCKER_NONE;
}

/**
 * @brief 获取摇杆方向
 *
 * @param mode 1:连续获取 0:单次获取
 * @return uint8_t 摇杆方向
 */
uint8_t Rocker_Get_Direction(uint8_t mode)
{
    static uint8_t lastDirection = ROCKER_NONE;

    uint8_t currentDirection = Rocker_Get_Once_Direction();

    // 单次获取
    if (mode == 0)
    {
        // 判断是否方向变化
        if (currentDirection != lastDirection)
        {
            lastDirection = currentDirection;
            return currentDirection;
        }
    }
    else if (mode == 1)
    {
        return currentDirection;
    }

    return ROCKER_NONE; // 默认返回
}

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
