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

/* ---------------------------------- ����ͷ�ļ� ---------------------------------- */
#include "bsp_rocker.h"
#include "USART/bsp_usart.h"

/* ---------------------------------- ˽�к궨�� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�����Ͷ��� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�б��� ---------------------------------- */
uint16_t x_coordinate = 0; // ҡ��X������
uint16_t y_coordinate = 0; // ҡ��Y������

/* ---------------------------------- ��չ���� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�к���ԭ�� ---------------------------------- */
/* None. */

/* ---------------------------------- ������ ---------------------------------- */

/**
 * @brief ��ADC����ֵת��Ϊ��ѹֵ
 *
 * @param reference_voltage �ο���ѹֵ����λmv
 * @param sampling_value ADC����ֵ
 * @return uint32_t ����ֵ��Ӧ�ĵ�ѹֵ����λmv
 */
uint32_t ADC_VoltageValue_Calculate(uint32_t reference_voltage, uint32_t sampling_value)
{
    return (uint32_t)((float)sampling_value / ADC_RES_12BIT * reference_voltage);
}

/**
 * @brief: ҡ�˳�ʼ��
 * @param: None
 * @return: None
 * @note: ��ADC����У׼������ADCת��
 */
void Rocker_Init(void)
{
    // ֹͣADCת��
    HAL_ADC_Stop(&ROCKER_HADCx);

    // У׼ADC
    HAL_ADCEx_Calibration_Start(&ROCKER_HADCx);

    // �ٴ�ֹͣADCת��
    HAL_ADC_Stop(&ROCKER_HADCx);
}

/**
 * @brief �ɼ�һ��ADCֵ
 *
 * @param adc_value Ҫ���ADCֵ�ı���ָ��
 */
void Get_ADC_Once(void)
{
    // ����ADCת��
    HAL_ADC_Start(&ROCKER_HADCx);

    // �ȴ�ADCת����ɣ���ѯ10���룩
    HAL_ADC_PollForConversion(&ROCKER_HADCx, 10);

    // ��ADCֵд��adc_value
    y_coordinate = HAL_ADC_GetValue(&ROCKER_HADCx);

    // ����ADCת��
    HAL_ADC_Start(&ROCKER_HADCx);

    // �ȴ�ADCת����ɣ���ѯ10���룩
    HAL_ADC_PollForConversion(&ROCKER_HADCx, 10);
    x_coordinate = HAL_ADC_GetValue(&ROCKER_HADCx);

    // ֹͣADCת��
    HAL_ADC_Stop(&ROCKER_HADCx);
}

/**
 * @brief: ��ȡҡ�˵ķ���
 * @param: None
 * @return: ROCKER_DIR ҡ�˷���
 * @note: None
 */
uint8_t Rocker_Get_Once_Direction(void)
{
    uint8_t i;
    uint16_t sumX = 0;
    uint16_t sumY = 0;

    // ��β������ۼ�
    for (i = 0; i < 10; ++i)
    {
        Get_ADC_Once();
        sumX += x_coordinate;
        sumY += y_coordinate;
    }

    // ����ƽ��ֵ
    uint16_t avgX = sumX / 10;
    uint16_t avgY = sumY / 10;

    printf("x_coordinate = %d\r\n", avgX);
    printf("y_coordinate = %d\r\n", avgY);

    // ҡ�˰�������
    if (HAL_GPIO_ReadPin(ROCKER_SW_GPIO_Port, ROCKER_SW_Pin) == GPIO_PIN_RESET)
    {
        return ROCKER_PRESS;
    }

    // �ж�ҡ�˷���
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
 * @brief ��ȡҡ�˷���
 *
 * @param mode 1:������ȡ 0:���λ�ȡ
 * @return uint8_t ҡ�˷���
 */
uint8_t Rocker_Get_Direction(uint8_t mode)
{
    static uint8_t lastDirection = ROCKER_NONE;

    uint8_t currentDirection = Rocker_Get_Once_Direction();

    // ���λ�ȡ
    if (mode == 0)
    {
        // �ж��Ƿ���仯
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

    return ROCKER_NONE; // Ĭ�Ϸ���
}

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
