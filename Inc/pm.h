#ifndef __PM_H__
#define __PM_H__

// ��������� ������ ��� - AD7782
#define FREEADC 0 // ��� ����� ���������
#define READYDATA 1 // ������ ������
#define BUSYADC 2 // ��� ������
#define PM_CS(a)    ((a>0)?(PM_CS_GPIO_Port->BSRR = PM_CS_Pin):(PM_CS_GPIO_Port->BSRR = (uint32_t)PM_CS_Pin << 16))  /* set CS_PM PB.04 to high/low */
#define PM_CLK(a)    ((a>0)?(PM_SCLK_GPIO_Port->BSRR = PM_SCLK_Pin):(PM_SCLK_GPIO_Port->BSRR = (uint32_t)PM_SCLK_Pin << 16))  /* set CLK_PM PB.08 to high/low */
#define GET_PM_DATA    HAL_GPIO_ReadPin(PM_DOUT_GPIO_Port, PM_DOUT_Pin)  /* set DATA_PM PB.05 to high/low */
//#define ADC_STEP (double)(7.9261495712E-7)
#define ADC_STEP (double)(2.5/2^24)

//#define KEY1(a)       CtrlExpand (COMMCTRL, PM_K1, ((a>0)?(DSBL):(ENBL))) // �������� 1
//#define KEY2(a)       CtrlExpand (COMMCTRL, PM_K2, ((a>0)?(DSBL):(ENBL))) // �������� 1
//#define KEY3(a)       CtrlExpand (COMMCTRL, PM_K3, ((a>0)?(DSBL):(ENBL))) // �������� 1
  /* set K1 to high/low */



// ������������� ������ ������ �� ���
uint32_t ReadDataADC7782 (void);

// 
uint32_t GetPMData(void);              // ������ ������ �� ���
//void SetRange(unsigned char Rn);                            // ��������� ������ �������� ���������� ���������
uint8_t GetRange(void);                        // ���������� ������� �������� ������ �������������� ���������
uint8_t GetModePM (void); // �������� ����� ����������
uint8_t SetModePM( uint8_t Mode); // ����������  ����� ����������
uint32_t GetPMDataNorm(void);   // ��������� ����� ��� �������������     

float GetCoeffSpctr(uint16_t Lambda, int8_t Mode);   // ��������� ������������� ������������
double GetPower(unsigned int Lambda);       // ������ �������� ��� �������� ����� �����
double GetPMValue (void); //��������� ������� ��������, ��� �������� � ������������� ������������!
float GetPMValuedBm (double PowOpt); //��������� ������� �������� � dBm
float GetPMValuedB (double PowOpt); //��������� ������� �������� � dB
// ��������� � ������ ������ �������� ������ ��� �������� ��� P1


void SetNewBaseLevel (void);

// ��������� �������� ��������
//void SettingZeroPoints (void);

void InitTableCoeff (void); // ������������� ������������� ��������� ����������

        // ������� �������� � ������ ��� �2
void CalkP2Mode (void);

//void InitScreenPM (void); // ������������� ��������� ���������� ����������
//void SetLasersAsMode(void);
//void SetOnLaserPlace (int Mode); //��������� ������ �� �����
//uint16_t SetUartCmdLS (uint8_t Mode, uint8_t Channel); // ��������� ������� �� ������� UART
// Variables
extern uint16_t PMWavelenght;  // ������� ����� �����
extern uint16_t P1aWavelenght;  // ������� ����� ����� P1a

extern float CurrBaseLevel; // ������� �������� �������� ������ ��� ������ ����� �����
extern float OldBaseLevel; // ������ �������� �������� ������
extern double OptPow; //������� �������� �������� � ����������� �� �������������� ��������� � ��������� � ����� �����, ����������� � ShowPM
extern float Opt_dB; //������� �������� �������� � ����������� �� �������������� ��������� � ��������� � ����� �����, ����������� � ShowPM
extern float Opt_dBm; //������� �������� �������� � ����������� �� �������������� ��������� � ��������� � ����� �����, ����������� � ShowPM
extern float CurrSpectrKoeff; // ������� ������������ �����, ������������� � ��������������� ��� ��������� ����� �����
extern float volatile RPON[WAVE_LENGTHS_NUM]; // ������ �������� � ������ ������� P1a PON 0-850,1-1300,2-1310,3-1490,4-1550,5-1625,6-1650  
extern const uint16_t CalibrLW[WAVE_LENGTHS_NUM];

extern float P2dBmValue;                 // ������� �������� �������� ��� �2 � ����������� �� �������������� ��������� � ��������� ����� ����� 
extern float P2dBValue;                 // ������� �������� �������� �2 ����������� �� �������������� ��������� � ��������� ����� �����
extern float P2ORLValue; // ������� �������� ORL �� ������ P2 (�� ����� ���)

