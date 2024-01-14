/*-------------------------------------------------------------------------------
 *       (C) Copyright 2024, PSYM. All Rights Reserved
 *-------------------------------------------------------------------------------
 * File name   : bsp_DHT22.c
 * Description :
 * Author      : PSYM
 *-------------------------------------------------------------------------------
 * Update note:
 * -----------   -----------   --------------------------------------------------
 *    Date         Author                              Note
 * -----------   -----------   --------------------------------------------------
 * 2024-01-06       PSYM
 *
 **/

/* ---------------------------------- ����ͷ�ļ� ---------------------------------- */
#include "bsp_dht22.h"
#include ".\USART\bsp_usart.h"
#include ".\DELAY\bsp_delay.h"

/* ---------------------------------- ˽�к궨�� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�����Ͷ��� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�б��� ---------------------------------- */
GPIO_InitTypeDef GPIO_InitStruct;

/* ---------------------------------- ��չ���� ---------------------------------- */
/* None. */

/* ---------------------------------- ˽�к���ԭ�� ---------------------------------- */
static void DHT22_SetINPUT(void);
static void DHT22_SetOUTPUT_PP(void);

/* ---------------------------------- ������ ---------------------------------- */

/**
 * @brief ��ʼ��DHT22
 * @param ��
 * @retval ��
 * @note ����DHT22�����ݶ˿�Ϊ���ģʽ��������ߵ�ƽ
 */
void DHT22_Init(DHT22Data_TypeDef *DHT22_Data)
{
    DHT22_SetOUTPUT_PP();                                          // ����Ϊ���ģʽ
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET); // ��Ϊ�͵�ƽ
    HAL_Delay(20);                                                 // ��������18ms
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);   // ��Ϊ�ߵ�ƽ
    Delay_us(30);                                                  // ����20~40us

    // ��ʼ��DHT22_Data�ṹ��
    DHT22_Data->hum_int = 0;
    DHT22_Data->hum_frac = 0;
    DHT22_Data->temp_int = 0;
    DHT22_Data->temp_frac = 0;
    DHT22_Data->checksum = 0;
    DHT22_Data->actual_hum = 0;
    DHT22_Data->actual_temp = 0;
}

/**
 * @brief: ����DHT22-DATA����Ϊ����ģʽ
 * @param: ��
 * @retval:��
 * @note:  ��
 */
static void DHT22_SetINPUT(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief: ����DHT22-DATA����Ϊ�������ģʽ
 * @param: ��
 * @retval:��
 * @note:  ��
 */
static void DHT22_SetOUTPUT_PP(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // �������

    HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);
}

/**
 * ��������: ��DHT22��ȡһ���ֽڣ�MSB(��λ)����
 * �������: ��
 * �� �� ֵ: ��
 * ˵    ������
 */
static uint8_t DHT22_ReadByte(void)
{
    uint8_t i;
    uint8_t temp = 0;

    for (i = 0; i < 8; i++)
    {
        // ��IO�ڼ�⵽����50us�͵�ƽ����26us�ߵ�ƽ����˵����ǰ����λ��'0'
        // ��IO�ڼ�⵽����50us�͵�ƽ����70us�ߵ�ƽ����˵����ǰ����λ��'1'��
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET)
            ;

        Delay_us(50); // ��ʱx us ������ʱ���ȡIO��ƽΪ�ߣ�������1

        if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET) // x us����Ϊ�ߵ�ƽ��ʾ����1
        {
            while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET)
                ; // �ȴ�����1�ĸߵ�ƽ����

            temp |= (uint8_t)(0x01 << (7 - i)); // �ѵ�7-iλ��1
        }
        else // x us��Ϊ�͵�ƽ��ʾ����0
        {
            temp &= (uint8_t) ~(0x01 << (7 - i)); // �ѵ�7-iλ��0
        }
    }
    return temp;
}

/**
 * ��������: ���ݴ���40bit
 * �������: DHT22_Data
 * �� �� ֵ: ERROR:   1
 *          SUCCESS: 0
 * ˵    ����8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��
 */
ErrorStatus DHT22_Read_TempAndHumidity(DHT22Data_TypeDef *DHT22_Data)
{
    uint8_t temp;
    uint16_t actual_hum_temp;                                      // ת���м�ֵ
    DHT22_SetOUTPUT_PP();                                          // ���ģʽ
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET); // �������ͣ���IO��ƽ����
    HAL_Delay(20);                                                 // ��ʱ1ms����
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);   // �������� ������ʱ30us
    Delay_us(30);                                                  // ��ʱ30us
    DHT22_SetINPUT();                                              // ������Ϊ����
    Delay_us(30);

    /*�жϴӻ��Ƿ�����Ӧ�ź� ,80us�͵�ƽ��80us�ߵ�ƽ*/
    if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET)
    {
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET)
            ; // ����80us �͵�ƽ
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET)
            ; // ����80us �ߵ�ƽ�źŽ���

        // ��������
        DHT22_Data->hum_int = DHT22_ReadByte();   // ʪ������
        DHT22_Data->hum_frac = DHT22_ReadByte();  // ʪ��С��
        DHT22_Data->temp_int = DHT22_ReadByte();  // �¶�����
        DHT22_Data->temp_frac = DHT22_ReadByte(); // �¶�С��
        DHT22_Data->checksum = DHT22_ReadByte();  // У��λ

        DHT22_SetOUTPUT_PP(); // ��ȡ���������Ÿ�Ϊ���ģʽ
        // �������ߣ��������50us�͵�ƽ���Զ���������״̬
        HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET);
        Delay_us(60);
        DHT22_SetINPUT(); // ��Ϊ����ģʽ��ʱ����������������ʼ�źţ�ֻ�н��յ�������������ʼ�źŲ��ܻ���

        actual_hum_temp = (DHT22_Data->hum_int << 8) + DHT22_Data->hum_frac;
        DHT22_Data->actual_hum = actual_hum_temp / 10.0;

        actual_hum_temp = ((DHT22_Data->temp_int & 0x7F) << 8) + DHT22_Data->temp_frac;
        DHT22_Data->actual_temp = actual_hum_temp / 10.0;

        /*����ȡ�������Ƿ���ȷ
        �����bit39:31��+��bit30:23��+��bit23:16��+��bit15:8��=��bit7:0�����ʾ���յ���������ȷ��*/
        temp = DHT22_Data->hum_int + DHT22_Data->hum_frac +
               DHT22_Data->temp_int + DHT22_Data->temp_frac;
        if (DHT22_Data->checksum == temp)
        {
            return SUCCESS;
        }
        else
        {
#if DEBUG
            printf("DHT22 У��ʧ��!\n");
#endif
            return ERROR;
        }
    }
    else
    {
#if DEBUG
        printf("DHT22 ����Ӧ�ź�!\n");
#endif
        return ERROR;
    }
}

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
