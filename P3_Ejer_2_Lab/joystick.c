#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "main.h"
#include "joystick.h"
#include "stm32f4xx_hal.h"
//#include "stm32f4xx_it.h"

/*-------------------------------------------------------------------------------
 *      Thread 'Thjoy': Hilo que gestiona las interrupciones del joystick
 *-----------------------------------------------------------------------------*/
osThreadId_t tid_Thjoy;                        // Thjoy id

/*------------------------------------------------------------------
            Variables del timer virtual
 -----------------------------------------------------------------*/
static osTimerId_t timer_rebotes;
static uint32_t exec;
static osStatus_t status;

/*------------------------------------------------------------------
                Otras variables
 -----------------------------------------------------------------*/

/*------------------------------------------------------------------
            Funciones del Thread
 -----------------------------------------------------------------*/
void Thjoy (void *argument);                   // Thjoy function
 
int Init_Joystick (void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  tid_Thjoy = osThreadNew(Thjoy, NULL, NULL);
  if (tid_Thjoy == NULL) {
    return(-1);
  }
  // Configuracion del joystick
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;      // Configuro para que detecte ambas
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_10;                 //Derecha | Arriba
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);                          //Puertos B

  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_12;   //Centro | Izquierda | Abajo
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);                          // Importante que sean los puertos E
  
  GPIO_InitStruct.Pin = GPIO_PIN_13;                               // Usuario
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);                          //Puertos C
  
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  
  return(0);
}

/*------------------------------------------------------------------
            Rutina de atencion a la interrupción
 -----------------------------------------------------------------*/
void EXTI15_10_IRQHandler(void){
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);		// Arriba
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);		// Derecha
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);		// Abajo
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);		// Izquierda
//  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);		// Centro
  
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);      // Usuario
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
  if(GPIO_PIN == GPIO_PIN_10 || GPIO_PIN == GPIO_PIN_11 || GPIO_PIN == GPIO_PIN_12 || GPIO_PIN == GPIO_PIN_14 || GPIO_PIN == GPIO_PIN_13){    // Arriba, Derecha, Abajo, Izquierda, Centro y Usuario
    osThreadFlagsSet(tid_Thjoy, 1);    // Activamos un flag para que detecte que se pulsado el joystick
  }
}

/*------------------------------------------------------------------
          Callback del timer virtual para los rebotes
 -----------------------------------------------------------------*/
static void Timer_Bounce_Callback(void const *arg)      // Callback creada para el timer virtual
{
    if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 1 ){      //Arriba

    }
    else if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1 ){  //Derecha

    }
    else if( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == 1 ){  //Abajo

    }
    else if( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == 1 ){  //Izquierda

    }
    else if( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == 1 ){  //Centro

    }
    else if( HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 1 ){		//Usuario
			Set_RTC_Time(0, 0, 0);
			Set_RTC_Date(0, 1, 1, 1);
    }

}

/*------------------------------------------------------------------
          Iniciacion del timer virtual para rebotes
 -----------------------------------------------------------------*/
int Init_timer (void)
{
  exec = 2U;
  timer_rebotes = osTimerNew((osTimerFunc_t)&Timer_Bounce_Callback, osTimerOnce, &exec, NULL);
  if(timer_rebotes != NULL){
    if( status != osOK){
      return -1;
    }
  }
  return NULL;
}

/*------------------------------------------------------------------
            Thread en ejecución
 -----------------------------------------------------------------*/
void Thjoy (void *argument) {
  Init_timer();
  
  while (1) {
    osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);
    osTimerStart(timer_rebotes, 50);    // Esperamos 50 ms

  }
}
