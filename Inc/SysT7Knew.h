/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SysT7Knew_H
#define __SysT7Knew_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Define ---------------------------------------------------------*/
//#define RELEASETIME 1613347201 // ��������� ����� ��������� ������ 15.02.2021 00:00:01
//#define RELEASETIME 1614643201 // ��������� ����� ��������� ������ 02.03.2021 00:00:01    1614643201
//#define RELEASETIME 1649152800 // ��������� ����� ��������� ������ 05.04.2022 10:00:00     ������ 3
//#define RELEASETIME 1658484000 // ��������� ����� ��������� ������ 22.07.2022 10:00:00     ������ 6
//#define RELEASETIME 1660125600 // ��������� ����� ��������� ������ 10.08.2022 10:00:00     ������ 6
//#define RELEASETIME 1660816800 // ��������� ����� ��������� ������ 18.08.2022 10:00:00     ������ 12
//#define RELEASETIME 1661162400 // ��������� ����� ��������� ������ 22.08.2022 10:00:00     ������ 12
//#define RELEASETIME 1661248800 // ��������� ����� ��������� ������ 23.08.2022 10:00:00     ������ 18
//#define RELEASETIME 1664791200 // ��������� ����� ��������� ������ 03.10.2022 10:00:00    ������ 19
//#define RELEASETIME 1664877600 // ��������� ����� ��������� ������ 04.10.2022 10:00:00    ������ 20
//#define RELEASETIME 1664964000 // ��������� ����� ��������� ������ 05.10.2022 10:00:00    ������ 21
//#define RELEASETIME 1667296800 // ��������� ����� ��������� ������ 01.11.2022 10:00:00    ������ 23
//#define RELEASETIME 1667988000 // ��������� ����� ��������� ������ 09.11.2022 10:00:00    ������ 27
//#define RELEASETIME 1673258400 // ��������� ����� ��������� ������ 09.01.2023 10:00:00    ������ 35 ����� ���!
//#define RELEASETIME 1676282400 // ��������� ����� ��������� ������ 13.02.2023 10:00:00    ������ 37 ����� ���!
//#define RELEASETIME 1676628000 // ��������� ����� ��������� ������ 17.02.2023 10:00:00    ������ - 38// ����� � MAIN � ������ 1673258400
//#define RELEASETIME  // ��������� ����� ��������� ������ 15.03.2023 10:00:00    ������ - 41// ����� � MAIN � ������ 1673258400
//#define RELEASETIME 1682503200 // ��������� ����� ��������� ������ 26.04.2023 10:00:00    ������ - 44// 
//#define RELEASETIME 1683280800 // ��������� ����� ��������� ������ 05.05.2023 10:00:00    ������ - 45// 
//#define RELEASETIME 1692093600 // ��������� ����� ��������� ������ 15.08.2023 10:00:00    ������ - 46// 
//#define RELEASETIME 1692698400 // ��������� ����� ��������� ������ 22.08.2023 10:00:00    ������ - 50// ������ ������ ��-�� ������ �������� ���������
//#define RELEASETIME 1707732000 // ��������� ����� ��������� ������ 12.02.2024 10:00:00    ������ - 52// 
//#define RELEASETIME 1709287200 // ��������� ����� ��������� ������ 01.03.2024 10:00:00    ������ - 53// 
//#define RELEASETIME 1729504800 // ��������� ����� ��������� ������ 21.10.2024 10:00:00    ������ - 54// 
#define RELEASETIME 1738220400 // ��������� ����� ��������� ������ 30.01.2025 10:00:00    ������ - 57// 
#define RELEASEYEAR 25
#define RELEASEMONTH 01
#define RELEASEDATE 30
// ����� ���������� �� ��������������...   
#define LASTTIMEOFF 30   
   
#define KEYSREAD 50 // ������ ������ ����������
#define ADCREAD 12 // ������ �������� ��������� ��� P1
   #if defined(STM32F302xE) || defined(STM32F303xE) || defined(STM32F398xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) 

