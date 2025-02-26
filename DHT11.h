/*
 * DHT11.h
 *
 *  Created on: Feb 26, 2025
 *      Author: Tolga Kaan Özyurt
 *
 *      README
 *
 *      Please take notice that this library was prepared with an STM32G031 Nucleo Board. PSC and ARR
 *      values were determined according to the 16MHz for microseconds counting.
 *
 *      Clock Frequency: 16MHz
 *      Timer:			 TIM3
 *      DHT11 Data Pin:  PA0
 *
 *      Please adjust your settings according to parameters above.
 *
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32g0xx_hal.h"

#define DHT11_PORT GPIOA					//Set according to user's preference
#define DHT11_PIN GPIO_PIN_0				//Set according to user's preference
extern TIM_HandleTypeDef htim3;				//Set according to user's preference

extern uint8_t response,Humidity,Temperature;
extern uint16_t tempVal,humVal;
extern uint8_t dhtVal[2];
extern uint8_t mData[40];
extern uint16_t mTime1, mTime2;
extern uint16_t mbit;
extern uint8_t  parityVal, genParity;


void delay_u(uint16_t time);
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t DHT11_Read (void);


#endif /* INC_DHT11_H_ */
