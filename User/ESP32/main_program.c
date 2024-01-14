#include "main_program.h"
#include <stdio.h>
//#include "time_handle.h"
//#include "oled.h"
//#include "dht11.h"
#include "esp8266.h"
#include "led.h"

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
extern uint16_t index_oled;
extern uint16_t index_dht11;
extern uint8_t index_send_msg;
extern uint8_t index_led;
/*
*************************************
函数声明
*************************************
*/

/*
*************************************
变量定义
*************************************
*/
uint8_t led_status = 0;
uint8_t temp_value = 0;
uint8_t humi_value = 0;
/*
*************************************
函数定义
*************************************
*/

/**
  * @brief          初始化函数
  * @param[in]      none
  * @retval         none
  */
void user_init_program(void)
{
//  //OLED_init(); 
  esp8266_init();
//  while(dht11_init())
//  {
//    HAL_Delay(500);
//    printf("dht11 init faild\r\n");
//   // OLED_printf (0,0,"dht11 init faild");
//  }
// // time_slot_start();
}
/**
  * @brief          初始化函数
  * @param[in]      none
  * @retval         none
  */
void user_main_program(void)
{
  if(esp8266_receive_msg()!=1)
  {
  }
  if(index_dht11 ==1)
  {
   //dht11_read_data(&temp_value,&humi_value);
    index_dht11=0;
  }
  if(index_led==1)
  {
    set_led(led_status);
    index_led=0;
  }
  if(index_send_msg==1)
	{
		index_send_msg =0;
		if(esp8266_send_msg()==0)
		{
		}
		else
		{
			printf("msg send error\r\n");
		}
	}
}

