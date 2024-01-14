#ifndef __ONEWIRE_DHT22_H__
#define __ONEWIRE_DHT22_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"

/* ���Ͷ��� ------------------------------------------------------------------*/

typedef struct
{
    uint8_t hum_int;   // ʪ����������
    uint8_t hum_frac;  // ʪ��С������
    uint8_t temp_int;  // �¶���������
    uint8_t temp_frac; // �¶�С������
    uint8_t checksum;  // У���
    float actual_hum;  // ʵ��ʪ��
    float actual_temp; // ʵ���¶�
} DHT22Data_TypeDef;

/* �궨�� -------------------------------------------------------------------*/

/* ��չ���� ------------------------------------------------------------------*/

/* �������� ------------------------------------------------------------------*/
void DHT22_Init(DHT22Data_TypeDef *DHT22_Data);
ErrorStatus DHT22_Read_TempAndHumidity(DHT22Data_TypeDef *DHT22_Data);

#endif /* __ONEWIRE_DHT22_H__ */
