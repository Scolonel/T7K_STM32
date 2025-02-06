/**
  ******************************************************************************
  * File Name          : USARText.c
  * Description        : ����� � ��������� ������ �� UART1 � ������ �����
                        ����� ��������������� USB-UART
    UART2 (����������)
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usartExt.h"
#include "main.h"

/* USER CODE BEGIN 0 */
// ������������� �������� ��� ��������� UART � ����������� �� ���������� 
// �������� ������ � ������ ���������� 
  // for F103 
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG) || \
    defined (STM32F205xx)
#define REG_DATA huart->Instance->DR
#define REG_ISR huart->Instance->SR

#endif

#if defined(STM32F302xE) || defined(STM32F303xE) || defined(STM32F398xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || \
    defined (STM32L4P5xx) || defined (STM32L4Q5xx) || \
    defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
      /* L476 */

#define REG_DATA huart->Instance->RDR
#define REG_ISR huart->Instance->ISR

#endif


uint32_t CntRX=0;
uint32_t WaitIdleRX=0; // �������� ����� ������ ��� ������ �������� ������
uint32_t CntWaitEnd; // ������� �������� ����� ������ ������ �� �������� ����!
uint8_t DataRdyUart1 = 0; // ������� ������ ������ �����, ���� ����������!
uint8_t RxBufExt[BUFF_SIZE_RX]; // ������ ��� ������� �����
uint8_t RxBufOpt[BUFF_SIZE_RX]; // ������ ��������� ������ ������� �����
uint8_t TxDMA = 0; // ������� ��������� DMA uart1
static volatile uint32_t err;
static volatile uint16_t AdrReadMem; 
uint32_t BinBlock=0; // ������� ��������� ����� ��� �������� (�� 0 => ������ �����)
// for UART2 (Optic)
uint8_t RecievOpt = RX_WAIT; // ��������� ������
uint32_t CntRXOpt=0;
static int CountWaitCmdRelaseOpt=0; // ����� ������� �������� ���������� �������
uint32_t WaitIdleRXOpt=0; // �������� ����� ������ ��� ������ �������� ������
uint32_t CntWaitEndOpt; // ������� �������� ����� ������ ������ �� ������!
uint8_t DataRdyUart2 = 0; // ������� ������ ������ �� ������, ���� ����������!
float PowSC_Ext; // �������� ��������� ��������� �����.
float BaseExtLvl[7]; // ������� ������  ��������� ��������� �����.

enum CodeCmd OpticCMD=NOTcmd; //������� �������� �� ������ (� ������ �������)
int iParamOpt; // ������������� �������� ��� ������ �� ������
float fParamOpt; // float �������� ��� ������ �� ������

// �������� UART 1
//void HAL_UART_RxCallbackU2(UART_HandleTypeDef *huart) // reciev from UART2
//{
//  uint8_t temp;
//  static int CountWaitCmdCheck=0;
//  if ((huart->Instance->SR & UART_FLAG_ORE) != 0)
//  {
//    
//  }
//  if ((huart->Instance->SR & UART_FLAG_RXNE) != 0)
//  {
//    temp = (uint8_t)(huart->Instance->DR & (uint16_t)0x00FF);
//    switch (Reciev2)
//    {
//    case RX_WAIT:                    //������� ����� ����� �������
//      if (temp==';')
//      {
//        CntRXu2=0;
//        //RxBufGPS[CntRXgps]=temp;
//        Reciev2=CMD_RECIV;
//        CntRXu2++;
//      }
//      break;
//    case CMD_RECIV:                   //��������� ������� ������� ASCII �������
//      {
//        switch (temp)
//        {
//        case ';':                 //����� ������� ������� ������ �������...  �������� �������
//          CntRXu2=0;
//          //RxBufGPS[CntRXgps]=temp;
//          CntRXu2++;
//          break;
//        //case 0xD:                 //����� ������� (��� ���� ���)
//        case 0xD:                 //���� ����� ������� �� �� �� ��������
//          RxBufUART2[CntRXu2]=temp;
//          Reciev2=CMD_END;            //������� ������� ��������� ���� ������������
//          break;
//        default:                  //��������� �����
//          RxBufUART2[CntRXu2]=temp;
//          CntRXu2++;
//          if (CntRXu2 > 31) // anee noi?ea aieuoa i?eaiiiai aooa?a
//          {
//            CntRXu2=0;
//            Reciev2=RX_WAIT;
//          }
//          break;
//        }
//      }
//      break;
//    case CMD_END:                     // 
//      if (CountWaitCmdCheck++>100) // �� �� ������� ����� �� �������������� (���� �� �� ��������)
//      {
//            CntRXu2=0;
//            Reciev2=RX_WAIT;
//            CountWaitCmdCheck = 0;
//        
//      }
//
//      break;                      //I?aauaouay eiiaiaa ia io?aaioaia - iiyoiio ie?aai ia aaeaou
//    }
//  }
//  //}
//  // HAL_UART_Receive_IT(&huart3, U3Buf,1);
//    
//}



void HAL_UART_RxCallback(UART_HandleTypeDef *huart)
{
  uint8_t temp;
  //  if ((huart->Instance->SR && UART_FLAG_ORE) != 0)
  //  {
  //    
  //  }
  //  if ((huart->Instance->SR && UART_FLAG_TC) != 0)
  //  {
  //    
  //  }
  //  if ((huart->Instance->SR && UART_FLAG_TXE) != 0)
  //  {
  //    
  //  }
  if ((REG_ISR & UART_FLAG_RXNE) != 0)
  {
    temp = (uint8_t)(REG_DATA & (uint16_t)0x00FF);
    // ������ ����������� ������
    RxBufExt[CntRX]=temp;
    CntRX++;
    if (CntRX > BUFF_SIZE_RX) // anee noi?ea aieuoa i?eaiiiai aooa?a
    {
      CntRX=0;
    }
    WaitIdleRX = 1; // ������� ����, ������� ������� ������, ��� �������� ����� ������
    CntWaitEnd=0;// �� ������� �������
  }
}
// Optic RECIEV
// ���� ������������ ����� �� ������, ��� �� ������� �������, � ��������� ����� ���� �� �����������
// ����� ��������� ������ RecievOpt
void HAL_UART_RxOptic(UART_HandleTypeDef *huart)
{
  uint8_t temp;
  if ((REG_ISR & UART_FLAG_RXNE) != 0)
  {
    temp = (uint8_t)(REG_DATA & (uint16_t)0x00FF);
    // ������ ����������� ������
    RxBufOpt[CntRXOpt]=temp;
    CntRXOpt++;
    if (CntRXOpt > BUFF_SIZE_RX) // anee noi?ea aieuoa i?eaiiiai aooa?a
    {
      CntRXOpt=0;
    }
    WaitIdleRXOpt = 1; // ������� ����, ������� ������� ������, ��� �������� ����� ������
    CntWaitEndOpt = 0;// �� ������� �������
  }
}

void SendUartTX (uint8_t *Str_mas) // ������� �������� �� DMA ������! (�� �������� ������)
{
    //while(TxDMA);
  HAL_UART_Transmit_DMA(&huart1, (void*)Str_mas,strlen((void*)Str_mas)); // ������ 
  TxDMA = 1;
  CntRX = 0; // ����� �������� ������, ��� �������� �������� �������)
    //CDC_Transmit_FS ((void*)Str_mas,strlen((void*)Str_mas));
}

