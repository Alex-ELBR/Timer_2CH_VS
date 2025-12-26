#include <main.h>
#include <main_loop.h>



void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(I2C_HandleTypeDef *i2c_instance);

/*** прототипы функций задач *********************************************************** */
void update_display(void);
void update_button(void);
void update_led(void);
void get_time_rtc(void);

void led_exception(); /* Индикация зависания какой-либо задачи */


/*************************************************************************************** */
I2C_HandleTypeDef hi2c1;                // шина I2C для обмена данными с часами и памятью

eDispatcher dispatcher;                 // объект диспетчера задач

Display displ;
eRTC rtc(&hi2c1, ADDRESS_RTC);          //обьект часов
eEEPROM eeprom(&hi2c1, ADDRESS_EEPROM); //обьект микросхемы памяти
EButton keyboard;
eChannel channel[CHANNEL_AMOUNT];

ELed led_1(LED_1_PORT, LED_1_PIN);
ELed led_2(LED_2_PORT, LED_2_PIN);
ELed led_3(LED_3_PORT, LED_3_PIN);
/*************************************************************************************** */

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_I2C1_Init(&hi2c1);
    MX_GPIO_Init();

    HAL_Delay(1000);

    dispatcher.add_task(main_loop, 1);
    dispatcher.add_task(update_display, 1);
    dispatcher.add_task(update_button, 1);
    dispatcher.add_task(update_led, 500);
    dispatcher.add_task(get_time_rtc, 50);

    while(1)
    {
        dispatcher.execution();
    }
};

/** РЕАЛИЗАЦИЯ ФУНКЦИЙ ЗАДАЧ ********************************************************/
/////////////////////////////////////////////////////////////////
void update_display(void)
{
  displ.display_update();
};

/////////////////////////////////////////////////////////////////
void update_button(void)
{
  keyboard.button_update();
}

/////////////////////////////////////////////////////////////////
void update_led(void)
{
  led_1.periodic();
  led_2.periodic();
  led_3.periodic();
}

/////////////////////////////////////////////////////////////////
void get_time_rtc(void)
{
  rtc.periodic();
}



/*** Индикация зависания какой-либо задачи ******************************************************* */
void led_exception()
{
  static uint32_t timeKeep = HAL_GetTick();

  bcd8_level_t stuck_task_digit = bin8_trans(dispatcher.get_current_task());

  char i[] = {'E', '-', stuck_task_digit.tens, stuck_task_digit.units};
  displ.show(i);

  if(HAL_GetTick() - timeKeep > 200)
  {
      timeKeep = HAL_GetTick();
      HAL_GPIO_TogglePin(LED_1_PORT, LED_1_PIN);
  }

  if(HAL_GetTick() & 1)
  {
    displ.display_update();
  }
}

/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;


  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  
}

/*****************************************************************************************************
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(I2C_HandleTypeDef *i2c_instance)
{
   __HAL_RCC_I2C1_CLK_ENABLE();
  HAL_Delay(100);
  __HAL_RCC_I2C1_FORCE_RESET();
  HAL_Delay(100);
  __HAL_RCC_I2C1_RELEASE_RESET();
  HAL_Delay(100);

  i2c_instance->Instance = I2C1;
  i2c_instance->Init.ClockSpeed = 400000;
  i2c_instance->Init.DutyCycle = I2C_DUTYCYCLE_2;
  i2c_instance->Init.OwnAddress1 = 0;
  i2c_instance->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  i2c_instance->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  i2c_instance->Init.OwnAddress2 = 0;
  i2c_instance->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  i2c_instance->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(i2c_instance) != HAL_OK)
  {
    while(1);
  }

}

/*****************************************************************************************************
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISP_SEG_F_PIN|LED_1_PIN|LED_2_PIN|LED_3_PIN
                          |DISP_SEG_G_PIN|DISP_SEG_C_PIN|DISP_SEG_DP_PIN|DISP_SEG_E_PIN
                          |DISP_SEG_B_PIN|RELAY_2_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP_SEG_A_PIN|DISP_SEG_D_PIN|DISP_DIG_1_PIN|DISP_DIG_2_PIN
                          |DISP_DIG_3_PIN|DISP_DIG_4_PIN|RELAY_1_PIN, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = DISP_SEG_F_PIN|LED_1_PIN|LED_2_PIN|LED_3_PIN
                          |DISP_SEG_G_PIN|DISP_SEG_C_PIN|DISP_SEG_DP_PIN|DISP_SEG_E_PIN
                          |DISP_SEG_B_PIN|RELAY_2_PIN;
                          
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUT_OK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUT_UP_PIN|BUT_DOWN_PIN|BUT_CANCEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_SEG_A_PIN|DISP_SEG_D_PIN|DISP_DIG_1_PIN|DISP_DIG_2_PIN
                          |DISP_DIG_3_PIN|DISP_DIG_4_PIN|RELAY_1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = I2C_BUS_SCL_PIN|I2C_BUS_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C_BUS_PORT, &GPIO_InitStruct);

}

/*****************************************************************************************************/
extern "C" int SysTick_Handler(void)
{
	HAL_IncTick();
  dispatcher.tasks_timeout_check(led_exception); // Проверка таймаута исполнения задач

  //osSystickHandler(); For FREERTOS stuff
  return 0;
}
/*****************************************************************************************************/

/*****************************************************************************************************/
void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}

/*****************************************************************************************************/
#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */
/*****************************************************************************************************/