#define UADC 3.0/4095 // ��� ���
#endif
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG) || \
    defined (STM32F205xx)
      #define UADC 2.5/4095 // ��� ���

#endif      
/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))
#define MAXBAT 4.1//4.2
#define MINBAT 3.1
#define MaxAT  250 // ������������ ���� � ������ At 15���, ��� 50��
   
   // ������������ �����������
#define CALIBRNUM 8 // ����� ���� ����������
#define LSPLACENUM 3 // ����� ���� ��������� �������
#define WAVE_LENGTHS_NUM 7 // ����� �������� ���� ���� ���������� (�� ��������� ������ 850,1300,1310,1490,1550,1625,1650)
#define WAVE_LENGTHS_SPEC 10 // ����� ����� ������������� ���������� (0-����� ��� 650 ��)
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(*a))
#define ADCSIZE 16 // ������ ����������� ��� P2
#define DELTAP2 4.0 // ������� ����� �������� P2
#define DELTAP2mW 0.000002 // ������� ����� �������� P2 in mW (40nW)
   
   // ORL LEVEL POROG
#define ORL_LVL_P  60.0
 // ������� ���������� ������������� ���������� ORL �� "�����" 14.7��     
#define LIM_ORL_UP  13.0
#define LIM_ORL_DN  17.0
#define LIM_ORL_CORR_NOISE -47.0
      
#define CntrlA(a) (a)?(HAL_GPIO_WritePin(ktA_GPIO_Port, ktA_Pin, GPIO_PIN_SET)):(HAL_GPIO_WritePin(ktA_GPIO_Port, ktA_Pin, GPIO_PIN_RESET)) // ON / off ktA

#define TsT(a) (a)?(HAL_GPIO_WritePin(test_GPIO_Port, test_Pin, GPIO_PIN_SET)):(HAL_GPIO_WritePin(test_GPIO_Port, test_Pin, GPIO_PIN_RESET)) // ON / off test
#define SW5V(a) (a)?(HAL_GPIO_WritePin(ENA_5V_GPIO_Port, ENA_5V_Pin, GPIO_PIN_SET)):(HAL_GPIO_WritePin(ENA_5V_GPIO_Port, ENA_5V_Pin, GPIO_PIN_RESET)) // ON / off 5V
 
// ������������� ������� ������� ��� �������� ��������� ����������
#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC
// ������������ �������� ��� ���������� �����
#define xImp 25
#define NUMMEASLN 4 // ����� �������� �����
#define SEQMEASLN 8 // ������������������ �������� ������     
      //
   // ������������� �������� ��� ��������� UART � ����������� �� ���������� 
// �������� ������ � ������ ���������� 
  // for F103 
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG) || \
    defined (STM32F205xx)
#define Clr_UART huart2.Instance->DR
//#define REG_ISR huart->Instance->SR

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

#define Clr_UART huart2.Instance->RDR
//#define REG_ISR huart->Instance->ISR

#endif


 /* Includes ------------------------------------------------------------------*/
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "pm.h" // ������� ���������� P1
#include "DrawFunctions.h"
#include "fonts.h"
#include "pca955x.h"
#include "ssd1305.h"
#include "KB_processing.h"
#include "HardSetDevice.h" // ������� ���������� "�������" 
#include "usartExt.h" // for UART 
#include "MyClock.h"
#include "Automat.h"
   
   
  
   
   
//const uint16_t CalibrLW[WAVE_LENGTHS_NUM]={850,1300,1310,1490,1550,1625,1650};
   
