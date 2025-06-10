#ifndef __APP_H
#define __APP_H

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include "DHT11.h"
#include "hx711.h"
#include "Delay.h"
#include "usart.h"
#include "gizwits_protocol.h"
#include "gizwits_product.h"
#include "common.h"
#include "usart3.h"
#include "Timer.h"
#include "Motor.h"
#include "relay.h"

void app_init(void);
void app_run(void);


#endif