void ParseCMD (void) // ������ ������ �������� �� uart1 
{
  static uint8_t AnsData[520]; // ������ ��� �������
  uint8_t  NeedSave;
  //  uint8_t  NeedSaveUser = 0;
  //  uint8_t  NeedSavePM = 0;
  static int params[12];
  static float fparams[17];
  uint8_t Str[32];
  int i,j,k; // ������� ������
  uint8_t EnaSendOK=0;// ������� ���������� ������ ��
  uint8_t EnaSendAns=0;// ������� ���������� ������ ��� ������� �������
  // ������ ����� ����� ���������� 
  if(DataRdyUart1 && CntRX)
  {
    NeedSave = 0;
    BinBlock = 0; // ������ �����
    
    // Set ERROR ansver if BAD commands
    sprintf ((void*)AnsData, "ERR\r");
    for (int i=0; ((i<CntRX)&&(RxBufExt[i]!=0x20)); i++ )
    {
      if ((RxBufExt[i] >=0x61) && (RxBufExt[i]<=0x7A))RxBufExt[i] = RxBufExt[i] - 0x20;
    }
    
    //memset(AnsData,0, ARRAY_SIZE(AnsData)); // ������ ������ ��������!
    if (RxBufExt[CntRX-1]==0xd) // ASCII commands enable (check \r in end commands)
    {
      // ������� �� *
      if (!memcmp ((void*)RxBufExt, "*",1)) // commands begin '*'
      {
        // ����� �������, ���������� () 
        if (!memcmp ((void*)&RxBufExt[1], "RST",3))
        {  
          myBeep(1010);
          NumMyBeep = 16;
          //Off Device
          HAL_GPIO_WritePin(POWER_ON_GPIO_Port, POWER_ON_Pin, GPIO_PIN_RESET);
          
        }
        // ������ �������������� 
        if (!memcmp ((void*)&RxBufExt[1], "IDN?",4))
        {
          sprintf ((void*)AnsData, "T7K,%s,(%d/%c:%d/%c:%d/%c)%s%s%s,%04d,SW_rev 1.%04d\r", // ������� VFL �� �������������� -VFL (� ����� �������)
                   DeviceConfig.NameDevice,
                   (DeviceConfig.PlaceLS[0]%1000)/10,
                   (DeviceConfig.CfgOUT[0])?('1'):('0'), // 
                   (DeviceConfig.PlaceLS[1]%1000)/10,
                   (DeviceConfig.CfgOUT[1])?('1'):('0'), // 
                   (DeviceConfig.PlaceLS[2]%1000)/10,
                   (DeviceConfig.CfgOUT[2])?('1'):('0'), // 
                   (DeviceConfig.CfgPM)?((DeviceConfig.CfgPM-1)?("-PMH"):("-PM")):(""),
                   (DeviceConfig.CfgL)?("-L"):(""), // 
                   (DeviceConfig.CfgRE & 0x1)?("-VFL"):(""),
                   DeviceConfig.NumDevice,
                   NumVer);//(g_ConfigDevice.ID_Device)?("EN"):("RU")
          
          //    HAL_UART_Transmit_DMA(&huart3, (void*)ID_Dev,strlen((void*)ID_Dev)); // ������ �������������
        }
        // ���������� ��������� � ��������� ���������� �������
        if (!memcmp ((void*)&RxBufExt[1], "SETUPEN",7))
        {
          g_EnaSetup = 1; // set Enable setting config & params
          EnaSendOK = 1;
        }
        // ���������� ��������� � ��������� ���������� �������
        if (!memcmp ((void*)&RxBufExt[1], "SETUPDIS",8))
        {
          g_EnaSetup = 0; // set Enable setting config & params
          EnaSendOK = 1;
        }
        if (!memcmp ((void*)&RxBufExt[1], "MODE",4))
        {
          if(!memcmp ((void*)&RxBufExt[5], "?",1)) // answer current MODE
          {
            sprintf((void*)AnsData,"%d\r",GetModePM());
          }
          if(!memcmp ((void*)&RxBufExt[5], " ",1)) // Set MODE
          {
            params[0] = atoi((char*)&RxBufExt[6]); // get param for setting
            if((params[0]==0)||(params[0]==1)) // valid 
            {
              SetModePM((uint8_t)params[0]);
              EnaSendOK = 1;
            }
          }
        }
        if (!memcmp ((void*)&RxBufExt[1], "CLOCK:",6))
        {
          //RTC_TimeTypeDef sTime = {0};
          //RTC_DateTypeDef sDate = {0};
          
          if (!memcmp ((void*)&RxBufExt[7], "DATE ",5))
          {
            int i=12;
            uint32_t Num;
            
            ReSetRegRTC(0, &hrtc); // for F303
            //Sec2Clock(GetSysTime(&hrtc));// ��������� ��������� ������� F103
            
            if (!memcmp ((void*)&RxBufExt[12], "?",1))
            {
              ReSetRegRTC(0, &hrtc); // for F303
              //Sec2Clock(GetSysTime(&hrtc)); // �������� ������� ���� F103
            }
            else
            {
              Num = atoi((char*)&RxBufExt[i]); // ����
              if ((Num>31)||(Num<1)) Num = 1;
              Clocks.cDay = Num;
              i++;
              if (RxBufExt[i] != ',') i++;
              i++;
              Num = atoi((char*)&RxBufExt[i]); // �����
              if ((Num>12)||(Num<1)) Num = 1;
              Clocks.cMonth = Num;
              i++;
              if (RxBufExt[i] != ',') i++;
              i++;
              Num = atoi((char*)&RxBufExt[i]); // ���
              if ((Num>2043)||(Num<2021)) Num = 2021;
              Clocks.cYear = Num-2000;
              
              //RTC_WriteTimeCounter(&hrtc,TotalSec()); // ������� ����� ���� for F103
              // for F303
              ReSetRegRTC(1,&hrtc);// ������� ����� ���� F303
              
            }
            sprintf ((char*)AnsData, "%02d/%02d/%04d\r",Clocks.cDay,Clocks.cMonth,Clocks.cYear+2000);
          }
          if (!memcmp ((void*)&RxBufExt[7], "TIME ",5))
          {
            int i=12;
            uint32_t Num;
            if (!memcmp ((void*)&RxBufExt[12], "?",1)) //
            {
              ReSetRegRTC(0, &hrtc); // for F303
              //Sec2Clock(GetSysTime(&hrtc)); // �������� ������� ���� F103
            }
            else
            {
              Num = atoi((char*)&RxBufExt[i]); // ����
              if (Num>23) Num = 0;
              Clocks.cHour = Num;
              i++;
              if (RxBufExt[i] != ',') i++;
              i++;
              Num = atoi((char*)&RxBufExt[i]); // ������
              if (Num>59) Num = 0;
              Clocks.cMinutes = Num;
              i++;
              if (RxBufExt[i] != ',') i++;
              i++;
              Num = atoi((char*)&RxBufExt[i]); //�������
              if (Num>59) Num = 0;
              //RTC_WriteTimeCounter(&hrtc,TotalSec()); // ������� ����� ���� F103
              ReSetRegRTC(1,&hrtc);// ������� ����� ���� F303
            }
            sprintf ((char*)AnsData, "%02d:%02d:%02d\r",Clocks.cHour,Clocks.cMinutes,Clocks.cSeconds);
          }
        }
      }// *
      if(g_EnaSetup) // ��������� ��������� 
      {
        if (!memcmp ((void*)RxBufExt, ";",1)) // commands begin ';'
        {
          if (!memcmp ((void*)&RxBufExt[1], "INI:",4)) // 
          {
            if (!memcmp ((void*)&RxBufExt[5], "NAME",4)) //
            {
              if(!memcmp ((void*)&RxBufExt[9], "?",1)) // answer NAME DEVICE
              {
                sprintf((void*)AnsData,"%s\r",DeviceConfig.NameDevice);
              }
              if(!memcmp ((void*)&RxBufExt[9], " ",1)) // set new NAME DEVICE
              {
                if(CntRX<26) // size valid
                {
                  for (int i=0; i<16; i++)
                  {
                    DeviceConfig.NameDevice[i] = 0;
                    if(((10+i)<CntRX)&&(RxBufExt[10+i]>=0x20)) DeviceConfig.NameDevice[i] = RxBufExt[10+i];
                  }
                  //sprintf((void*)AnsData,"%s\r",DeviceConfig.NameDevice);
                  EnaSendOK = 1;
                  NeedSave |=0x08;
                }
              }
              
            } // PARAM        
            if (!memcmp ((void*)&RxBufExt[5], "PARAM",5)) //
            {
              if(!memcmp ((void*)&RxBufExt[10], "?",1)) // answer PARAMS CONFIG DEVICE
              {
                sprintf ((void*)AnsData, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r", // 
                         DeviceConfig.NumDevice,
                         DeviceConfig.CfgPM,
                         DeviceConfig.CfgRE,
                         DeviceConfig.CfgL, // 
                         DeviceConfig.PlaceLS[0],                     // 
                         DeviceConfig.PlaceLS[1],                     // 
                         DeviceConfig.PlaceLS[2],                     // 
                         DeviceConfig.CfgOUT[0], // 
                         DeviceConfig.CfgOUT[1], // 
                         DeviceConfig.CfgOUT[2] // 
                           );//(g_ConfigDevice.ID_Device)?("EN"):("RU")
              }
              
              if (!memcmp ((void*)&RxBufExt[10], " ",1)) //
              {
                i = 10;
                k = 0;
                while(RxBufExt[i]!=0xd)
                {
                  j=0;
                  i++;
                  while((RxBufExt[i]!=0xd)&&(RxBufExt[i]!=','))
                  {
                    Str[j++] = RxBufExt[i++];
                  }
                  Str[j]=0;
                  params[k++] = atoi((char*)Str);
                }
                // ��������� ������
                if(k==10) // ��� ���������, ������� ������
                { 
                  err=0;
                  err = ((params[0]<0)||(params[0]>65535));  // number
                  err = (err<<1) | ((params[1]<0)||(params[1]>2));  // type PHotdiod
                  err = (err<<1) | ((params[2]<0)||(params[2]>3));  // VFL + New Plate 03/03/2023
                  err = (err<<1) | ((params[3]<0)||(params[3]>1));  // L -type
                  err = (err<<1) | ((params[4]<0)||(params[4]>1700));  // LW 1 place
                  err = (err<<1) | ((params[5]<0)||(params[5]>1700));  // LW 2 place
                  err = (err<<1) | ((params[6]<0)||(params[6]>1700));  // LW 3 place
                  err = (err<<1) | ((params[7]<0)||(params[7]>1));  // ena LS_1 when 'd'
                  err = (err<<1) | ((params[8]<0)||(params[8]>1));  // ena LS_2 when 'd'
                  err = (err<<1) | ((params[9]<0)||(params[9]>1));  // ena LS_3 when 'd'
                  if(err==0)
                  {
                    // rewrite SETTING CONFIG
                    DeviceConfig.CfgOUT[2] = params[9]; //         // 0 bit
                    DeviceConfig.CfgOUT[1] = params[8]; //  // 1 bit
                    DeviceConfig.CfgOUT[0] = params[7]; //    2 bit
                    DeviceConfig.PlaceLS[2] = params[6];                     // 3 bit
                    DeviceConfig.PlaceLS[1] = params[5];                     // 4 bit
                    DeviceConfig.PlaceLS[0] = params[4];                     // 5 bit
                    DeviceConfig.CfgL = params[3];                     // 6 bit
                    DeviceConfig.CfgRE = params[2]; //                     // 7 bit
                    DeviceConfig.CfgPM = params[1];                     // 8 bit
                    DeviceConfig.NumDevice = params[0];                     // 8 bit
                    NeedSave |= 0x08;
                    EnaSendOK = 1;
                  }
                }
              }
              
            } // PARAM
          } // INI:  
          if (!memcmp ((void*)&RxBufExt[1], "PM:",3)) // 
          { 
            if (!memcmp ((void*)&RxBufExt[4], "CNTRL:",6)) // 
            {
              if (!memcmp ((void*)&RxBufExt[10], "ADC?",4)) //// ADC? 
              {
                sprintf((void*)AnsData,"%d\r",GetPMData());
                
              }
              if (!memcmp ((void*)&RxBufExt[10], "AP2?",4)) // // ADC P2 - ������� � �������
              {
                sprintf((void*)AnsData,"%.3f\r",MediumP2);//,IN_BuffADC[1]
              }
              // 29.11.2022 ������� �������� P2 � dBm
              if (!memcmp ((void*)&RxBufExt[10], "DP2?",4)) // // ADC P2 - ������� � dBm
              {
                sprintf((void*)AnsData,"%.3f[%.f] (%.2f/%.2f/%.2f) (P=%.8f Pm=%.8f sm=%.8f(mW) Pc=%.8f)\r",P2dBmValue,MediumP2, ORLCalcFree, ORLCalc, ORLCalcCorr, PowP2_mW, MedP2mW,PowClbr_mW,MedP2mW_corr );//
              }
              if (!memcmp ((void*)&RxBufExt[10], "RNG",3)) // 
              {
                if (!memcmp ((void*)&RxBufExt[13], "?",1))
                {
                  sprintf((void*)AnsData,"%d\r",GetRange());
                  
                }
                if (!memcmp ((void*)&RxBufExt[13], " ",1))
                {
                  params[0] = atoi((char*)&RxBufExt[14]); // get param for setting
                  if((params[0]>=0)&&(params[0]<4))
                  {
                    //Range=params[0];                                          //����������� ��������
                    SetRange(params[0]);
                    EnaSendOK = 1;
                  }
                }
              } // RNG
              
            } // CNTRL:
          } // PM:
          //SET: // ������� ������/������ ����������� ������ ���������� �������� �1 � �2
          if (!memcmp ((void*)&RxBufExt[1], "SET:",3)) // 
          {
            //------------------------------------------------------------- YEAR MANUFACTURING
            if (!memcmp ((void*)&RxBufExt[5], "YEAR",4)) // ��� �������������
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%02d�\r", // 
                         DeviceConfig.YearManuf
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                DeviceConfig.YearManuf = params[0];
                EnaSendOK = 1;
                NeedSave |= 0x08;            
              }
            }
            //------------------------------------------------------------- LN 
            if (!memcmp ((void*)&RxBufExt[5], "KGEN",4)) // �����. ������������������ ���������� �����
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%f\r", // 
                         DeviceConfig.KGen
                           );//
              }
              else
              {
                fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                DeviceConfig.KGen = fparams[0];
                EnaSendOK = 1;
                NeedSave |= 0x08;            
              }
            }
            //------------------------------------------------------------- LN 
            if (!memcmp ((void*)&RxBufExt[5], "MZON",4)) // ������� ���� ���������� �����
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%.1f\r", // 
                         DeviceConfig.MZone
                           );//
              }
              else
              {
                fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                DeviceConfig.MZone = fparams[0];
                EnaSendOK = 1;
                NeedSave |= 0x08;            
              }
            }
            //------------------------------------------------------------- �2
            if (!memcmp ((void*)&RxBufExt[5], "GRAD",4)) // ORLS (�����. ������������������
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%f\r", // 
                         DeviceConfig.Kprop
                           );//
              }
              else
              {
                fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                DeviceConfig.Kprop = fparams[0];
                EnaSendOK = 1;
                NeedSave |= 0x08;            
              }
            }
            if (!memcmp ((void*)&RxBufExt[5], "SMBEG",5)) // P2 �������� ��������� (���� ��� ��� 0 ���)
            {
              if (!memcmp ((void*)&RxBufExt[10], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%d,%d,%d,%d,%d,%d,%d\r", // 
                         DeviceConfig.SmPx[0],
                         DeviceConfig.SmPx[1],
                         DeviceConfig.SmPx[2],
                         DeviceConfig.SmPx[3],
                         DeviceConfig.SmPx[4],
                         DeviceConfig.SmPx[5],
                         DeviceConfig.SmPx[6]
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                if((params[0]>=0)&&(params[0]<7))
                {
                  params[1] = atoi((char*)&RxBufExt[11]); // get param for setting
                  DeviceConfig.SmPx[params[0]]= params[1];
                  EnaSendOK = 1;
                  NeedSave |= 0x08;            
                }
              } // ? or data
            } // �������� �2 
            //---------------------------------------------------------- �1
            if (!memcmp ((void*)&RxBufExt[5], "OFSET",5)) // P1 ����
            {
              if (!memcmp ((void*)&RxBufExt[10], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%d,%d,%d,%d\r", // 
                         CoeffPM.ShZeroRng[0],
                         CoeffPM.ShZeroRng[1],
                         CoeffPM.ShZeroRng[2],
                         CoeffPM.ShZeroRng[3]
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                if((params[0]>=0)&&(params[0]<4))
                {
                  params[1] = atoi((char*)&RxBufExt[11]); // get param for setting
                  CoeffPM.ShZeroRng[params[0]]= params[1];
                  EnaSendOK = 1;
                  NeedSave |= 0x02;            
                }
              } // ? or data
            } // ofset
            if (!memcmp ((void*)&RxBufExt[5], "STYK",4)) // STYK
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%f,%f,%f,%f\r", // 
                         CoeffPM.CoefStykRange[0],
                         CoeffPM.CoefStykRange[1],
                         CoeffPM.CoefStykRange[2],
                         CoeffPM.CoefStykRange[3]
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[9]); // get param for setting
                if((params[0]>=0)&&(params[0]<4))
                {
                  fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                  if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>100.0)))// ������������ �������� ����������
                  {
                    CoeffPM.CoefStykRange[params[0]]= fparams[0];
                    EnaSendOK = 1;
                    NeedSave |= 0x02;  
                  }
                }
              } // ? or data
            } //STYK
            if (!memcmp ((void*)&RxBufExt[5], "SPTR",4)) // SPTR ( ������������ ����� (3 ������������)
            {
              switch(RxBufExt[9])                  //  if (!memcmp ((void*)&RxBufExt[9], "SPTR",4)) // SPTR ( ������������ ����� (3 ������������)
              {
              case 'L': // 5 ��.
                if (!memcmp ((void*)&RxBufExt[10], "?",1)) // 
                {
                  sprintf ((void*)AnsData, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r", // 
                           CoeffPM.CoefSpctrL[0],
                           CoeffPM.CoefSpctrL[1],
                           CoeffPM.CoefSpctrL[2],
                           CoeffPM.CoefSpctrL[3],
                           CoeffPM.CoefSpctrL[4],
                           CoeffPM.CoefSpctrL[5]
                             );//
                }
                else
                {
                  params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                  if((params[0]>=0)&&(params[0]<6))
                  {
                    fparams[0] = atof((char*)&RxBufExt[11]); // get param for setting
                    if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>8000.0)))// ������������ �������� ����������
                    {
                      CoeffPM.CoefSpctrL[params[0]]= fparams[0];
                      EnaSendOK = 1;
                      NeedSave |= 0x02; 
                    }
                  }
                }
                break;
              case 'M': // 10 ��.
                if (!memcmp ((void*)&RxBufExt[10], "?",1)) // 
                {
                  sprintf ((void*)AnsData, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r", // 
                           CoeffPM.CoefSpctrM[0],
                           CoeffPM.CoefSpctrM[1],
                           CoeffPM.CoefSpctrM[2],
                           CoeffPM.CoefSpctrM[3],
                           CoeffPM.CoefSpctrM[4],
                           CoeffPM.CoefSpctrM[5],
                           CoeffPM.CoefSpctrM[6],
                           CoeffPM.CoefSpctrM[7],
                           CoeffPM.CoefSpctrM[8],
                           CoeffPM.CoefSpctrM[9]
                             );//
                }
                else
                {
                  params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                  if((params[0]>=0)&&(params[0]<10))
                  {
                    fparams[0] = atof((char*)&RxBufExt[11]); // get param for setting
                    if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>8000.0)))// ������������ �������� ����������
                    {
                      CoeffPM.CoefSpctrM[params[0]]= fparams[0];
                      EnaSendOK = 1;
                      NeedSave |= 0x02;   
                    }
                  }
                }
                break;
              case 'H': // 13 ��.
                if (!memcmp ((void*)&RxBufExt[10], "?",1)) // 
                {
                  sprintf ((void*)AnsData, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r", // 
                           CoeffPM.CoefSpctrH[0],
                           CoeffPM.CoefSpctrH[1],
                           CoeffPM.CoefSpctrH[2],
                           CoeffPM.CoefSpctrH[3],
                           CoeffPM.CoefSpctrH[4],
                           CoeffPM.CoefSpctrH[5],
                           CoeffPM.CoefSpctrH[6],
                           CoeffPM.CoefSpctrH[7],
                           CoeffPM.CoefSpctrH[8],
                           CoeffPM.CoefSpctrH[9],
                           CoeffPM.CoefSpctrH[10],
                           CoeffPM.CoefSpctrH[11],
                           CoeffPM.CoefSpctrH[12]
                             );//
                }
                else
                {
                  params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                  if((params[0]>=0)&&(params[0]<13))
                  {
                    fparams[0] = atof((char*)&RxBufExt[12]); // get param for setting
                    if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>8000.0)))// ������������ �������� ����������
                    {
                      CoeffPM.CoefSpctrH[params[0]]= fparams[0];
                      EnaSendOK = 1;
                      NeedSave |= 0x02;  
                    }
                  }
                }
                break;
              }
              
            }
            if (!memcmp ((void*)&RxBufExt[5], "CLBR",4)) // CLBR (����������� ��� ������������ ���� ����
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\r", // 
                         CoeffPM.CoefPointKlb[0],
                         CoeffPM.CoefPointKlb[1],
                         CoeffPM.CoefPointKlb[2],
                         CoeffPM.CoefPointKlb[3],
                         CoeffPM.CoefPointKlb[4],
                         CoeffPM.CoefPointKlb[5],
                         CoeffPM.CoefPointKlb[6]
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[9]); // get param for setting
                if((params[0]>=0)&&(params[0]<7))
                {
                  fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                  if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>800.0)))// ������������ �������� ���� ���� ����������
                  {
                    CoeffPM.CoefPointKlb[params[0]]= fparams[0];
                    EnaSendOK = 1;
                    NeedSave |= 0x02; 
                  }
                }
              }
            }
            if (!memcmp ((void*)&RxBufExt[5], "SPCLB",5)) // SPCLB (����������� ������������ �����. � �������� ���� ����)
            {
              if (!memcmp ((void*)&RxBufExt[10], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n%d,%.4f\n\r", // 
                         SpecCoeffPM.PointSpecKalib[0],
                         SpecCoeffPM.CoefSpecKlb[0],
                         SpecCoeffPM.PointSpecKalib[1],
                         SpecCoeffPM.CoefSpecKlb[1],
                         SpecCoeffPM.PointSpecKalib[2],
                         SpecCoeffPM.CoefSpecKlb[2],
                         SpecCoeffPM.PointSpecKalib[3],
                         SpecCoeffPM.CoefSpecKlb[3],
                         SpecCoeffPM.PointSpecKalib[4],
                         SpecCoeffPM.CoefSpecKlb[4],
                         SpecCoeffPM.PointSpecKalib[5],
                         SpecCoeffPM.CoefSpecKlb[5],
                         SpecCoeffPM.PointSpecKalib[6],
                         SpecCoeffPM.CoefSpecKlb[6],
                         SpecCoeffPM.PointSpecKalib[7],
                         SpecCoeffPM.CoefSpecKlb[7],
                         SpecCoeffPM.PointSpecKalib[8],
                         SpecCoeffPM.CoefSpecKlb[8],
                         SpecCoeffPM.PointSpecKalib[9],
                         SpecCoeffPM.CoefSpecKlb[9]
                           );//
              }
              else
              {
                int j=12;
                params[0] = atoi((char*)&RxBufExt[10]); // get param for setting
                if((params[0]>=0)&&(params[0]<10))
                {
                  params[1] = atoi((char*)&RxBufExt[12]); // get param for setting
                  //CoeffPM.CoefPointKlb[params[0]]= fparams[0];
                  while((RxBufExt[j++]!=',') && (j<20));
                  if(j<18)
                  {
                    fparams[0] = atof((char*)&RxBufExt[j]); // get param for setting
                    if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>800.0)))// ������������ ������������ ���� ����
                    {
                      SpecCoeffPM.PointSpecKalib[params[0]]=params[1];
                      SpecCoeffPM.CoefSpecKlb[params[0]]=fparams[0];
                      EnaSendOK = 1;
                      NeedSave |= 0x04;  
                    }
                  }
                }
              }
            } //SPCLB (���� ������������
            if (!memcmp ((void*)&RxBufExt[5], "TUNE",4)) // ������������� ������������ ��� ���������� 0-(800-900);1-(1210-1405);2-(1410-1650)
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%.2f,%.2f,%.2f\r", // 
                         CoeffPM.CoefTune[0],
                         CoeffPM.CoefTune[1],
                         CoeffPM.CoefTune[2]
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[9]); // get param for setting
                if((params[0]>=0)&&(params[0]<3))
                {
                  fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                  if (!(isnan(fparams[0])||(fparams[0]<=0.1)||(fparams[0]>10.0))) // ������������ ����� �� �������� ���������� .0 �� +12.0 (�.�. ����� ��������� ���)
                  {            
                    CoeffPM.CoefTune[params[0]]= fparams[0];
                    EnaSendOK = 1;
                    NeedSave |= 0x02; 
                  }
                }
              }
            }// TUNE
            if (!memcmp ((void*)&RxBufExt[5], "ORLB",4)) // ORLB (������� ������ ��� ���������� ORL
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%.2f,%.2f,%.2f\r", // 
                         CoeffPM.BaseRet[0],
                         CoeffPM.BaseRet[1],
                         CoeffPM.BaseRet[2]//,
                           //CoeffPM.BaseRet[3],
                           //CoeffPM.BaseRet[4],
                           //CoeffPM.BaseRet[5],
                           //CoeffPM.BaseRet[6]
                           );//
              }
              else
              {
                params[0] = atoi((char*)&RxBufExt[9]); // get param for setting
                if((params[0]>=0)&&(params[0]<3))
                {
                  fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                  if (!(isnan(fparams[0])||(fparams[0]<=-9.0)||(fparams[0]>9.0)))      // ������� ������ ORL �� -9.0 �� +9.0 �������� ���������
                  {
                    CoeffPM.BaseRet[params[0]]= fparams[0];
                    // ����������� ������ ��������� �������� ORL �� ������� ���������
                    for(int i=0; i<3; i++)
                      BaseRetCurrORL[i] = CoeffPM.BaseRet[i];
                    // ����� � ��������� �� �������� ������ ���������! 28.03.2023
                    EnaSendOK = 1;
                    NeedSave |= 0x02; 
                  }
                }
              }
            }// ORLB
            if (!memcmp ((void*)&RxBufExt[5], "ORLS",4)) // ORLS (������ �������� � ������ ��������� ORL
            {
              if (!memcmp ((void*)&RxBufExt[9], "?",1)) // 
              {
                sprintf ((void*)AnsData, "%e\r", // 
                         CoeffPM.Pow_mW_Clbr
                           );//
              }
              else
              {
                fparams[0] = atof((char*)&RxBufExt[10]); // get param for setting
                if (!(isnan(fparams[0])||(fparams[0]<=0.0)||(fparams[0]>0.00001)))// �������� ORL �� 10���
                {
                  CoeffPM.Pow_mW_Clbr= fparams[0];
                  // ��������� � �������
                  PowClbr_mW = CoeffPM.Pow_mW_Clbr;
                  
                  EnaSendOK = 1;
                  NeedSave |= 0x02;   
                }
              }
            }//ORLS
            if (!memcmp ((void*)&RxBufExt[5], "SCRO",4)) // SCRO (��������� ������� ���������� ����������)
            {
              params[0] = atoi((char*)&RxBufExt[10]); // ����� ��������� ������� ��������
              if(params[0]<3) //� ������ ���������
              {
                Ind_LW = params[0]; // ������� ����� ������� �����
                // 07.12.2022 �������� ������������� � ������������ ������� � ������������ � ��������������� ������ �����
                IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
                indx_LW_ORL = Ind_LW ;// ��������� ������ ���������� ��������� ��� ��������� � P2
                SWITCH_LW(); // ���������� ������� ����������.
                
                // �������� ��� ���� �������
                switch(RxBufExt[9])
                {
                case 'F':
                  ScMode = OFFs;
                  ControlSC (ScMode); //// ���������� ���������� 
                  EnaSendOK = 1;
                  break;
                case 'N':
                  ScMode = SCW;
                  ControlSC (ScMode); //// ���������� ���������� 
                  EnaSendOK = 1;
                  break;
                }
              }
              NeedReDraw=1;
              
            }
          }// SET:
          
          // ������� ���������� ����������� ���� ���, ���������
          // ������� ���������� �������������� ���������
        } // ';'
      } // ENASetup!
      else // ������� ��� ���������
      {
        // �������� ������
        if (!memcmp ((void*)RxBufExt, ";",1)) // commands begin ';'
        {
          if (!memcmp ((void*)&RxBufExt[1], "RMT:",4)) // 
          {
            if (!memcmp ((void*)&RxBufExt[5], "SCR",3)) // ������ �������� ����������
            {
              BinBlock = 579; // ������ �����
              
            }
            if (!memcmp ((void*)&RxBufExt[5], "IND",3)) // ������ ����� ���������� ��� ���������� �������
            {
              WriteBlockMEM (1); // ������ ����� ������ � ������, �������� �������� ��� UART EXT
              memcpy(AnsData, "#265", 4 );
              memcpy( &AnsData[4], &MemoryData, 64 ); // 
              memcpy( &AnsData[68], "\r", 1 );
              BinBlock = 69; // ������ �����
              
            }
            if (!memcmp ((void*)&RxBufExt[5], "LW ",3)) // ������ ������ ��� ��� ���������
            {
              params[0] = atoi((char*)&RxBufExt[8]); // get param for setting
              switch(CondWork)
              {
              case P1:
                CurrSpectrKoeff = GetCoeffSpctr((int)(((int)((params[0]+2.5)/5))*5),0);
                sprintf((void*)AnsData,"%d\r",PMWavelenght);
                
                break;
              case P2:
                for(int i=0;i<WAVE_LENGTHS_NUM -1;i++)
                {
                  if( CalibrLW[i]==params[0])
                  {
                    IndxP2LW = i;
                    IN_BuffADC_one = (IN_BuffADC[1]>ADC_Calibr)?(IN_BuffADC[1]-ADC_Calibr):(IN_BuffADC[1]);
                    memset(BuffP2,IN_BuffADC_one, ARRAY_SIZE(BuffP2)); // ����� ����������� ������
                    //memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // ����� ����������� ������
                    sprintf((void*)AnsData,"%d\r",CalibrLW[IndxP2LW]);
                    NeedReDraw=1;
                    break; 
                  }
                }
                break;
              case Sc:
                for(int i=0;i<3;i++)
                {
                  if( DeviceConfig.PlaceLS[i]==params[0])
                  {
                    Ind_LW = i;
                    SWITCH_LW(); // ���������� ������� ����������.
                    sprintf((void*)AnsData,"%d\r",DeviceConfig.PlaceLS[i]);
                    NeedReDraw=1;
                    break; 
                  }
                }
                break;
              case Rl:
                for(int i=0;i<3;i++)
                {
                  if( DeviceConfig.PlaceLS[i]==params[0])
                  {
                    Ind_LW = i;
                    SWITCH_LW(); // ���������� ������� ����������.
                    sprintf((void*)AnsData,"%d\r",DeviceConfig.PlaceLS[i]);
                    IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
                    indx_LW_ORL = Ind_LW ; // ��������� ������ ���������� ��������� ��� ���������
                    NeedReDraw=1;
                    break; 
                  }
                }
                break;
              }
            }
            if (!memcmp ((void*)&RxBufExt[5], "KEY",3)) // ������ ������ ��� ��� ���������
            {
              switch(RxBufExt[8])
              {
              case ' ': // �������� ���� ��������� ������ � �������� �� ��
                params[0] = atoi((char*)&RxBufExt[9]); // get param for setting
                switch(params[0])
                {
                case 1:
                  KeyCode = 0x04; //
                  break;
                case 2:
                  KeyCode = 0x7B; //
                  break;
                case 3:
                  KeyCode = 0x02; //
                  break;
                }
                sprintf((void*)AnsData, "%d(%d)-%d(%02X):%d\r",Mod_At_cnt,NeedReDraw,CntErrI2C,CntErrKbrd,CntErrAuto);//���� �����

                //sprintf((void*)AnsData,"%d\r",NumMyBeep);
                EnaSendAns = 0;

                break;
              }
            }
            if (!memcmp ((void*)&RxBufExt[5], "MOD",3)) // ������ ������ ��� ��� ���������
            {
              
              switch(RxBufExt[8])
              {
              case ' ': // �������� ���� ��������� ������ � �������� �� ��
                params[0] = atoi((char*)&RxBufExt[9]); // get param for setting
                OldCWork = CondWork; // ���������� ����� � ������ ������� ��������������
                
                switch(params[0])
                {
                case 10: // P1 dBm
                case 11: // P1 dB
                case 12: // P1 W
                  if(DeviceConfig.CfgPM)
                  {
                    CondWork = P1;
                    ModeI = params[0]%10;
                    SetNeedCondWork(CondWork);
                    EnaSendAns = params[0];
                  }
                  break;
                case 20: // P1a dBm
                case 21: // P1a dB
                case 22: // P1a dBe
                  if(DeviceConfig.CfgPM)
                  {
                    CondWork = P1a;
                    ModeIP1a = params[0]%10;
                    SetNeedCondWork(CondWork);
                    EnaSendAns = params[0];
                  }
                  break;
                case 30: // Sc OFF
                case 31: // Sc CW
                case 32: // Sc 270 Hz
                case 33: // Sc 2 kHz
                case 34: // Sc CW*
                  if(N_LS)
                  {
                    CondWork = Sc;
                    SetNeedCondWork(CondWork);
                    ScMode = params[0]%10;
                    ControlSC (ScMode); // ���������� ���������� -  ������������� �������� ������
                    NeedReDraw=1;
                    EnaSendAns = params[0];
                  }
                  break;
                case 40: // P2 dBm
                case 41: // P2 dB
                  if(DeviceConfig.CfgL)
                  {
                    CondWork = P2;
                    P2Mode = params[0]%10;
                    SetNeedCondWork(CondWork);
                    EnaSendAns = params[0];
                  }
                  break;
                case 50: // ORL
                  if(DeviceConfig.CfgL)
                  {
                    CondWork = Rl;
                    SetNeedCondWork(CondWork);
                    EnaSendAns = params[0];
                  }
                  break;
                case 60: // Ln
                case 61: // Ln
                  if(DeviceConfig.CfgL)
                  {
                    if((CondWork == Ln)&&(params[0]==61))
                    {
                      KeyCode = 0x04;
                    }
                    else
                    {
                      CondWork = Ln;
                      SetNeedCondWork(CondWork);
                    }
                    EnaSendAns = params[0];
                  }
                  break;
                case 90: // RE OFF
                case 91: // RE CW
                case 92: // RE 1Hz
                  if(DeviceConfig.CfgRE & 0x1)
                  {
                    CondWork = RE;
                    ReMode = params[0]%10;
                    SetNeedCondWork(CondWork);
                    EnaSendAns = params[0];
                  }
                  break;
                }
                break;
              case '?':
                EnaSendAns = CondWork*10;
                switch(CondWork)
                {
                case 0: // ����� ������ 
                  EnaSendAns++;
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Stra,Strb,Strd);
                  
                  break;
                case 1: // P1
                  EnaSendAns += ModeI;
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Str2,Strb,Strc,Strm);
                  break;
                case 2: // P1a
                  EnaSendAns += ModeIP1a;
                  sprintf((void*)AnsData,"%d\n%d\n%s %s %s\n",EnaSendAns,BatLevel,StPon1[0],StPon2[0],StPon3[0]);
                  sprintf((void*)AnsData,"%s%s %s %s\r",AnsData,StPon1[1],StPon2[1],StPon3[1]);
                  sprintf((void*)AnsData,"%s%s %s %s\r",AnsData,StPon1[2],StPon2[2],StPon3[2]);
                  sprintf((void*)AnsData,"%s%s\n%s\n%s\r",AnsData,Strd,Strc,Strm);
                  break;
                case 3: // Sc
                  EnaSendAns += ScMode;
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Strc,Strb,Stra,Strm,Strd);
                  break;
                case 4: // P2
                  EnaSendAns += P2Mode;
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Strc,Strb,Stra,Strm);
                  break;
                case 9: // RE
                  EnaSendAns += ReMode;
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Strc,Strb,Stra,Strm);
                  break;
                case 5: // ORL
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Strc,Strb,Stra,Strm);
                  break;
                case 6: // Ln
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Strc,Strb,Stra,Strm);
                  break;
                case 8: // TESTER AUTO
                  sprintf((void*)AnsData,"%d\n%d\n%s\n%s\n%s\n%s\n%s\r",EnaSendAns,BatLevel,Stra,Strb,Strd,Strc,Strm);
                  break;
                default:
                  sprintf((void*)AnsData,"%d\n%d\r",EnaSendAns,BatLevel);
                  break;
                }
                EnaSendAns = 0;
                break;
              default:
                break;
              }
              if(EnaSendAns)
              {
                sprintf((void*)AnsData,"%d\r",EnaSendAns);
                EnaSendAns = 0;
              }
            }
          }          
          // ������ � �������
          if (!memcmp ((void*)&RxBufExt[1], "MEM:",4)) // 
          {
            if (!memcmp ((void*)&RxBufExt[5], "FR?",3)) // ������ ��������� ������
            {
              sprintf((void*)AnsData,"%d,512\r",SettingPrm.CountMemory);
            }
            if (!memcmp ((void*)&RxBufExt[5], "CLR",3)) // ������� ������
            {
              //������ ������
              SettingPrm.CountMemory=0;
              NeedSave |=0x10; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
              sprintf((void*)AnsData,"%d,512\r",SettingPrm.CountMemory);
            }
            if (!memcmp ((void*)&RxBufExt[5], "WRN",3)) // ������ �������� ��������� � ������
            { 
              if(SettingPrm.CountMemory<512)
              {
                WriteBlockMEM (1); // ������ ����� ������ � ������, �������� �������� ��� UART EXT
                sprintf((void*)AnsData,"%d,512\r",SettingPrm.CountMemory);
                CountDrawMsg = 5;
                NeedReDraw = 1;
              }
              
            }
            if (!memcmp ((void*)&RxBufExt[5], "REM",3)) // ������� ������������ ������
            {
              params[0] = atoi((char*)&RxBufExt[8]); // get param for setting (�� ����� ������ ������������)
              if(params[0]<512)
              {
                SettingPrm.CountMemory=params[0];
                NeedSave |=0x10; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
                EnaSendOK = 1;
              }
            }
            if (!memcmp ((void*)&RxBufExt[5], "LD?",3)) // ������ ������ ������ � ������� N
            {
              params[0] = atoi((char*)&RxBufExt[8]); // get param for setting (����� ������ ������)
              if(params[0]<=512)
              {
                AdrReadMem=(uint16_t)((params[0]-1)<<6) + ADR_SaveMemory; 
                EEPROM_read(&MemoryData, AdrReadMem , sizeof(MemoryData));
                memcpy(AnsData, "#265", 4 );
                memcpy( &AnsData[4], &MemoryData, 64 ); // 
                memcpy( &AnsData[68], "\r", 1 );
                BinBlock = 69; // ������ �����
              }                
            }
          }
        } // ';'
        
      }
      // ASCII commands
      if(EnaSendOK)
      {
        sprintf((void*)AnsData,"OK\r");
        EnaSendOK = 0;
      }
      
    }
    
    if (NeedSave) // ������� ������������� ���������� ������ ������
    {
      myBeep(28);
      NumMyBeep = 17;
      WriteNeedStruct (NeedSave);
      NeedSave = 0;
      // ��� �� �������� ���������� ������������
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght,0);//����������� �������� �������� � ���������, ��� ������ ����� �����
      
    }
    
    if(BinBlock)
    {
      if(BinBlock<500)
        
        // 
        HAL_UART_Transmit_DMA(&huart1, (void*)AnsData,BinBlock); // ������ 
      else
        HAL_UART_Transmit_DMA(&huart1, (void*)SSD1305_Buffer_BMP,BinBlock); // ������ 
      
    }
    else 
      // ������� 
      HAL_UART_Transmit_DMA(&huart1, (void*)AnsData,strlen((void*)AnsData)); // ������ 
    // ������ ����� ����� ���������� 
    memset(RxBufExt,0, BUFF_SIZE_RX); // ������ ������ ���� ��� �������
    DataRdyUart1 = 0;
    CntRX = 0; // ����� �������� ������, ��� �������� �������� �������)
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    
  }
}