////���������� ������� ������
enum WorkM {SetFn, P1, P1a, Sc, P2, Rl, Ln, Ls, At, RE, Clock, SetPrm}; // ���������� ������������
//enum WorkM CondWork = P1, OldCWork = SetFn, ConSet; //������� ����� ������, ������ ����� ������, ���������� ����� (����� ������)
//���������� ��� ���������� �������� ������ � ���������
enum Condition {dBm, dB, W, RCell, WCell, Loss_Lim_Set, Hold, WrHold}; // ����� ��������� �������, ������, ��� ���������� P2
enum CondSource {OFFs, SCW, S27W, S2KW, SCW_P, OFF_S}; // ��� ��������� + OFF_S (��������� �������� ���������)
enum AtM {Wt, Orl, Px, Sl_Px, Sl_Orl, AskOrl, AnsOrl, Lnx, LnR, CmdSw, AnsOK, SavPw, CalPw, Err_Ans, View, Clb_Err, LnR_Clb, Cfg_At}; // ��������� ��������������� ������
enum CondView {LOSS, ORL, LNM, BasV}; // ��������� ��� ��������� ����������� PON ����������
enum SubSetting {sTime, sAutoOff, sNumF, sLOSSm, sLOSS, sORL, sSCp, sMem, sSWm, sVer};   // ��������� �������, ����� ������� � ������� ����������, ������� ��������� ��������� P1a, �������� ���������, ���� ������� ������, ������������� ��������� ���������� ORL-Loss-Ln in At.
enum CodeCmd {NOTcmd, SMPcmd, SLAcmd, YPcmd, SWMcmd, SOKcmd, SRLcmd, SWLcmd, SLOcmd}; // ���(�����) �������� ������� �� ������
// ��������� ����� ������ �������
typedef struct
{
  uint32_t mTime; // ����� ������
  uint16_t mNumFiber;// ����� �������
  uint16_t mLW[3]; // ����� ����� ������� (3 ��������)
  float mRes[3][4]; // ���� ������ ������ (3 ����� �� 4 ������ Float)
  uint16_t mLen; // ����� �������
  uint8_t mCode; // ��� ������ ��� �����������
  uint8_t mFree; // ������ 
  // ���� ������ 64 ����� (512 ������� 
} MEM_BLK;
// ��������� ����� �������
typedef struct
{
uint8_t cYear; //������� �����
uint8_t cMonth;// �����
uint8_t cDay;// ����
uint8_t cHour;//���
uint8_t cMinutes;//������
uint8_t cSeconds; //�������
//Alarm
uint8_t aYear; //������� �����
uint8_t aMonth;// �����
uint8_t aDay;// ����
uint8_t aHour;//���
uint8_t aMinutes;//������
uint8_t aSeconds; //�������
} CLOCK_DEV;

// ��������� ������������ �������
typedef struct
{
uint16_t NumDevice; //2 number device
uint8_t NameDevice[16];//16 ������ ������������ �������
uint8_t CfgPM;//1  ������������ ���������� 0 - ���, 1- ��������, 2 -�����
uint8_t CfgRE;//1 ������������ �������� ����� 0 - ���, 1 - ���� + newPlate 0x2 2- ��� ������ ������ ���������� 3- ����
uint8_t CfgL;//1 ������������ ������� � ���������
uint16_t PlaceLS[LSPLACENUM]; //6  ����� 1, (1310) ������������ ������������� ������� �� ������ 2byte   
uint8_t CfgOUT[LSPLACENUM];//3 ������������ ������ � ������ ��������������� ������������ ���� ���� 0- �� � ������, 1- � ������ (������� "d"), �� ������ ��� ������� ������ ����
uint32_t SmPx[WAVE_LENGTHS_NUM];// 28�������� (���������) ��� ���������� P2 ���� ���� ���������� //28
// ������������� ������������ ���������� ����� (
float KGen; //4
float MZone; //4
float Kprop; // ����� ������������������ ��� ������� �������� P2 4
uint8_t YearManuf;// ��� ������������
//27 byte+12
} DEV_CFG;


