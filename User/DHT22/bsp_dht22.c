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

/* ---------------------------------- 包含头文件 ---------------------------------- */
#include "bsp_dht22.h"
#include ".\USART\bsp_usart.h"
#include ".\DELAY\bsp_delay.h"

/* ---------------------------------- 私有宏定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有类型定义 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有变量 ---------------------------------- */
GPIO_InitTypeDef GPIO_InitStruct;

/* ---------------------------------- 扩展变量 ---------------------------------- */
/* None. */

/* ---------------------------------- 私有函数原型 ---------------------------------- */
static void DHT22_SetINPUT(void);
static void DHT22_SetOUTPUT_PP(void);

/* ---------------------------------- 函数体 ---------------------------------- */

/**
 * @brief 初始化DHT22
 * @param 无
 * @retval 无
 * @note 设置DHT22的数据端口为输出模式，并输出高电平
 */
void DHT22_Init(DHT22Data_TypeDef *DHT22_Data)
{
    DHT22_SetOUTPUT_PP();                                          // 配置为输出模式
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET); // 设为低电平
    HAL_Delay(20);                                                 // 下拉至少18ms
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);   // 设为高电平
    Delay_us(30);                                                  // 上拉20~40us

    // 初始化DHT22_Data结构体
    DHT22_Data->hum_int = 0;
    DHT22_Data->hum_frac = 0;
    DHT22_Data->temp_int = 0;
    DHT22_Data->temp_frac = 0;
    DHT22_Data->checksum = 0;
    DHT22_Data->actual_hum = 0;
    DHT22_Data->actual_temp = 0;
}

/**
 * @brief: 设置DHT22-DATA引脚为输入模式
 * @param: 无
 * @retval:无
 * @note:  无
 */
static void DHT22_SetINPUT(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);
}

/**
 * @brief: 设置DHT22-DATA引脚为推挽输出模式
 * @param: 无
 * @retval:无
 * @note:  无
 */
static void DHT22_SetOUTPUT_PP(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出

    HAL_GPIO_Init(DHT22_GPIO_Port, &GPIO_InitStruct);
}

/**
 * 函数功能: 从DHT22读取一个字节，MSB(高位)先行
 * 输入参数: 无
 * 返 回 值: 无
 * 说    明：无
 */
static uint8_t DHT22_ReadByte(void)
{
    uint8_t i;
    uint8_t temp = 0;

    for (i = 0; i < 8; i++)
    {
        // 若IO口检测到先是50us低电平后是26us高电平，则说明当前数据位是'0'
        // 若IO口检测到先是50us低电平后是70us高电平，则说明当前数据位是'1'。
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET)
            ;

        Delay_us(50); // 延时x us ，若延时后读取IO电平为高，即数据1

        if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET) // x us后仍为高电平表示数据1
        {
            while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET)
                ; // 等待数据1的高电平结束

            temp |= (uint8_t)(0x01 << (7 - i)); // 把第7-i位置1
        }
        else // x us后为低电平表示数据0
        {
            temp &= (uint8_t) ~(0x01 << (7 - i)); // 把第7-i位置0
        }
    }
    return temp;
}

/**
 * 函数功能: 数据传输40bit
 * 输入参数: DHT22_Data
 * 返 回 值: ERROR:   1
 *          SUCCESS: 0
 * 说    明：8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验
 */
ErrorStatus DHT22_Read_TempAndHumidity(DHT22Data_TypeDef *DHT22_Data)
{
    uint8_t temp;
    uint16_t actual_hum_temp;                                      // 转化中间值
    DHT22_SetOUTPUT_PP();                                          // 输出模式
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET); // 主机拉低，即IO电平拉低
    HAL_Delay(20);                                                 // 延时1ms以上
    HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_SET);   // 总线拉高 主机延时30us
    Delay_us(30);                                                  // 延时30us
    DHT22_SetINPUT();                                              // 主机设为输入
    Delay_us(30);

    /*判断从机是否有响应信号 ,80us低电平，80us高电平*/
    if (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET)
    {
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_RESET)
            ; // 接收80us 低电平
        while (HAL_GPIO_ReadPin(DHT22_GPIO_Port, DHT22_Pin) == GPIO_PIN_SET)
            ; // 接收80us 高电平信号结束

        // 接收数据
        DHT22_Data->hum_int = DHT22_ReadByte();   // 湿度整数
        DHT22_Data->hum_frac = DHT22_ReadByte();  // 湿度小数
        DHT22_Data->temp_int = DHT22_ReadByte();  // 温度整数
        DHT22_Data->temp_frac = DHT22_ReadByte(); // 温度小数
        DHT22_Data->checksum = DHT22_ReadByte();  // 校验位

        DHT22_SetOUTPUT_PP(); // 读取结束，引脚改为输出模式
        // 主机拉高，继续输出50us低电平后自动进入休眠状态
        HAL_GPIO_WritePin(DHT22_GPIO_Port, DHT22_Pin, GPIO_PIN_RESET);
        Delay_us(60);
        DHT22_SetINPUT(); // 变为输入模式随时接收主机发来的起始信号，只有接收到主机发来的起始信号才能唤醒

        actual_hum_temp = (DHT22_Data->hum_int << 8) + DHT22_Data->hum_frac;
        DHT22_Data->actual_hum = actual_hum_temp / 10.0;

        actual_hum_temp = ((DHT22_Data->temp_int & 0x7F) << 8) + DHT22_Data->temp_frac;
        DHT22_Data->actual_temp = actual_hum_temp / 10.0;

        /*检查读取的数据是否正确
        如果（bit39:31）+（bit30:23）+（bit23:16）+（bit15:8）=（bit7:0）则表示接收到的数据正确。*/
        temp = DHT22_Data->hum_int + DHT22_Data->hum_frac +
               DHT22_Data->temp_int + DHT22_Data->temp_frac;
        if (DHT22_Data->checksum == temp)
        {
            return SUCCESS;
        }
        else
        {
#if DEBUG
            printf("DHT22 校验失败!\n");
#endif
            return ERROR;
        }
    }
    else
    {
#if DEBUG
        printf("DHT22 无响应信号!\n");
#endif
        return ERROR;
    }
}

/*>>>>>>>>>> (C) COPYRIGHT PSYM <<<<<< >>>>>> END OF FILE <<<<<<<<<<*/
