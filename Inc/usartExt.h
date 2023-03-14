/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usartExt_H
#define __usartExt_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "string.h"

/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */
#define BUFF_SIZE_RX 512
// режимы UART_RX (reciev)
#define RX_WAIT    0  //ожидаем команду
#define CMD_RECIV  1  //принимаем команду (в процессе)
#define CMD_END  2  //приняли символ конца команды можно обрабатывать
#define DATA_TX  3  //надо передать
#define END_TX  4  //закончили передачу

extern uint8_t RxBufExt[BUFF_SIZE_RX]; // буффер принятой команды
extern uint8_t RxBufOpt[BUFF_SIZE_RX]; // буффер UART2 Optic

extern uint8_t RxBufGPS[BUFF_SIZE_RX]; // буффер принятой 

extern uint8_t Reciev; // Состояние приема 
extern uint8_t RecievOpt; // Состояние приема uart2

extern uint8_t TransmitUT; // Состояние предачи - ожидаем
extern uint32_t CntRX;
extern uint32_t CntRXgps;
extern uint32_t CntRXOpt; // счетчик байт оптического приема
extern uint8_t TxDMA;
extern uint8_t AnsSIMMod; // признак ответа от 

extern uint32_t CntWaitEnd; // счетчик ожидания конца приема ответов от SIM800
extern uint32_t WaitIdleRX; // ожидание конца приема для взвода признака приема
extern uint8_t DataRdyUart1; // признак приема пакета извне, надо обработать!
extern uint32_t CntWaitEndOpt; // счетчик ожидания конца приема ответов от SIM800
extern uint32_t WaitIdleRXOpt; // ожидание конца приема для взвода признака приема
extern uint8_t DataRdyUart2; // признак приема пакета извне, надо обработать!
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
void HAL_UART_RxCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxOptCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxOptic(UART_HandleTypeDef *huart);

void SendUartTX (uint8_t *Str_mas);
void ParseCMD (void); // разбор данных принятых по uart1 
void ParseOptic (void); // разбор данных принятых по uart2 (OPTIC) 

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usartExt_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