extern uint8_t IndxP2LW; // ������ ����� ����� ���������� ��� ������������ � �2
extern uint32_t BuffP2[ADCSIZE]; // ������ ������ ��� P2 (Fifo)
extern float MediumP2;  // ������� �������� ��� �2
extern float MediumP2_Ind; // ������� �������� ��� �2 ��� ������� ���������

extern float P2UP_Lim; // ������� ����� ����� ���������
extern float P2DN_Lim; // ������ ����� ����� ���������

extern float BuffP2mW[ADCSIZE]; // ������ ������ ��� P2 mW(Fifo)
extern float MedP2mW;  // ������� �2 � ���������
extern float MedP2mW_Ind; // ������� �2 � ��������� ��� ������� ���������
extern float P2UPmW_Lim; // ������� ����� ����� ��������� ��� ���������
extern float P2DNmW_Lim; // ������ ����� ����� ��������� ��� ���������
extern float MedP2mW_corr;  // ������� �2 � ��������� � ����������

extern uint32_t P2SUM;
extern float P2SUMmW;
// ORL - ������� ������ �� ��� P2 - 
extern float BaseRetCurrORL[WAVE_LENGTHS_NUM]; // ������� ������� �������� ��� ������� ORL ������� �� ����� �����, ����� ��� �����������
// ������������� � "�����" (13-17 �� ORL)

extern float ORLCalc; // ��������� �������� ORL
extern float ORLCalcFree; // ��������� �������� ORL
extern float ORLCalcCorr; // ��������� �������� ORL
extern double PowP2_mW; // �������� �� ����� P2 � ���, ����������
extern float PowClbr_mW; // �������� ������������� ��� ����� �������� ORL � ���
extern uint16_t indx_LW_ORL; // ������ ����� ����� ��������� ORL, 

// P1a
extern uint32_t CntWaitLW;// ������� �������� ����� ����� � ������ P1a
extern uint32_t YesLW; // ������� ���������� ����� ����� ��� ���������� ����������� � ��������� ������, ���� 0 �� �������
extern uint8_t EnaOpticUART; // ������� ���������� UARt2 (Optic)

//extern unsigned int g_CountRepetCWADD; // ������� ������� ������������ � ������ CW_ADD
//float GetCoefPointKlb(unsigned int Lambda);              // Aica?auaao aiiieieoaeuiue eiyooeoeaio aey eaeea?iaaiiuo aeei aiei
//
//
//// "Aiaoiea" ooieoee
//void SetSwitchPMMode(BYTE SwMode);          // Onoaiaaeeaaao ?a?ei ia?aee??aiey 
//                                            // eiyooeoeaioia oneeaiey ecia?eoaey iiuiinoe (?o?iie eee aaoiiaoe?aneee)
//unsigned int GetPMWavelenght(signed char DeltaLambda); //Aica?auaao, eeai eciaiyao oaeouo? aeeio aiei
//unsigned int SetPMWavelenght (unsigned int CurrPMWavelenght); // i?eioaeoaeuiay onoaiiaea oaeouae aeeiu aieiu
//float Watt2dB(char *str, float mWatt,BYTE TypeRslt);    // I?aia?acoao iAo a ieAo (iAo), aA eee aAi
//int GetTypeRslt();                          // Aica?auaao oaeouaa cia?aiea oeia aa. eca?aiey
//int SetTypeRslt(BYTE type);                 // Onoaiaaeeaaao iiaia cia?aiea oeia aa.ecia?aiey
//int AcquireShZeroRng(void);                 // Ecia?aiea o?iaiae niauaiey
//int AcquireShZeroLowRng(void);                          // Ecia?aiea o?iaiae niauaiey ia ie?iai aeaiaciia (ia?aonoaiiaea ioey)
//int SetDataADC (int Data); // onoaiiaea aaiiuo AOI a i?a?uaaiee
//void SetStateADC (BYTE State); // onoaiiaea ?a?eia AOI
//BYTE GetStateADC (void); // iieo?aiea ?a?eia AOI
//int AcquireCoefStykRange(BYTE Rng, float* PrevRng, float* CurrRng);         // Au?eneyao noueiau?iue eiyooeoeaio oaeouaai aeaiaciia n i?aauaouei
//float GetCoefSpctr(WORD Lambda, float RealPower);     // Aica?auaao niaeo?aeuiue eiyooeoeaio aey oaeouae aeeiu aieiu
//float GetCoefSpctrKlb(WORD index, float RealPower);    // Aica?auaao niaeo?aeuiue eiyooeoeaio aey eaeea?iai?iuo aeei aiei
//// onoaiiaea aeeiiu aieiu anee aee??ai Wide e JDSU iiaaa??ea
//void SetAutoLamdaJDSU (DWORD Freq);
//// oioei iieo?eou aeeiio aieiu ii ?anoioa
//unsigned int GetLambdaJDSUFreq (unsigned int Freq);
//



#endif  /* __PM_H__ */
