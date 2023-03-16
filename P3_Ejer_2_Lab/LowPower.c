#include "LowPower.h"
#include "leds.h"

/****************************************
			id del Thread
****************************************/
osThreadId_t tid_ThLowPower;  
osThreadId_t tid_ThLowPower_Parpadeo;

RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
//static void SYSCLKConfig_STOP(void);
void LowPower_Thread (void *argument); 
void LowPower_ParpadeoThread (void *argument);                   // thread function


/**
  * @brief  This function configures the system to enter Sleep mode for
  *         current consumption measurement purpose.
  *         Sleep Mode
  *         ==========  
  *            - System Running at PLL (100MHz)
  *            - Flash 3 wait state
  *            - Instruction and Data caches ON
  *            - Prefetch ON   
  *            - Code running from Internal FLASH
  *            - All peripherals disabled.
  *            - Wakeup using EXTI Line (user Button)
  * @param  None
  * @retval None
  */
void SleepMode_Measure(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Disable USB Clock */
  __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

  /* Disable Ethernet Clock */
  __HAL_RCC_ETH_CLK_DISABLE();

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
  //HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  //HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct); 
//  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
//  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct); 
//  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  //__HAL_RCC_GPIOB_CLK_DISABLE();
  //__HAL_RCC_GPIOC_CLK_DISABLE();
  //__HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();
  __HAL_RCC_GPIOJ_CLK_DISABLE();
  __HAL_RCC_GPIOK_CLK_DISABLE();


	LED_Init();
	PushButtonPins_Init();

  /* Configure user Button */
  //BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Suspend Tick increment to prevent wakeup by Systick interrupt. 
     Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base) */
  HAL_SuspendTick();

  /* Request to enter SLEEP mode */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

  /* Resume Tick interrupt if disabled prior to sleep mode entry */
  HAL_ResumeTick();
  
  /* Exit Ethernet Phy from LowPower mode */
  //ETH_PhyExitFromPowerDownMode();
	
}

//static void SYSCLKConfig_STOP(void)
//{
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  uint32_t pFLatency = 0;
//  
//  /* Get the Oscillators configuration according to the internal RCC registers */
//  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
//  
//  /* After wake-up from STOP reconfigure the system clock: Enable HSE and PLL */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();				// Mirar de que fichero de cabecera es
//  }

//  /* Get the Clocks configuration according to the internal RCC registers */
//  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);
//  
//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
//     clocks dividers */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}

/**************************************************************************************
			Init modo de bajo consumo
***************************************************************************************/
int Init_ThLowPower(void) {
  tid_ThLowPower = osThreadNew(LowPower_Thread, NULL, NULL);
  if (tid_ThLowPower == NULL) {
    return(-1);
  }
	
  return(0);
}

/**************************************************************************************

***************************************************************************************/
void LowPower_Thread(void *argument) 
{
	osDelay(15000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	osThreadFlagsSet(tid_ThLowPower_Parpadeo, 1);
	SleepMode_Measure();
	
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == 0){

	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
}

/**************************************************************************************
			Init parpadeo leds en modo de bajo consumo
***************************************************************************************/
int Init_LowPower_Parpadeo (void) {
 
  tid_ThLowPower_Parpadeo = osThreadNew(LowPower_ParpadeoThread, NULL, NULL);
  if (tid_ThLowPower_Parpadeo == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void LowPower_ParpadeoThread (void *argument) {
 
  while (1) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		osDelay(100);	
		osThreadYield();
  }
}
