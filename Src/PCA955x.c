/**
  ******************************************************************************
  * @file    pca9554.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28 december-2014
  * @brief   This file includes the driver for control PCA9554 .
  ******************************************************************************
  * @attention
  *
   *
  ******************************************************************************
������������� ������ 512 = 64� ����
  */ 

/* Includes ------------------------------------------------------------------*/
#include "pca955x.h"
#include "main.h"
/** @addtogroup Component
  * @{
  */ 
  
HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;

uint8_t MemBlock[256]; // ���� ��� ���������� ����� ������

/**
  * @}
  */ 
// "�������" ��� ������� ������ �� I2C ��� �� �� ������ ���� �����
HAL_StatusTypeDef TOP_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, DevAddress, Trials, Timeout);
  return status;
}

HAL_StatusTypeDef TOP_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
  return status;
}

HAL_StatusTypeDef TOP_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
  return status;
}


uint32_t GetExpand (void) // ��������� ��������� ����� Expanders (9554 b 9555), ����� ������ 23-16:9554,15-0:9555
{
  uint32_t Result = 0;
//  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
  uint8_t BufTX[4];
  uint8_t BufRX[4];
  //GPIOE->BSRRH = GPIO_PIN_3; // (PE3) reset pin

  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(KEYBOARD<<1) , 1, 300); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2<<26;
    // read current stats
  BufTX[0] = 1;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(KEYBOARD<<1), BufTX, 1, 300);
  StatusI2C2 = TOP_I2C_Master_Receive(&hi2c1, (uint16_t)(KEYBOARD<<1), BufRX, 2, 300);
