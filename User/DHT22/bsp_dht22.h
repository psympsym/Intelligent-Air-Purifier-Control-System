#ifndef __ONEWIRE_DHT22_H__
#define __ONEWIRE_DHT22_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/

typedef struct
{
    uint8_t hum_int;   // 湿度整数部分
    uint8_t hum_frac;  // 湿度小数部分
    uint8_t temp_int;  // 温度整数部分
    uint8_t temp_frac; // 温度小数部分
    uint8_t checksum;  // 校验和
    float actual_hum;  // 实际湿度
    float actual_temp; // 实际温度
} DHT22Data_TypeDef;

/* 宏定义 -------------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/
void DHT22_Init(DHT22Data_TypeDef *DHT22_Data);
ErrorStatus DHT22_Read_TempAndHumidity(DHT22Data_TypeDef *DHT22_Data);

#endif /* __ONEWIRE_DHT22_H__ */
