/*==============================================================================
 *            HAL-driver: Blink a LED is Button is pressed
 *==============================================================================
 *--------------+
 * Description  |
 *--------------+
 *  Read the state of a button. If it's pressed then turn led on, if not turn led off.
 *
 *--------------+
 * Hardware     |
 *--------------+
 *
 *  +------------------------+
 *  |                        |
 *  |                Led pin +- - - - -|>|- - - GND
 *  |  STM32                 |
 *  |                        |         ___
 *  |                Btn pin +- - - - -o o- - - GND
 *  |                        |
 *  +------------------------+
 *
 *
 * */

#include "stm32f1xx.h"

#define userBtn_Pin 		GPIO_PIN_15
#define userBtn_Port 	GPIOC
#define userLED_Pin 		GPIO_PIN_14
#define userLED_GPIO_Port 	GPIOC

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void _Error_Handler(char *file, int line);
void Main_Function();

void Main_Function()
{
	GPIO_PinState ledState, btnState;

	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();

	for(;;) {
		ledState = HAL_GPIO_ReadPin(userLED_GPIO_Port, userLED_Pin);
		btnState = HAL_GPIO_ReadPin(userBtn_Port, userBtn_Pin);

		if(btnState == GPIO_PIN_SET && ledState == GPIO_PIN_RESET) {
			HAL_GPIO_WritePin(userLED_GPIO_Port, userLED_Pin, GPIO_PIN_SET);
		}
		if(btnState == GPIO_PIN_RESET && ledState == GPIO_PIN_SET) {
			HAL_GPIO_WritePin(userLED_GPIO_Port, userLED_Pin, GPIO_PIN_RESET);
		}
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
	_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(userLED_GPIO_Port, userLED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : userBtn_Pin */
  GPIO_InitStruct.Pin = userBtn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(userBtn_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : userLED_Pin */
  GPIO_InitStruct.Pin = userLED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(userLED_GPIO_Port, &GPIO_InitStruct);
}

void _Error_Handler(char *file, int line)
{
  while(1);
}