// �� ������ - �������
// ������ �������� ������
void ParseOptic (void) // ������ ������ �������� �� uart2 (OPTIC) 
{
  unsigned short Temp = 0;
  // signed int iTemp;
  //unsigned char *up;

  volatile uint32_t BeginStr = CntRXOpt; // ���� ��������� �� ������ ������� ��� �������
  fParamOpt = 0.0;
  iParamOpt = 0;
    if(DataRdyUart2 && CntRXOpt)
  {
 
  //sprintf((char*)Strf,"(%d-%d)%s\r",TxOptBusy,CntRXOpt,RxBufOpt) ;
  sprintf((char*)Strf,"(%d)%s\r",TxOptBusy,RxBufOpt) ;
         HAL_UART_Transmit_DMA(&huart1, (void*)Strf,strlen((void*)Strf)); // ������ 
    //myBeep(100);// ��� �� ������� ��������� ���������
    OpticCMD = NOTcmd; // ��� ������
    for (int i=0; i <CntRXOpt; i++)//���� �������������� ��������
    {
      if ((RxBufOpt[i] >=0x61) && (RxBufOpt[i]<=0x7A))RxBufOpt[i] = RxBufOpt[i] - 0x20;
      if(RxBufOpt[i]=='#') BeginStr=i;//
    }
    // �������� ����� ����� (��� ��� ������ ������)
    switch(CondWork)
    {
    case P1a:
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SL",3)) //
      {
        Temp=atoi((char*)(&RxBufOpt[BeginStr+3]));   //�������������� ���������� � ��������
        if ((Temp<800)||(Temp>1650))Temp = 0;
        else
        {
          //MemoryData.mLW[0] = Temp;
          P1aWavelenght = Temp;
          CntWaitLW = 0; // ����� �������� �������� ������ ����� �����
          YesLW=1; // �������� ������� ���������� ����������� � ��������� �����������
          SetModePM(1); // ������������ �������������� ����� P1 - ��� ��������� ���������
          CurrSpectrKoeff = GetCoeffSpctr(P1aWavelenght,0);//����������� �������� �������� � ���������, ��� ������ ����� �����
          
        }
        // ����� ������� ������� ��� "������� ����"
        if(RxBufOpt[BeginStr+7]==0x20) // ���� ���� � ������ �������� ��������
        {
          PowSC_Ext = atof((char*)(&RxBufOpt[BeginStr+7])); // ��������� ��������,����� ���� ���������� � ����� ����(��� ���������)
        }
        else
          PowSC_Ext = 0.;
        for(int i=0; i < WAVE_LENGTHS_NUM; ++i) // ��������� ������ RPON �� ������� ��������
        {
          if(CalibrLW[i] == P1aWavelenght)
            BaseExtLvl[i]=PowSC_Ext; 
        }
      }
      break; // from P1a
    case At:
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SMP",4)) //����� ����� ����� ����������� ���������
      {
        iParamOpt = atoi((char*)(&RxBufOpt[BeginStr+4]));   //�������������� ���������� � �������� - ����� ����� ��������� 
        OpticCMD = SMPcmd;
      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SLA",4)) //����� ����� ����� � �������� ����������� ���������
      {
        iParamOpt = atoi((char*)(&RxBufOpt[BeginStr+4]));   //�������������� ���������� � �������� - ����� ����� ��������� 
        if(RxBufOpt[BeginStr+8]==0x20) // ���� ���� � ������ �������� ��������
        {
          fParamOpt = atof((char*)(&RxBufOpt[BeginStr+8])); // ��������� ��������,����� ���� ���������� � ����� ����(��� ���������)
        }
        OpticCMD = SLAcmd;
      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#YP",3)) //����� �������� ����������� ��������� ���������� ��������������� ��������
      {
          fParamOpt = atof((char*)(&RxBufOpt[BeginStr+3])); // ��������� ��������,����� ���� ���������� � ����� ����(��� ���������)
        OpticCMD = YPcmd;
      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SWM",4)) //����� ������� �� ������� - 
      {
        OpticCMD = SWMcmd;
      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SOK",4)) //����� ������ �� �� Slave
      {
        OpticCMD = SOKcmd;
      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SRL",4)) //����� ������� �� ������� - 
      {
        OpticCMD = SRLcmd;
      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SWL",4)) //����� ������� �� ������� -  
      {
        OpticCMD = SWLcmd;
        //TsT(1);
        SW5V(1);// ON 5V
        //TsT(0);

      }
      if (!memcmp ((void*)&RxBufOpt[BeginStr], "#SLO",4)) //����� ����� ����� � �������� ORL �� Slave
      {
        iParamOpt = atoi((char*)(&RxBufOpt[BeginStr+4]));   //�������������� ���������� � �������� - ����� ����� ��������� 
        if(RxBufOpt[BeginStr+8]==0x20) // ���� ���� ORL
        {
          fParamOpt = atof((char*)(&RxBufOpt[BeginStr+8])); // ��������� ��������,����� ���� ���������� � ����� ����(��� ���������)
        }
        OpticCMD = SLOcmd;
      }

      break;
    }
//    // �������� ��������� ��� � "������" �������
//    // ��������� ��������� �� ������ ������
//    up=(unsigned char*)&RxBufOpt[BeginStr]; // ����������� ������ ������ �� ���� ������ "#"
//    // ������� ������ ������ �� ��������� � ����������� �� ������
//        switch (RxBufOpt[BeginStr+1]) // ������ ������ �������
//    {
//    case 'S':
//      switch (RxBufOpt[BeginStr+2]) // ������ ������ �������
//      {
//      case 'L':                           //��������� ����� ����� ���������  ###SL1310 -0.12
//        {
//          // ����� P1a P2
//          if (CondWork==P1a) //���1300(CondWork==P2)||
//          {
//            if(
//            Temp=atoi((char*)(up+3));   //�������������� ���������� � ��������
//            if ((Temp<800)||(Temp>1650))Temp = 0;
//            else
//            {
//              //MemoryData.mLW[0] = Temp;
//              P1aWavelenght = Temp;
//              CntWaitLW = 0; // ����� �������� �������� ������ ����� �����
//              YesLW=1; // �������� ������� ���������� ����������� � ��������� �����������
//              SetModePM(1); // ������������ �������������� ����� P1 - ��� ��������� ���������
//              CurrSpectrKoeff = GetCoeffSpctr(P1aWavelenght,0);//����������� �������� �������� � ���������, ��� ������ ����� �����
//              
//            }
//            // ����� ������� ������� ��� "������� ����"
//            if(RxBufOpt[BeginStr+7]==0x20) // ���� ���� � ������ �������� ��������
//            {
//              PowSC_Ext = atof((char*)(up+7)); // ��������� ��������,����� ���� ���������� � ����� ����(��� ���������)
//            }
//            else
//              PowSC_Ext = 0.;
//            for(int i=0; i < WAVE_LENGTHS_NUM; ++i) // ��������� ������ RPON �� ������� ��������
//            {
//              if(CalibrLW[i] == P1aWavelenght)
//                BaseExtLvl[i]=PowSC_Ext; 
//            }
//          } // for P1a
//          if(CondWork==At) // ������ �������
//          {
//            if(RxBufOpt[BeginStr+3]=='A' // read SLA..
//            {
//            }
//          }
//        }
//               break; // 'SL' ������ ������
//      }
//      break; // 'S'  ������ ������
//    case 'Y':
//              if ((Mod_At == SavPw)&&(RxBufOpt[BeginStr+2]=='P'))
//        {
////          PwLvl= atof((char*)(up+3)); // ���� ����� �������� ���������
////          PwS[Ind_LW] = PwLvl;
////          Cnt_SPw++;
////          if ( Cnt_SPw < NumFrstLS)Mod_At_cnt = 0;
////          else //��������� ���������� ���. ��������� � ������
////          {
////            Mod_At_cnt=0;
////            Start_Cmd = 2;
////            Mod_At = LnR_Clb;
////            sprintf(Str4, "  eaeea?iaea  ");
////            sprintf(Str5, "    aeeiu     ");
////            ShowMsg(30);
////            // ReWrite_SetConst();// ia?acaienu a iaiyou 
////          }
//        }
//
//      break;// ������ ������ 'Y'
//    }
    // ������ ����� ����� ���������� 
    memset(RxBufOpt,0, BUFF_SIZE_RX); // ������ ������ ���� ��� �������
    DataRdyUart2 = 0;
    CntRXOpt = 0; // ����� �������� ������, ��� �������� �������� �������)
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    NeedReDraw = 1;

    }

}
// ����� ��������- �� ����� ������
void HAL_UART_RxOptCallback(UART_HandleTypeDef *huart)
{
  // uint16_t tmp;
  volatile uint8_t temp;
  if ((REG_ISR && UART_FLAG_ORE) != 0)
  {
    
  }
  if ((REG_ISR & UART_FLAG_RXNE) != 0)
  {
    temp = (uint8_t)(REG_DATA & (uint16_t)0x00FF); // 
    switch (RecievOpt)
    {
    case RX_WAIT:                    //������� ����� ����� �������
      if (temp=='#')
      {
        CntRXOpt=0;
        RxBufOpt[CntRXOpt]=temp;
        RecievOpt=CMD_RECIV;
        //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3); // ����� ������ ��� ������� # ��� �����������
        CntRXOpt++;
      }
      break;
    case CMD_RECIV:                   //��������� ������� ������� ASCII �������
      
        switch (temp)
        {
        case '#':                 //����� ������� ������� ������ �������...  �������� �������
          CntRXOpt=0;
          RxBufOpt[CntRXOpt]=temp;
          //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3); // ����� ������ ��� ������� # ��� �����������
          CntRXOpt++;
          break;
        case 0xD:                 //����� ������� (��� ���� ���)
        //case 0xA:                 //���� ����� ������� �� �� �� ��������
          RxBufOpt[CntRXOpt]=temp;
          RxBufOpt[CntRXOpt+1]=0x0;
          RecievOpt=CMD_END;            //������� ������� ��������� ���� ������������
          CountWaitCmdRelaseOpt=0; // ����� ������� �������� ���������� �������
          DataRdyUart2=1; // �������� ����� ������ � ������� ������� ������������� ��������� ���� ��� �������

          break;
        default:                  //I?iaie?aiea i?e?ia eiiaiau
          RxBufOpt[CntRXOpt]=temp;
          CntRXOpt++;
//                    if (CntRXOpt == 10) // 
//                    {
//                      __no_operation();
//                    }
          if (CntRXOpt > 64) // 
          {
            CntRXOpt=0;
            RecievOpt=RX_WAIT;
          }
          break;
        }
      break;
    case CMD_END:                     //������� ������� �� ����� ������������
      if (CountWaitCmdRelaseOpt++>100) // �� �� ������� ����� �� �������������� (���� �� �� ��������)
      {
            CntRXOpt=0;
            RecievOpt=RX_WAIT;
      }

      break;                      //I?aauaouay eiiaiaa ia io?aaioaia - iiyoiio ie?aai ia aaeaou
    }
  }
  //}
  // HAL_UART_Receive_IT(&huart3, U3Buf,1);
  
}


