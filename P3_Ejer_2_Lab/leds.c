#include "leds.h"

static GPIO_InitTypeDef GPIO_InitStruct;

void LED_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP; /*Elegimos modo Pull Up*/
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /*Elegimos la velocidad*/
		
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	// Para los leds del RGB
	GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; // Azul, Verde y Rojo
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);   // Cambiamos esta parte del codigo porque estamos usando los pines del puerto D para conectarlo a la tarjeta mbed
	
	// Los pines del RGB funcionan al reves
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
}