// ����� ���� � �����
  Result = (uint32_t)(BufRX[0]); // �������
  Result = Result<<8;
  Result += (uint32_t)(BufRX[1]);// �������
  // ��� ��� �������� ������ ��� ���������� 7 ��� (��������� ����� �����)
  
  return (Result & 0x7F);

}

    
//������������ PCA9555 ( ����������� - ����������) ������ 0x20
// ������������� ��� ����� P0-P7, ��� ������ P8-P15! �������� �� �� ��� ����� ������ ��� ������ ���� ��� �����
HAL_StatusTypeDef ConfigKBRD (void)
{
    //uint16_t DevAddr = EXPWRCTRL<<1; // address I2C
  uint8_t BufTX[10];
  //uint8_t BufRX[10];
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(KEYBOARD<<1), 1, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  // ����� � OUT ����� 0 - 
  BufTX[0] = 2;
  BufTX[1] = 0xff;
  BufTX[2] = 0x80;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(KEYBOARD<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
  // ��� �������� - 
  BufTX[0] = 4;
  BufTX[1] = 0x0;
  BufTX[2] = 0x0;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(KEYBOARD<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
  // ������������� ���������� ��� ����� ����� �0-�6 - ����, P7 & P8-P15 - �����
  BufTX[0] = 6;
  BufTX[1] = 0x7f; 
  BufTX[2] = 0x00;
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(KEYBOARD<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
//  BufTX[0] = 6;
//  BufTX[1] = 0x0;
//  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c2, (uint16_t)(COMMCTRL<<1), BufTX, 2, 10000);
   return StatusI2C2;

}



HAL_StatusTypeDef CtrlExpand ( uint16_t PinOut, uint16_t Mask, uint8_t ModeS) // ���������� ������ ������� Expanders ( 9555), only P8-P15 & P7 = P8-0x01, P9-0x02, ... P15-0x80 P7-0x100
{
  //HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
//  StatusI2C2 = HAL_ERROR;
  uint8_t BufTX[3];
  uint8_t BufRX[3];
  volatile uint16_t InPin1 = (PinOut & 0x100)>>1; // ������������� ��� ��� �������� ����� ����������
  volatile uint16_t InPin2 = PinOut & Mask; //   
  
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(KEYBOARD<<1), 1, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
    // ������� ������� ���� ����������������� � ������������ � PinOUT
    // read current stats LOEW byte
  BufTX[0] = 0;   // ����� �������  0 - ������ ��������� ������� ������ 
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(KEYBOARD<<1), BufTX, 1, 1000);
  // ������ 2 ����� ( ����� ��� ) ������� ������
  StatusI2C2 = TOP_I2C_Master_Receive(&hi2c1, (uint16_t)(KEYBOARD<<1), BufRX, 2, 1000); // ��������� ��� ����� �������� ������������� ������
  if (ModeS) // ������������� 1 �� ������ ()������������� ������ ������� ����
  {
  BufRX[0] |= (uint8_t)InPin1;
  BufRX[1] &= ~(uint8_t)Mask;
  BufRX[1] |= (uint8_t)InPin2;
  }
  else // �������� ����. ���
  {
  BufRX[0] &= ~(uint8_t)InPin1;
  BufRX[1] &= ~(uint8_t)InPin2;
  }
  // ������ ���� ��������
  BufTX[0] = 2 ; // ������ �������
  BufTX[1] = BufRX[0]; 
  BufTX[2] = BufRX[1]; 
  // write NEW stat in IC LOW Byte
   StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(KEYBOARD<<1), BufTX, 3, 1000);
  if (StatusI2C2) return StatusI2C2;
  
  return StatusI2C2 ;
  }


// ������ � ������ ��������������� ���������
void WriteNeedStruct (int NumStruct)
  {
    if(NumStruct)
    {
      if (NumStruct & 0x01) EEPROM_write(&UserMeasConfig, ADR_UserMeasConfig, sizeof(UserMeasConfig)); // ������������ ��������� ��������� ������������ ��� 0 (0x01)
      NumStruct = NumStruct>>1;
      if (NumStruct & 0x01) EEPROM_write(&CoeffPM, ADR_CoeffPM, sizeof(CoeffPM)); // ���������� ������������� ��������� ���������� P1 ��� 1  (0x02)
      NumStruct = NumStruct>>1;
      if (NumStruct & 0x01)   EEPROM_write(&SpecCoeffPM, ADR_SpecCoeffPM, sizeof(SpecCoeffPM)); // ���������� ������� ����������� ������������� ��� 2(0x04)
      NumStruct = NumStruct>>1;
      if (NumStruct & 0x01)   EEPROM_write(&DeviceConfig, ADR_ConfigDevice, sizeof(DeviceConfig)); // ����������� ������������  ������� ��� 3(0x08)
      NumStruct = NumStruct>>1;
      if (NumStruct & 0x01) EEPROM_write(&SettingPrm, ADR_SettingPrm, sizeof(SettingPrm)); // ���������� ������ �2, ���������� �����, ����� ������� � ��������� 4(0x10)
      NumStruct = NumStruct>>1;
 
    }
  }



//

uint32_t BeginConfig (void) // ��������� ������������
{
  uint32_t CodeError = 0; //
    uint32_t ErrDev=0;
  
  // �������� ������� ���������� ���������� ������� 95555 �� ���� I2C(2)
  //������������ PCA9555 ( ����������� ) ����� 0x20
  // ������������� ��� ����� ���! �������� �� �� ��� ����� ������ ��� ������ ���� ��� �����
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(KEYBOARD<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) CodeError |= NOT_PCA9555_KB; // ���������� �� ������������
  StatusI2C2 =  ConfigKBRD (); // ������������� PCA9555  
  if (StatusI2C2) CodeError |= CFG_ERR_PCA9555_KB; // ���������� �� ���������������
  //
  
//  // �������� ������� ����������  24lc512 �� ���� I2C(2) EEPROM
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) CodeError |= NOT_EEPROM_24LC128; // ���������� �� ������������
  else // ���������� ���� ��������� ������������ ������ � �������� ���� ����
  {

    // ������� �������� ������������ ������� (��������)
    EEPROM_read(&DeviceConfig, ADR_ConfigDevice, sizeof(DeviceConfig));
    ErrDev = InvalidDevice();
    if (ErrDev)
    {
      InitDevice(ErrDev); 
    EEPROM_write(&DeviceConfig, ADR_ConfigDevice, sizeof(DeviceConfig));
    }
    // �������� ������������ �������
    EEPROM_read(&CoeffPM, ADR_CoeffPM, sizeof(CoeffPM));
    ErrDev = FindErrCoeff ();
     if (ErrDev)
    {
      FixErrCoeff (ErrDev); //  ���������  (���������� ������������)
      EEPROM_write(&CoeffPM, ADR_CoeffPM, sizeof(CoeffPM));
    }
    // ��������  ����������� ������������ ������� 
    //        EEPROM_write(&SpecCoeffPM, ADR_SpecCoeffPM, sizeof(SpecCoeffPM));
    EEPROM_read(&SpecCoeffPM, ADR_SpecCoeffPM, sizeof(SpecCoeffPM));
    ErrDev = FindErrSpec ();
     if (ErrDev)
    {
      FixErrSpec (ErrDev); //  ���������  (���������� ������������)
      EEPROM_write(&SpecCoeffPM, ADR_SpecCoeffPM, sizeof(SpecCoeffPM));
    }

    // �������� ��������� ������������ ��������� �������
    EEPROM_read(&UserMeasConfig, ADR_UserMeasConfig, sizeof(UserMeasConfig));
    ErrDev = FindErrUserMC ();
    if (ErrDev)
    {
      FixErrUserMC (ErrDev); //  ���������  (���������� ���������)
      EEPROM_write(&UserMeasConfig, ADR_UserMeasConfig, sizeof(UserMeasConfig));
    }
    // ������� �������� ����������� ��������� P2 � ���������� �����
    EEPROM_read(&SettingPrm, ADR_SettingPrm, sizeof(SettingPrm));
    ErrDev = FindErrSettingPrm ();
    if (ErrDev)
    {
      FixErrSettingPrm (ErrDev); //  ���������  (���������� ���������)
      EEPROM_write(&SettingPrm, ADR_SettingPrm, sizeof(SettingPrm));
    }
//    // ������� �������� ����������� ��������� �������� ����-����
//    EEPROM_read(&g_FileParamInfo, ADR_FileInfoParam, sizeof(g_FileParamInfo));
//    ErrDev = FindErrPrmFileInfo ();
//    if (ErrDev)
//    {
//      FixErrPrmFileInfo (ErrDev); //  ���������  (���������� ���������)
//      EEPROM_write(&g_FileParamInfo, ADR_FileInfoParam, sizeof(g_FileParamInfo));
//    }
//    
//        // �������� ��������� ������ �������
//    EEPROM_read(&g_eventSearch, ADR_SearchEventsParam, sizeof(g_eventSearch));
//    ErrDev = FindErrEventsParam ();
//    if (ErrDev)
//    {
//      FixErrEventsParam (ErrDev); //  ���������  (���������� ���������)
//      EEPROM_write(&g_eventSearch, ADR_SearchEventsParam, sizeof(g_eventSearch));
//    }
//    
//    g_eventSearch.iThreshoulds = THRESHOULDS_BY_USER;
//    
//    //�������� ��������� ���������� �������
//    EEPROM_read(&g_customFiberParam, ADR_FiberParam, sizeof(g_customFiberParam));
//    ErrDev = FindErrFiberParam ();
//    if (ErrDev)
//    {
//      FixErrFiberParam (ErrDev); //  ���������  (���������� ���������)
//      EEPROM_write(&g_customFiberParam, ADR_FiberParam, sizeof(g_customFiberParam));
//    }
//    
//    //�������� ��������� ������� �������� ������������ ����� ����
//    EEPROM_read(&g_SetLW_REF, ADR_LW_RefPM, sizeof(g_SetLW_REF));
//    ErrDev = FindErrSetRefCalibr ();
//    if (ErrDev)
//    {
//      FixErrSetRefCalibr (ErrDev); //  ���������  (���������� ���������)
//      EEPROM_write(&g_SetLW_REF, ADR_LW_RefPM, sizeof(g_SetLW_REF));
//    }
//    
//    //�������� ��������� �������� ��������� �������� ���������� ����������
//    EEPROM_read(&g_SetLS_InOutParam, ADR_LS_Set, sizeof(g_SetLS_InOutParam));
//    ErrDev = FindErrLSParamSet ();
//    if (ErrDev)
//    {
//      FixErrLSParamSet (ErrDev); //  ���������  (���������� ���������)
//      EEPROM_write(&g_SetLS_InOutParam, ADR_LS_Set, sizeof(g_SetLS_InOutParam));
//    }
//    
//    
//  }
//  
//  // ����������� ����������
//  CntrlExpUnit (DSBL, EN_FLTR); // ����. ������ ��������� 
//  CntrlExpUnit (DSBL, EN_RELAY); // ����. ����, ������ ��������
//  CntrlExpUnit (DSBL, EN_70V); // ��������� ��������������� ���������� ������
//  SetRange(0); // ������������� ����� �������������� ��������
  }
  return (CodeError | (ErrDev<<3)); 
}



