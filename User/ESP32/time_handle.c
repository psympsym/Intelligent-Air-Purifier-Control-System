#include "time_handle.h"
#include "main.h"
#include "tim.h"
#include <stdio.h>

/*
*************************************
宏定义
*************************************
*/
/*
*************************************
宏定义
*************************************
*/
/*
*************************************
变量声明
*************************************
*/
/*
*************************************
变量定义
*************************************
*/
static uint16_t index_10ms = 0;
uint16_t index_oled = 0;
uint16_t index_dht11 = 0;
uint8_t index_send_msg = 0;
uint8_t index_led = 0;

/**
  * @brief          定时器2中断服务函数,10ms一次中断
  * @param[in]      htim:定时器
  * @retval         none
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)	
{
	if (htim->Instance == htim2.Instance)
	{
		index_10ms++;	 
    if(index_10ms%100==0)
    {
      index_oled=1;
      index_dht11=1;
      index_led=1;
    }
    if(index_10ms >= 500)	
		{
			index_send_msg=1;
			index_10ms = 0;
		}
	}

}
/**
  * @brief          开启定时器2
  * @param[in]      none
  * @retval         none
  */
void time_slot_start(void)
{
  HAL_TIM_Base_Start_IT(&htim2);	//打开定时器2中断
}

	