// ��������� ������������� ���������� ��������
typedef struct
{
int ShZeroRng[4];//16 ��������� �������� ����������
float CoefStykRange[4]; //16 ������������ �������� ����������={1.0,1.0,1.0,1.0};
float CoefSpctrL[6]; //24 ������������ ������������ ����. 800-900={NumWave = 0}
float CoefSpctrM[10]; //40 ������������ ������������ ����. 1210-1390 (NumWave = 1,2}
float CoefSpctrH[13]; //52 ������������ ������������ ����. 1410-1650={NumWave = 3,4,5,6}
float CoefPointKlb[WAVE_LENGTHS_NUM]; //28 ������������ �������� ���� ���� ����������=(�� ��������� ������ 850,1300,1310,1490,1550,1625,1650)
float CoefTune[3]; //  ������������� ������������ ��� ���������� 0-(800-900);1-(1210-1405);2-(1410-1650)
float BaseRet[WAVE_LENGTHS_NUM]; // ������� ������ ��� ������� ��������� ��������� (��� ���� �.�. ���� �������� �  � ������ �������)
float Pow_mW_Clbr; // ������� �������� ��� ������ ��������� ORL 
//168 byte
} TAB_SET;
// ��������� ������������� ���������� ��������
typedef struct
{
uint16_t PointSpecKalib[WAVE_LENGTHS_SPEC];//������������� ����� ���� ���������� ���� 0 �� �� ���=����� ����� ������������� ���������� (0-����� ��� 650 ��)�� 10 ��.
float CoefSpecKlb[WAVE_LENGTHS_SPEC]; // ������������ �������� ������������� ���� ���� ����������= �� 10 ��.
    // NumWave = 7... 
} TAB_SPECIFIC;

// ��������� ����������� ������ ���������(��������), ����� �������,  ���� ���������, �������� ���������� ���������� � ���������, ������� ������
typedef struct
{
float PowSc[LSPLACENUM]; // �������� �������� ��������� ������������� ������� //12
uint16_t NumberFbrCount;// ����� ������� 2
uint8_t EnaAutoInkrNF; // ������� ���������� �������������� 1
uint8_t EnaCmpLOSS; // ������� ���������� ��������� ������ ��� ��������� ��������� ��� 3 ���� ���� ����������  
uint8_t EnaCmpORL; // ������� ���������� ��������� ������ ��� ��������� ORL ��� 3 ���� ���� ����������  
uint8_t EnaWrtMem; // ������� ���������� ������ ��������� � ������ 1���-LOSS, 2-ORL, 3-Ln(�����)
uint16_t CountMemory; // ������� ����� ������ � ������� �������� ������
} TAB_DEVS;

// ��������� ���������������� ������ ��� �������
typedef struct
{
  //������� ������ ���������� P1
float BaseLevel; // ������� ������� ������� 4
float BaseLvl[WAVE_LENGTHS_NUM]; // ������� ������� ��� ������������� ���� ���� (7 ��) 28
float BaseSpecKlb[WAVE_LENGTHS_SPEC]; // �������� ������� ������� ��� ������������ ���� ���� = �� 10 ��. 40
  //������� ��������� ��������� ��� ��������� � ORL � �������������� ������� �������� ����������
float Lim_LS[5]; // ������ ��� ��������� 12 (+8)
float Lim_ORL[3]; // ������ ��� ORL 12
float BaseLevelP2; // ������� ������� ������� ��� �2 4
float BaseLvlP2[WAVE_LENGTHS_NUM]; // ������� ������� ��� ������������� ���� ���� (7 ��) ��� �2  28
//132+8
uint16_t CalibLnAutoMem; //���������� ���������� ����� � �������� +2 = 142 
// 30.09.2022 - ��������� ���� ���������� ������� ��� ������������� ��� ���������
uint16_t PMWaveLenghtMem; // ����� ����� ��������� P1
uint8_t CondWorkMem; // ������� ����� ������ ��� ������������� P1,P1a,Sc,P2 
uint8_t ScModeMem; // ����� ���������, �������� ���� ���������
uint8_t Ind_LWMem; // ������ ����� ����� ��������� �� �����������
uint8_t ModeIMem; // ����� ��������� P1
uint8_t ModeIP1aMem; // ����� ��������� P1�
uint8_t P2ModeMem; // ����� ��������� P2
uint8_t IndxP2LWMem; //������ ����� ����� ���������� P2
uint8_t AutoOff; // ������� ���� ���������� ( �������� ����� �� ����� ...) ���� ���������� ���� FFFF �� 0 ���, ���� ���� 1...������ ��� ���������
uint8_t AutoOffMod; // �����  ���� ���������� ( ��������� �������),�.�. ��������� ������������� � ����������, ����� ������� 
// ���������/���������� ����� �������� Mode+S ������ �������, �� ��������� ��� � ������ �����+����
} TAB_USER;