//������� ������ EEPROM
HAL_StatusTypeDef EEPROM_write(void *buff, uint16_t address, uint32_t byte)
{ 
#define size_pg 128
  uint8_t BufTX[140];
  uint8_t *ptr = buff;
  uint16_t n_pg = byte/size_pg ; // ����� �������
  uint16_t last_byte=byte - (n_pg*size_pg); // ������� �����
  
  
  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
  StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  //����� �� ����� 64 ����  �� ���
  for (int j=0; j<n_pg;j++)
  {
    BufTX[0] = (uint8_t)((address+(j*size_pg))>>8 & 0xFF);
    BufTX[1] = (uint8_t)((address+(j*size_pg)) & 0xFF);
    for (int i=0; i<size_pg; ++i)
    {
      BufTX[2+i] = *ptr++;
    }
    StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(EEPROM24LC128<<1), BufTX, size_pg+2, 1000); // ���������� ������ ����� �� ����� 64 ����
     HAL_Delay(10);
  }
  
  BufTX[0] = (uint8_t)((address+(n_pg*size_pg))>>8 & 0xFF);
  BufTX[1] = (uint8_t)((address+(n_pg*size_pg)) & 0xFF);
  for (int i=0; i<last_byte; ++i)
  {
    BufTX[2+i] = *ptr++;
  }
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(EEPROM24LC128<<1), BufTX, last_byte+2, 10000); // ���������� ������ ����� �� ����� 64 ����
  HAL_Delay(10);

  
  return StatusI2C2;
}

