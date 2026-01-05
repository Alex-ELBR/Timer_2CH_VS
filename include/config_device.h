#ifndef CONFIG_DEVICE_H_INCLUDED
#define CONFIG_DEVICE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Общие операции
const uint16_t CANCEL = 0;
const uint16_t PLUS = 0xFFFF;
const uint16_t MINUS = 0xFFFE;

const uint32_t SPEED_BLINK = 300;   // частота мигания символа


/****************************************************************************************************
  Конфигурация каналов и таймеров при данной емкости EEPROM
  и адресах настроек:
*****************************************************************************************************/
const uint8_t CHANNEL_AMOUNT =     2;     // колличество каналов максимум - 9 каналов
const uint8_t TIMER_AMOUNT   =     99;    // колличество таймеров на канал - максимум 99 таймеров
/****************************************************************************************************/



/** Порт и пины подключения индикатора ***************************************************************/

#define DISP_SEG_A_PORT      GPIOB
#define DISP_SEG_B_PORT      GPIOA
#define DISP_SEG_C_PORT      GPIOA
#define DISP_SEG_D_PORT      GPIOB
#define DISP_SEG_E_PORT      GPIOA
#define DISP_SEG_F_PORT      GPIOA
#define DISP_SEG_G_PORT      GPIOA
#define DISP_SEG_DP_PORT     GPIOA

#define DISP_SEG_A_PIN       GPIO_PIN_10
#define DISP_SEG_B_PIN       GPIO_PIN_12
#define DISP_SEG_C_PIN       GPIO_PIN_9
#define DISP_SEG_D_PIN       GPIO_PIN_11
#define DISP_SEG_E_PIN       GPIO_PIN_11
#define DISP_SEG_F_PIN       GPIO_PIN_3
#define DISP_SEG_G_PIN       GPIO_PIN_8
#define DISP_SEG_DP_PIN      GPIO_PIN_10

#define DISP_DIG_4_PORT      GPIOB
#define DISP_DIG_3_PORT      GPIOB
#define DISP_DIG_2_PORT      GPIOB
#define DISP_DIG_1_PORT      GPIOB

#define DISP_DIG_4_PIN       GPIO_PIN_12
#define DISP_DIG_3_PIN       GPIO_PIN_13
#define DISP_DIG_2_PIN       GPIO_PIN_14
#define DISP_DIG_1_PIN       GPIO_PIN_15
/*****************************************************************************************************/


/** Порт и пины подключения реле канала 1 ************************************************************/
#define  RELAY_1_PORT       GPIOB
#define  RELAY_1_PIN        GPIO_PIN_9
/*****************************************************************************************************/

/** Порт и пины подключения реле канала 2 ************************************************************/
#define  RELAY_2_PORT       GPIOA
#define  RELAY_2_PIN        GPIO_PIN_15
/*****************************************************************************************************/

/** Порт и пины светодиодов **************************************************************************/
#define  LED_1_PORT         GPIOA
#define  LED_2_PORT         GPIOA
#define  LED_3_PORT         GPIOA

#define  LED_1_PIN          GPIO_PIN_4
#define  LED_2_PIN          GPIO_PIN_5
#define  LED_3_PIN          GPIO_PIN_6
/*****************************************************************************************************/

/** Порт и пины подключения кнопок управления ********************************************************/
#define BUT_OK_PORT         GPIOA
#define BUT_UP_PORT         GPIOB
#define BUT_DOWN_PORT       GPIOB
#define BUT_CANCEL_PORT     GPIOB
#define BUT_OK_PIN          GPIO_PIN_7
#define BUT_UP_PIN          GPIO_PIN_0
#define BUT_DOWN_PIN        GPIO_PIN_1
#define BUT_CANCEL_PIN      GPIO_PIN_2

const uint32_t THRESHOLD    = 50;      //колличество циклов антидребезга кнопки для фиксации
const uint32_t HOLD         = 1000;    //колличество циклов удержания кнопки для автоперебора значений
const uint32_t BUST         = 50;      //колличество циклов для скорости автоперебора значений
/*****************************************************************************************************/

#define I2C_BUS             I2C1
#define I2C_BUS_PORT        GPIOB
#define I2C_BUS_SCL_PIN     GPIO_PIN_6
#define I2C_BUS_SDA_PIN     GPIO_PIN_7

const uint16_t ADDRESS_RTC = (uint16_t)0x68;     //0xD0
const uint16_t ADDRESS_EEPROM = (uint16_t)0x50; //0xA0; //;
/*****************************************************************************************************/

















#endif /* CONFIG_DEVICE_H_INCLUDED */