// ���������  ������ PON AUTO
typedef struct
{

float PW_AB[WAVE_LENGTHS_NUM+1]; // �������� ���������� �������� B ���������� ������� �  A 
float PW_BA[WAVE_LENGTHS_NUM+1]; // �������� ���������� �������� � (�) ������������ �������� PS_BA
float PS_BA[WAVE_LENGTHS_NUM+1]; // �������� ��������� ������� �(�������� � A)  
float LS_AB[WAVE_LENGTHS_NUM+1]; // ������ (���������) ���������� �� � � �
float LS_BA[WAVE_LENGTHS_NUM+1]; // ������ (���������) ���������� �� � � �
float ORL_AB[WAVE_LENGTHS_NUM+1]; // �������� ��������� ���������� �� ���� A->B
float ORL_BA[WAVE_LENGTHS_NUM+1]; // �������� ��������� ORL ���������� �� ������� � (����� �������� ��������)
// +1 ����� �� "������" �� ������ ���� ������ ����� �� �� ������ ������������� (� ��� ��������� ��������
uint16_t LW_BA[3]; // ����� ����� ��������� ������� � (�������� � A) 
uint16_t LW_AB[3]; // ����� ����� ��������� ������� A (����������� �� ������������ �������) 
uint16_t LnA; // ����� ����� 

} TAB_PONAUTO;

// for DrawFunction
extern uint8_t Str[64];
extern uint8_t Stra[32];
extern uint8_t Strb[32];
extern uint8_t Strc[32]; // 
extern uint8_t Strd[32];
extern uint8_t Stre[32];
extern uint8_t Strf[32];

extern uint8_t Strm[32]; // 
extern uint8_t Str2[32]; // ����������� ��������� � ������  
extern uint8_t StrDrM[4]; // ������ ����������� ������ ������ �� ����������
extern uint8_t StrBat[4]; // ������ ����������� ������ ������ �� ����������

// for Automat
extern uint8_t KeyCode; //��� ������ ��� ������ ����������� (msp430 Inp)
extern uint32_t CntErrI2C; // ������� ������ I2C  


extern uint8_t StPon1[3][5], StPon2[3][8], StPon3[3][8]; //������ ��� ������������� ������� �� ������������

extern const uint16_t NumVer; 

extern TAB_PONAUTO PonA; // ���������  ������ PON AUTO
extern DEV_CFG DeviceConfig; // ��������� ������������ �������
extern TAB_SET CoeffPM; // ������� ������������� ���������� (��������, ��������, ����������...)
extern TAB_SPECIFIC SpecCoeffPM; // ������� ������������� ���������� (������������� ���� ����)
extern TAB_USER UserMeasConfig; // ������� ���������������� ������ ��� �������
// ��������� ����������� ������ ���������(��������), ����� �������,  ���� ���������, �������� ���������� ���������� � ���������, ������� ������
extern TAB_DEVS SettingPrm; // 
extern CLOCK_DEV Clocks; // ����
extern MEM_BLK MemoryData; // ���� ������ ���������� (�������) � ��� ���������� ������� �������
//���������� ������� ������
extern enum WorkM CondWork , OldCWork, ConSet, CondRet; //������� ����� ������, ������ ����� ������, ���������� ����� (����� ������)
//���������� ��� ���������� �������� ������ � ���������
extern enum Condition ModeI, OldMode, P2Mode; // ����� ��������� �������, ������, ��� ���������� P2
extern enum CondSource ScMode, OldScMode, ReMode; // ��� ��������� + OFF_S (��������� �������� ���������)
extern enum AtM Mod_At;// ��������� ��������������� ������
extern enum CondView Mod_View; // ��������� ��� ��������� ����������� PON ����������
extern enum SubSetting SetupMod;
extern enum CodeCmd OpticCMD; //������� �������� �� ������ (� ������ �������)
extern int iParamOpt; // ������������� �������� ��� ������ �� ������
extern float fParamOpt; // float �������� ��� ������ �� ������
// for UART
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
// for Timer Generator Zimp
extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;
extern uint8_t CntLCD_LN; // ����������� ������� 12 �� ��� �������� ���������� ���������� ��� ����������� �����������
extern uint8_t CntLCD_P1; // ����������� ������� 50 �� ��� �������� ����������

