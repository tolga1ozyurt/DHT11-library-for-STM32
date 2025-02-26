/*
 * DHT11.c
 *
 *  Created on: Feb 26, 2025
 *      Author: Tolga Kaan Özyurt
 */
#include <DHT11.h>


TIM_HandleTypeDef htim3;
uint8_t response=0,Humidity=0,Temperature=0;
uint16_t tempVal=0,humVal=0;
uint8_t dhtVal[2];
uint8_t mData[40];
uint16_t mTime1 = 0, mTime2 = 0;
uint16_t mbit = 0;
uint8_t  parityVal = 0, genParity = 0;



void delay_u(uint16_t time){ //us sayılacak delay

//	uint16_t tickstart = __HAL_TIM_GET_COUNTER(&htim3);
//	  uint16_t wait = time;
//	  while((__HAL_TIM_GET_COUNTER(&htim3) - tickstart) < wait){  }


	__HAL_TIM_SET_COUNTER(&htim3,0);
	while(__HAL_TIM_GET_COUNTER(&htim3) < time); //burada takılı kalıyor???
}


void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef DHT11_Data = {0};
	DHT11_Data.Pin = GPIO_Pin;
	DHT11_Data.Mode = GPIO_MODE_OUTPUT_PP;
	DHT11_Data.Pull = GPIO_NOPULL;
	DHT11_Data.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &DHT11_Data);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef DHT11_Data = {0};
	DHT11_Data.Pin = GPIO_Pin;
	DHT11_Data.Mode = GPIO_MODE_INPUT;
	DHT11_Data.Pull = GPIO_NOPULL;
	DHT11_Data.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &DHT11_Data);
}

uint8_t DHT11_Read (void){

  for(int a=0;a<40;a++)
	  mData[a]=0;			//resetting all values

   mTime1 = 0, mTime2 = 0, response=0, tempVal=0, humVal=0, parityVal = 0, genParity = 0,  mbit = 0;

     Set_Pin_Output(DHT11_PORT,DHT11_PIN);
	 HAL_GPIO_WritePin(DHT11_PORT,DHT11_PIN,GPIO_PIN_RESET); //waking up the DHT11
    delay_u(18000);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, SET);			//waking up the DHT11
    delay_u(40);

   	Set_Pin_Input(DHT11_PORT,DHT11_PIN);					//start to retrieve response from DHT11

	 __HAL_TIM_SET_COUNTER(&htim3, 0);				//set timer counter to zero
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) {
		if((uint16_t)__HAL_TIM_GET_COUNTER(&htim3) > 500)
			return 0;}

	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET) {
		if((uint16_t)__HAL_TIM_GET_COUNTER(&htim3) > 500)
			return 0;}
	mTime1 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);

	__HAL_TIM_SET_COUNTER(&htim3, 0);
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) {
		if((uint16_t)__HAL_TIM_GET_COUNTER(&htim3) > 500)
			return 0;}
    mTime2 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);


	if(mTime1 < 75 && mTime1 > 85 && mTime2 < 75 && mTime2 > 85)
	{

		return 0;
	}




	for(int j = 0; j < 40; j++)
	{
		__HAL_TIM_SET_COUNTER(&htim3, 0);
		while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET)
			{if((uint16_t)__HAL_TIM_GET_COUNTER(&htim3) > 500)
				return 0;}
		__HAL_TIM_SET_COUNTER(&htim3, 0);
		while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
			{if((uint16_t)__HAL_TIM_GET_COUNTER(&htim3) > 500)
				return 0;}
		mTime1 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim3);

		//check pass time in high state
		//if pass time 25uS set as LOW
		if(mTime1 > 20 && mTime1 < 30)
		{
			mbit = 0;
		}
		else if(mTime1 > 60 && mTime1 < 80) //if pass time 70 uS set as HIGH
		{
			 mbit = 1;
		}

		//set i th data in data buffer
		mData[j] = mbit;

	}


	for(int i = 0; i < 8; i++) //Humidity value buffer
	{
		humVal += mData[i];
		humVal = humVal << 1;
	}


	for(int i = 16; i < 24; i++) //Temperature value buffer
	{
		tempVal += mData[i];
		tempVal = tempVal << 1;
	}


	for(int i = 32; i < 40; i++) //Checksum
	{
		parityVal += mData[i];
		parityVal = parityVal << 1;
	}

	parityVal = parityVal >> 1;
	humVal = humVal >> 1;
	tempVal = tempVal >> 1;

	genParity = humVal + tempVal;


	dhtVal[0]= tempVal;
	dhtVal[1] = humVal;

	return 1;
}

int getTemp(void)
{
	return Temperature = dhtVal[0];
}

int getHum(void)
{
	return Humidity = dhtVal[1];
}
