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
// ������ UART_RX (reciev)
#define RX_WAIT    0  //������� �������
#define CMD_RECIV  1  //��������� ������� (� ��������)
#define CMD_END  2  //������� ������ ����� ������� ����� ������������
#define DATA_TX  3  //���� ��������
#define END_TX  4  //��������� ��������

extern uint8_t RxBufExt[BUFF_SIZE_RX]; // ������ �������� �������
extern uint8_t RxBufOpt[BUFF_SIZE_RX]; // ������ UART2 Optic

extern uint8_t RxBufGPS[BUFF_SIZE_RX]; // ������ �������� 

extern uint8_t Reciev; // ��������� ������ 
extern uint8_t RecievOpt; // ��������� ������ uart2

extern uint8_t TransmitUT; // ��������� ������� - �������
extern uint32_t CntRX;
extern uint32_t CntRXgps;
extern uint32_t CntRXOpt; // ������� ���� ����������� ������
extern uint8_t TxDMA;
extern uint8_t AnsSIMMod; // ������� ������ �� 

extern uint32_t CntWaitEnd; // ������� �������� ����� ������ ������� �� SIM800
extern uint32_t WaitIdleRX; // �������� ����� ������ ��� ������ �������� ������
extern uint8_t DataRdyUart1; // ������� ������ ������ �����, ���� ����������!
extern uint32_t CntWaitEndOpt; // ������� �������� ����� ������ ������� �� SIM800
extern uint32_t WaitIdleRXOpt; // �������� ����� ������ ��� ������ �������� ������
extern uint8_t DataRdyUart2; // ������� ������ ������ �����, ���� ����������!
/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */
void HAL_UART_RxCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxOptCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxOptic(UART_HandleTypeDef *huart);

void SendUartTX (uint8_t *Str_mas);
void ParseCMD (void); // ������ ������ �������� �� uart1 
void ParseOptic (void); // ������ ������ �������� �� uart2 (OPTIC) 

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