// ���������� ���������� ���������� ��������� 
extern uint8_t g_EnaSetup;
extern uint8_t N_LS;
// Source
extern uint8_t lSofn; // ������� ��������� ����������� ������
extern uint8_t TxOptBusy; // ������� ��������� UatrOptic �� �������� �� ������
extern uint8_t StrTXOpt[64]; // ������ �������� ������ �� ������
// �������
extern volatile uint8_t NeedTransAnsver; // ������������� ������ Slave �� �������� ������� �� �������
extern uint8_t DisRecImp; // ������ ������ ���� �������� ������� (����� �� 3-5 ��) (����� ��������� ������)
extern uint16_t CalibLnAuto ; // �������� ����� ����� ���������� � ������ �������!
extern uint16_t IndxShowLN; // ������ ��������� ���������� ���������� �� ���������

extern volatile uint32_t WaitCntLnX; // ������� ����� � �� ��� �������� �������� ������ ������� �� Slave � ������ LnX
extern volatile uint32_t Beg_Evnt; // 
extern volatile uint32_t End_Evnt; // 
extern volatile uint32_t Time_Evnt; // 
// for ADC
extern float Ubat, OldUbat; // ���������� ����������� (��� ���������� ����������)


extern uint8_t NeedSaveChange; // ������� ������������� ���������� ������ ������
extern uint8_t IndxEditClock; // ������ ��������� �� �������� �������� in Clock
extern uint8_t IndxEditNumFbr; // ������ ��������� �� �������� �������� in Number Fiber
extern uint8_t IndxEditLOSS; // ������ ��������� �� �������� �������� LOSS
extern uint8_t IndxEditLOSSm; // ������ ��������� �� �������� �������� LOSS MM 850,1300
extern uint8_t IndxEditORL; // ������ ��������� �� �������� �������� ORL
extern uint8_t IndxEnaSave; // ������ ��������� �� �������� �������� ���������� ����������
extern uint8_t IndxPowSC; // ������ ��������� �� �������� �������� �������� ���������
extern uint8_t IndxClrMEM; // ������ ��������� ���������� ������� ������
extern uint8_t IndxInvView; // ������ ��������� �� �������� ��� ������ ����������� � �������� (1-������ �����, 2-������, 4-������)
extern uint8_t MemBlock[256]; // ���� ��� ���������� ����� ������
extern uint8_t CountDrawMsg; // ������� ��������� ������� ( P1 - ������ � ������)

extern float BaseExtLvl[7]; // ������� ������  ��������� ��������� �����.
extern uint8_t MsgAuto[3][20]; // ������ ������ � ������ �������
extern uint32_t TestCntWait;// �������� ������� ��������� (� ������ ������ P2) 
extern uint16_t Dummy;
extern HAL_StatusTypeDef StatusI2C2;
// ������� ���� ����������
extern int CntPwrOffCount; // ������� ������� ��������������
// test var
extern volatile uint16_t NumMyBeep;



   
#ifdef __cplusplus
}
#endif
#endif /*__ sysT7Knew_H */