//������� ������ EEPROM
HAL_StatusTypeDef EEPROM_read(void *buff, uint16_t address, uint32_t byte)
{
  uint8_t BufTX[3];
  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
  
    StatusI2C2 =  TOP_I2C_IsDeviceReady(&hi2c1, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
  if (StatusI2C2) return StatusI2C2;
  // ������ ������ ����� ���� - 
  // ������� ���������
  // ������������� �����
  BufTX[0] = (uint8_t)(address>>8 & 0xFF);
  BufTX[1] = (uint8_t)(address & 0xFF);
  StatusI2C2 = TOP_I2C_Master_Transmit(&hi2c1, (uint16_t)(EEPROM24LC128<<1), BufTX, 2, 1000);
  TOP_I2C_Master_Receive(&hi2c1, (uint16_t)(EEPROM24LC128<<1), buff, byte, 1000);

  return StatusI2C2;
  }

//
//
//
//
//// �������� ������������ ��������� �� I2C
//uint32_t ChkI2CDevConected (void)
//{
//  uint32_t Result = 0;
//  HAL_StatusTypeDef StatusI2C2 = HAL_ERROR;
//
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)(EXPWRCTRL<<1) , 1, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! �������� ��������
//  {
//    Result++;
//  }
//    Result = Result<<1;
//   StatusI2C2 = HAL_ERROR;
//  StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)(KEYBOARD<<1), 1, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! ����������
//  {
//    Result++;
//  }
//    Result = Result<<1;
//   StatusI2C2 = HAL_ERROR;
//      // �������� ������� ����������  24sr16 �� ���� I2C(2) EEPROM
//  StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)(EEPROM24SR16<<1), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! ����������
//  {
//    Result++;
//  }
//    Result = Result<<1;
//      // �������� ������� ����������  24LC128 �� ���� I2C(2) EEPROM
//  StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)(EEPROM24LC128<<1), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! ����������
//  {
//    Result++;
//  }
//    Result = Result<<1;
//    // �������� ������� ���������� ���������� ������� 95555 �� ���� I2C(2)  ����� 0x21
//   StatusI2C2 = HAL_ERROR;
//  StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)(COMMCTRL<<1), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! ����������
//  {
//    Result++;
//  }
//    Result = Result<<1;
//    // �������� ������� ���������� ���������� ������� AD5242 �� ���� I2C(2)�����. 70� � ��������
//   StatusI2C2 = HAL_ERROR;
//  StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)(RVOTDR<<1), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! ����������
//  {
//    Result++;
//  }
//    Result = Result<<1;
//  if (Result && 0x4) // ���� �����������
//  {
//    for (int i =1;i<6;++i)
//    {
//   StatusI2C2 = HAL_ERROR;
//       CtrlExpand (COMMCTRL, DSBL_ALLRV, DSBL); // ��������� ������ ���������)
//  switch (i)
//  {
//  case 1:
//     CtrlExpand (COMMCTRL, (uint16_t)EN_RVCH1, ENBL); // �������� ����� �������a RV1
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)( RVBASEA << 1 ), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! 
//  {
//    Result++;
//  }
//    Result = Result<<1;
//  break;  
//  case 2:
//  CtrlExpand (COMMCTRL, (uint16_t)EN_RVCH2, ENBL); // �������� ����� �������a RV2
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)( RVBASEA << 1 ), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! 
//  {
//    Result++;
//  }
//    Result = Result<<1;
//  break;  
//  case 3:
//  CtrlExpand (COMMCTRL, (uint16_t)EN_RVCH3, ENBL); // �������� ����� �������a RV3
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)( RVBASEA << 1 ), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! 
//  {
//    Result++;
//  }
//    Result = Result<<1;
//  break;  
//  case 4:
//  CtrlExpand (COMMCTRL, (uint16_t)EN_RVCH4, ENBL); // �������� ����� �������a RV4
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)( RVBASEA << 1 ), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! 
//  {
//    Result++;
//  }
//    Result = Result<<1;
//  break;  
//  case 5:
//  CtrlExpand (COMMCTRL, (uint16_t)EN_RVCH5, 0); // �������� ����� �������a RV5(�� �� ��������� ����������� �����)���������!
//    StatusI2C2 =  TOP_I2C_IsDeviceReady(&I2c2Handle, (uint16_t)( RVBASEA << 1 ), 2, 1000); // ���� ��� ����������???
//  if (!StatusI2C2) // ���������� ������������! 
//  {
//    Result++;
//  }
//    Result = Result<<1;
//  break;  
//  } 
//    }
//  }
//  else
//  {
//    Result =     Result<<5;
//
//  }
//    Result =     Result<<4;
//  
//  return Result;
//}
//

