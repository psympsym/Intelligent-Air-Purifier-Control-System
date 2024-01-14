#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"
void uart2_receiver_handle(void);
uint8_t esp8266_receive_msg(void);
uint8_t esp8266_send_msg(void);
void esp8266_init(void);
#endif

