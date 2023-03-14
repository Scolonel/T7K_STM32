#include "main.h"

#define SIZE_BUF 8


//#define AUTO      1
//#define MANUAL    0

//static BYTE Range = 0;                  // �������� ������ ����������
//static uint8_t CurrRange = 0;                  //  ������� �������� ������ ���������� ���� ����������� �� ������������� (�� ���������� ������)
//static BYTE TypeRslt=0;                 // ������� ��������� : ��� - 0, �� -1, ��-2
const uint16_t CalibrLW[WAVE_LENGTHS_NUM]={850,1300,1310,1490,1550,1625,1650};

float volatile RPON[WAVE_LENGTHS_NUM]; // ������ �������� � ������ ������� P1a PON 0-850,1-1300,2-1310,3-1490,4-1550,5-1625,6-1650  

static uint32_t Watchdog;
uint8_t StateADC = FREEADC;
static int DataADC_PM=0x00;
static uint8_t ModePM = 1;                  // ����� ������������ ����������
static int FreeCodeADC_PM=0x00;       // ����������� �������� ����� ��� ��� �������� ���������

uint16_t PMWavelenght=1310;  // ������� ����� ����� P1
uint16_t P1aWavelenght=0;  // ������� ����� ����� P1a

static double PMCurrValue=0;                 // ������� �������� �������� � ����������� �� �������������� ��������� ��� �������� � ����� �����
double OptPow; //������� �������� �������� � ����������� �� �������������� ��������� � ��������� � ����� �����, ����������� � ShowPM
float Opt_dB; //������� �������� �������� � ����������� �� �������������� ��������� � ��������� � ����� �����, ����������� � ShowPM
float Opt_dBm; //������� �������� �������� � ����������� �� �������������� ��������� � ��������� � ����� �����, ����������� � ShowPM
uint8_t IndxNumWave; // ������ ����� ����� ��� ��������� ����� ����� ��� ������������ �� �������������
// � ����������� ������ ����-���� �������������� Num2Lengh Lengh2Num
float CurrSpectrKoeff; // ������� ������������ �����, ������������� � ��������������� ��� ��������� ����� �����
// �������������, � ����� ���������� � ��������������� ������� �������
static float PMdBmValue=0.0;                 // ������� �������� �������� � ����������� �� �������������� ��������� � ��������� ����� ����� 
static float PMdBValue=0.0;                 // ������� �������� �������� � ����������� �� �������������� ��������� � ��������� ����� �����
float CurrBaseLevel=-84.0; // ������� �������� �������� ������ ��� ������ ����� �����
float OldBaseLevel=0.0; // ������ �������� �������� ������
// ���� �� �������� ������ ���������� ��� ������ � �2
float P2dBmValue=0.0;                 // ������� �������� �������� ��� �2 � ����������� �� �������������� ��������� � ��������� ����� ����� 
float P2dBValue=0.0;                 // ������� �������� �������� �2 ����������� �� �������������� ��������� � ��������� ����� �����
float P2ORLValue; // ������� �������� ORL �� ������ P2 (�� ����� ���)
float CurrP2BaseLvl=0.0; // ������� ������� ������� ��� �������� � ����� �����
uint8_t IndxP2LW = 2; // ������ ����� ����� ���������� ��� ������������ � �2
// ���������� ������� �� ��� ��������� � �����������
uint32_t BuffP2[ADCSIZE]; // ������ ������ ��� P2 (Fifo)
float MediumP2;  // ������� �������� ��� �2
uint32_t P2SUM;
float P2UP_Lim = 0.0; // ������� ����� ����� ��������� �� ����� DAC
float P2DN_Lim =4095.0; // ������ ����� ����� ���������
float MediumP2_Ind = 0.0; // ������� �������� ��� �2 ��� �������� � ���������

// ���������� ��� ������ ��������� P2 � ��� (��� ORL)
float BuffP2mW[ADCSIZE]; // ������ ������ ��� P2 mW(Fifo)
float MedP2mW;  // ������� �2 � ���������
float P2SUMmW;
// � ������ 14. �� 1 ��� ��� ���� 
float P2UPmW_Lim = 0.0; // ������� ����� ����� ��������� ��� ���������
float P2DNmW_Lim = 10.0; // (100pW)  ������ ����� ����� ��������� ��� ���������
float MedP2mW_Ind = 0.001; // ������� �2 � ��������� ��� ������� ���������
float MedP2mW_corr;  // ������� �2 � ��������� � ����������

// ORL - ������� ������ �� ��� P2 - 
float BaseRetCurrORL[WAVE_LENGTHS_NUM]; // ������� ������� �������� ��� ������� ORL ������� �� ����� �����, ����� ��� �����������
// ������������� � "�����" (13-17 �� ORL)
float ORLCalc; // ��������� �������� ORL
float ORLCalcFree; // ��������� �������� ORL
float ORLCalcCorr; // ��������� �������� ORL
double PowP2_mW; // �������� �� ����� P2 � ���
float PowClbr_mW; // �������� ������������� ��� ����� �������� ORL � ���
uint16_t indx_LW_ORL; // ������ ����� ����� ��������� ORL, 
// P1a
uint32_t CntWaitLW=0;// ������� �������� ����� ����� � ������ P1a
uint32_t YesLW=0; // ������� ���������� ����� ����� ��� ���������� ����������� � ��������� ������, ���� 0 �� �������
uint8_t EnaOpticUART=0; // ������� ���������� UARt2 (Optic

TAB_SET CoeffPM; // ������� ������������� ���������� (��������, ��������, ����������...)
//unsigned int g_CountRepetCWADD=0; // ������� ������� ������������ � ������ CW_ADD
TAB_SPECIFIC SpecCoeffPM; // ������� ������������� ���������� (������������� ���� ����)

uint32_t GetPMData(void)       
{
  
  
  switch (StateADC)
  {
  case FREEADC: 
    PM_CS(0);               // ��������� ���� ��������� ������ CS
    Watchdog=0; // �������� ������� �� ��������� (25 ��)
    StateADC = BUSYADC;
    break;
  case BUSYADC: // 
    Watchdog++; // ���� ���������� ���
    
    if (Watchdog > 10) // �� ��������� ����� 250 ��
    {
      // ������ ����� �� ������ ������ (������ ������ ������
      ReadDataADC7782 ();
      StateADC = FREEADC;
      PM_CS(1);
    }
    else
    {
      if (!(GET_PM_DATA)) // ������ ������ ����� ���������
      {
        DataADC_PM =  ReadDataADC7782 ();
        Watchdog=0; //
        StateADC = READYDATA;
      }
    }
    
    break;
  case READYDATA: // ������ ������ (����� ��������� ����������� ��)
    Watchdog=0; // 
    PM_CS(1);
    //CurrRange = Range;
    PMCurrValue = GetPower(PMWavelenght);      // �������� �������� � ��� �� �������� �� ����� �����
    StateADC = FREEADC;
    break;
  }
  
  //FreeCodeADC_PM = 0x800000-DataADC_PM;
  //FreeCodeADC_PM = 0xFFFFFF-DataADC_PM; // ��� ��������� ��������
  if(DeviceConfig.CfgRE>>1) // ���� ����� ����� ���������� 03.03.2023
    FreeCodeADC_PM = 0x800000 - DataADC_PM;
  else // ������ ���������� �����
    FreeCodeADC_PM = DataADC_PM-0x800000;
  return  FreeCodeADC_PM;                           // 
}

void MyDe_t (int lin) // ������������ ������������ ����������� ��������
{
  for (int i=0;i<lin;i++);
}

double GetPMValue (void) //��������� ������� �������� � ����������
{
  return PMCurrValue;
}

float GetPMValuedBm (double PowOpt) //��������� ������� �������� � dBm
{
  float lim=-85.0;
  PMdBmValue = (float)(10.0*log10(PowOpt));

  switch (DeviceConfig.CfgPM)
  {
  case 1: // ������� ��������
    lim = -85.0;
    break;
  case 2: // ������� ��������
    lim = -60.0;
    break;
  default:
    lim = -85.0;
    break;
  
  }
  if(PMdBmValue<lim) PMdBmValue = lim;
     
  return PMdBmValue; 
}

float GetPMValuedB (double PowOpt) //��������� ������� �������� � dB 
{
  PMdBValue = (GetPMValuedBm(PowOpt) - CurrBaseLevel);
  return PMdBValue;
}

uint32_t GetPMDataNorm(void)   // ��������� ����� ��� �������������     
{
  return  FreeCodeADC_PM;                           // 
}

// ������������� ������ ������ �� ���
uint32_t ReadDataADC7782 (void)
{
  uint32_t Data=0;
  for (int i=0; i<24; i++)
  {
    PM_CLK(0);
    MyDe_t(2);
    if ((GET_PM_DATA)!=0) Data++;
    PM_CLK(1);  
    Data = Data<<1;
    MyDe_t(4);
  }
  return Data>>1;
}


uint8_t GetRange(void)                        // ���������� ������� �������� ������ �������������� ��������� ���������
{
  return Range;//CurrRange;
}

uint8_t GetModePM (void) // �������� ����� ����������
{
  return ModePM;
}

// ������������� ����� ������������ ������������� �������� 
// ���������� �������� (������ ��� ��������������)
uint8_t SetModePM( uint8_t Mode) // ����������  ����� ����������
{
  if (Mode) ModePM = 1; // auto
  else ModePM = 0; // manual
  return ModePM;
}



// ����� ������� � ������ �����������
uint8_t SearchIndxNW (uint16_t LW)
{
  uint8_t i=0,yesLW=0;
  for (i=0; i<WAVE_LENGTHS_SPEC; i++)
  {
    if(LW == SpecCoeffPM.PointSpecKalib[i])
    {
      yesLW=1;
      break;
    }
  }
  if(yesLW) yesLW=WAVE_LENGTHS_NUM+i;
  return yesLW;
}

// ��������� ������������� ������������ � ����������� �� ����� �����
// ��������� 800-900, 1210-1390, 1410-1650 
// ������ ������������� �����. �� ����� �����, ��������� �������� IndxNumWave - � ����������� �� ����� �����
// ������������ ����� ����� �� ������������ � ���� ������
float GetCoeffSpctr(uint16_t Lambda, int8_t Mode)
{
  static float k_c =1.0;    // ������������ �����������
  unsigned int i;    // ������
  double a,b;        // ������� �� ������� ����������� �����.  a*x+b=y
  uint8_t YesNumWave; // ������ ������� ����� ����� �� ������� ������������� � �����������
  k_c =1.0;
  // 1. ��������� �� ����� ����� ������� ������
  if (Lambda < 800)
  {
    YesNumWave = SearchIndxNW(Lambda);
    if(YesNumWave)  IndxNumWave = YesNumWave;// �������� �����. CurrNumWave
    else
    {
      IndxNumWave = 6;
      Lambda = 1650;
    }
  }
  else if (Lambda < 900)
  {
    IndxNumWave =0;
  }
  else if (Lambda < 1210)
  {
    YesNumWave = SearchIndxNW(Lambda);
    if(YesNumWave)  IndxNumWave = YesNumWave;// �������� �����. CurrNumWave
    else // ��� ���� ���� � ��������� "900-1210" ���� ������������ ����� ���������
    {
      if(Lambda==905)
      {
      IndxNumWave =1;
      Lambda = 1210;
      }
      else
      {
      IndxNumWave =0;
      Lambda = 900;
      }
    }
  }
  else if (Lambda <= 1300)
  {
    IndxNumWave =1;
  }
  else if (Lambda <= 1310)
  {
    IndxNumWave =2;
  }
  else if (Lambda <= 1490)
  {
    IndxNumWave =3;
  }
  else if (Lambda <= 1550)
  {
    IndxNumWave =4;
  }
  else if (Lambda <= 1625)
  {
    IndxNumWave =5;
  }
  else if (Lambda <= 1650)
  {
    IndxNumWave =6;
  }
  else
  {
    YesNumWave = SearchIndxNW(Lambda);
    if(YesNumWave)  IndxNumWave = YesNumWave;// �������� �����. CurrNumWave
    else
    {
      IndxNumWave =0;
      Lambda = 800;
    }
  }
  // ����� ����� ������� ������
  // 2. ��� �� �����, ���� ���� ������������ �� ������������� � ���� ������ , �� ������������� 
  // ������ � ������� ������� ��������
  
  
  // ������� �������� � ���������, ���������� ����� ���� �� ������������� ��� �����������
  // �������� Mode
  if(Mode) // ������������� �� �������, ������������� � �������, ������������ �� ������������� ������ ����
  {
    if(Mode == 1) // increment
    {
      if(IndxNumWave<WAVE_LENGTHS_NUM-1) // �� 1625, �������� ����� ���� 
      {
        if(Lambda>=CalibrLW[IndxNumWave])IndxNumWave++;
        //IndxNumWave = (IndxNumWave)?(IndxNumWave):(IndxNumWave++);
        //if(IndxNumWave == 7) IndxNumWave=0; // ����������� ����� 1650, �������� �� 850
      }
      else // ��� 1650 ��� ���� �� �����������
      {
        YesNumWave=0;// ����� �������������
        for(i=IndxNumWave; i<(WAVE_LENGTHS_NUM+WAVE_LENGTHS_SPEC-1); i++)
        {
          if( SpecCoeffPM.PointSpecKalib[i-(WAVE_LENGTHS_NUM-1)])
          {
            YesNumWave = i+1;
            break;
          }
        }
        IndxNumWave = YesNumWave;
      }
    }
    // � �������� �������
    if(Mode == -1)
    {
            if((IndxNumWave>0) && (IndxNumWave<WAVE_LENGTHS_NUM)) //1...7
      {
        IndxNumWave--;
      }
      else
      {
        YesNumWave=0;
        for(i=(IndxNumWave)?(IndxNumWave-WAVE_LENGTHS_NUM):(WAVE_LENGTHS_SPEC); i>0; i--)//IndxNumWave;(WAVE_LENGTHS_NUM+WAVE_LENGTHS_SPEC)
        {
          if(SpecCoeffPM.PointSpecKalib[i-1])
          {
            YesNumWave = i;
            break;
          }
        }
        IndxNumWave = WAVE_LENGTHS_NUM + YesNumWave -1;
      }
    }
    // �������� ����� ��������� ��� �� ����� ����� � ��������� ����� ����������, ������� �� ������ ������������� ���� ���� � �������������
//    if(IndxNumWave<WAVE_LENGTHS_NUM) // ���� ����� ����� �� ��������� ������ ������������� ���� ����, �� �.�. ������ ��� ���� ������������ ��� ������
//      // �� ���� ����� ������������ ����������� � �������� �� ���������
//    {
//      Lambda = CalibrLW[IndxNumWave];
//      CurrBaseLevel = UserMeasConfig.BaseLvl[IndxNumWave];
//      k_c = CoeffPM.CoefPointKlb[IndxNumWave];
//    }
//    else
//    {
//      CurrBaseLevel = UserMeasConfig.BaseSpecKlb[IndxNumWave-WAVE_LENGTHS_NUM];
//      Lambda =  SpecCoeffPM.PointSpecKalib[IndxNumWave-WAVE_LENGTHS_NUM];
//      k_c =  SpecCoeffPM.CoefSpecKlb[IndxNumWave-WAVE_LENGTHS_NUM];
//    }
//    //  UserMeasConfig.BaseLevel = CurrBaseLevel; 
//    PMWavelenght = Lambda;
//    return k_c;  
    
    // �������� �� ������������� ����� �����, � ��������� ����� ����������, ����� ������ ������. �����. �� ����� ����� ��� ������
    if(IndxNumWave>=WAVE_LENGTHS_NUM) // ���� ����� ����� �� ���� ������ ������ ������������� ���� ����, �� �.�. ������ ��� ���� ������������ ��� ������
      // �� ���� ����� ������������ ����������� � �������� �� ���������
    {
      CurrBaseLevel = UserMeasConfig.BaseSpecKlb[IndxNumWave-WAVE_LENGTHS_NUM];
      Lambda =  SpecCoeffPM.PointSpecKalib[IndxNumWave-WAVE_LENGTHS_NUM];
      k_c =  SpecCoeffPM.CoefSpecKlb[IndxNumWave-WAVE_LENGTHS_NUM];
    PMWavelenght = Lambda;
    return k_c;  
    }
    //  UserMeasConfig.BaseLevel = CurrBaseLevel; 
    //��������������� �� ������ ���� ����������, ��������� ����� �����, ������� ������� 
      Lambda = CalibrLW[IndxNumWave];
      CurrBaseLevel = UserMeasConfig.BaseLvl[IndxNumWave];
      //k_c = CoeffPM.CoefPointKlb[IndxNumWave];
    PMWavelenght = Lambda;
  }
  // ����� ����������� ������ ��� ������� ���� ���������� ������������ �����������, � ���������� ������� �������
  // �������� LAmbda �� ����� ����� �� ���� ���� ����������, ���� �� �� ��������� CurrBaseLevel
  for(int j=0;j<WAVE_LENGTHS_NUM;j++)
  {
    if(Lambda == CalibrLW[j])
    {
            CurrBaseLevel = UserMeasConfig.BaseLvl[j];
    }
  }
  // ������ � ��������� ������������� ����� ��� ������ ����� �����
  if(Lambda<800) // ���� �������  ����� �����  ��� ��������� ���������
  {
    // ��������� ������ ����������� ���� ����
    YesNumWave = SearchIndxNW(Lambda);
    CurrBaseLevel = UserMeasConfig.BaseSpecKlb[YesNumWave-WAVE_LENGTHS_NUM];
    Lambda =  SpecCoeffPM.PointSpecKalib[YesNumWave-WAVE_LENGTHS_NUM];
    k_c =  SpecCoeffPM.CoefSpecKlb[YesNumWave-WAVE_LENGTHS_NUM];
    PMWavelenght = Lambda;
    return k_c;
  }
  
  // 800 ... 900 
  if(Lambda<=900)                      // ������ �������� L-�������� 800...900��
  {
    i=(int)((Lambda-800)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrL[i+1]-CoeffPM.CoefSpctrL[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrL[i]-a*(800+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*CoeffPM.CoefTune[0]; // for 850 nm
    PMWavelenght = Lambda;
    return k_c;
  }
  // 
  //�� ������������ �������� ��������� �� ����� 900 - 1210 
  //
  if(Lambda<1210)                     // 
  {
    // ��������� ������ ����������� ���� ����
    YesNumWave = SearchIndxNW(Lambda);
    CurrBaseLevel = UserMeasConfig.BaseSpecKlb[YesNumWave-WAVE_LENGTHS_NUM];
    Lambda =  SpecCoeffPM.PointSpecKalib[YesNumWave-WAVE_LENGTHS_NUM];
    k_c =  SpecCoeffPM.CoefSpecKlb[YesNumWave-WAVE_LENGTHS_NUM];
    PMWavelenght = Lambda;
    return k_c;
  }
  // 1210.... 1390
  if(Lambda<=1390)                     // ������ �������� M-�������� 1210...1390��
  {
    i=(int)((Lambda-1210)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrM[i+1]-CoeffPM.CoefSpctrM[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrM[i]-a*(1210+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*CoeffPM.CoefTune[1];
    PMWavelenght = Lambda;
    return k_c;
  }
  // 1390... 1410
  if(Lambda<1410)                       // "����" 1391...1409 ��
  {
    a = (CoeffPM.CoefSpctrH[0]*CoeffPM.CoefPointKlb[4]-CoeffPM.CoefSpctrM[9]*CoeffPM.CoefPointKlb[3])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrM[9]*CoeffPM.CoefPointKlb[3]-a*1390;                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    //k_c = k_c;	// #???
    k_c = k_c*CoeffPM.CoefTune[1];
    PMWavelenght = Lambda;
    
    return k_c;
  }  
  // 1410... 1650
  if(Lambda<=1650)                     // ������ �������� H-�������� 1410...1650��
  {
    i=(int)((Lambda-1410)/20);          // ��� ����� ������������� ������������� - 20 ��
    
    
    a = (CoeffPM.CoefSpctrH[i+1]-CoeffPM.CoefSpctrH[i])/20;   //a = (y2-y1)/(x2-x1)=(y2-y1)/20
    b = CoeffPM.CoefSpctrH[i]-a*(1410+i*20);                   //b = y1-a*x1 = y - a*(820��)
    
    k_c = a*Lambda+b;
    k_c = k_c*CoeffPM.CoefTune[2];
    PMWavelenght = Lambda;
    
    return k_c;
  }
  // 1650... 1700
  if(Lambda<=1700)                     // �� ������������ �������� ��������� �� ����� 1650 - 1700(1650*1.1) ������ �������������
  {
    // ��������� ������ ����������� ���� ����
    YesNumWave = SearchIndxNW(Lambda);
    CurrBaseLevel = UserMeasConfig.BaseSpecKlb[YesNumWave-WAVE_LENGTHS_NUM];
    Lambda =  SpecCoeffPM.PointSpecKalib[YesNumWave-WAVE_LENGTHS_NUM];
    k_c =  SpecCoeffPM.CoefSpecKlb[YesNumWave-WAVE_LENGTHS_NUM];
    PMWavelenght = Lambda;
    return k_c;
  }
  return k_c;
  
}

double GetPower(unsigned int Lambda)      // �������� �������� � ��� � ����������� �� ����� �����
{
  //  
  //  char M_PON;
  //  float clck_K;
  //  clkP++;
  
  int i;                          // �������
  int Res_Now;                    // ������� �������� ��� � ������ ��������
  static float Lev_x;                    // ����������� �������� ���: ������� �������� Level[0..39]
  static double Lev_W=1e-9;
  static int Res_Old=0;                 // ���������� ����������� �������� ��� � ������ ��������
  static int Level[SIZE_BUF];         // ����������� �������� ��� � ������ ��������
  static uint8_t OldRange=0;
  //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
   //   GPIOE->BSRRL = GPIO_PIN_5; // (PE5) set pin
  
  Res_Now = FreeCodeADC_PM - CoeffPM.ShZeroRng[Range];        // �������� ��������� � ������ ��������
  
  //  ������ ������!
  
  if (abs(Res_Old - Res_Now)>10000)                      // ����������� ������ ��� ������ ��������� ���������
  {
    for (i=0; i<SIZE_BUF; i++)
      Level[i]=Res_Now;    
  }
  
  for (i=SIZE_BUF-1; i>=1; i--)
    Level[i]=Level[i-1];
  Level[0] = Res_Now; 
  
  
  Lev_x = 0.;                                            // ������� ������� �������� ��������� ���
  for (i=0; i<SIZE_BUF; i++)Lev_x +=Level[i];
  Lev_x = Lev_x/SIZE_BUF;
  // ������ ����� �������, ������� ��������� ����� �������������
  Res_Old = (int)Lev_x;                                      // ��������� ��������, ��� "������"
  if (Lev_x<=0)     Lev_x = 1.0;                          // ���� ����� �������� ������ ��� ����� ����
  
  //Lev_W = ( 7.9261495712e-7 * Lev_x  * CoeffPM.CoefStykRange[Range] * GetCoeffSpctr(Lambda)) / pow( 100.0, (double)(3-Range) ); // ���������� ������ �������� � ���������� 
  Lev_W =  (double)(Lev_x);
  //Lev_W =  Lev_W * ADC_STEP ;
  Lev_W =  Lev_W * 5.0e-7 ; // ��� � �������������! ��� ����� ��������� �������� ���� ���
  Lev_W = Lev_W  * CoeffPM.CoefStykRange[Range];
  //Lev_W = Lev_W * GetCoeffSpctr(Lambda); // ��� �������� � ����� �����
  Lev_W = Lev_W  / pow( 100.0, (double)(3-Range)); // ���������� ������ �������� � ���������� 
  
  if (GetModePM())                          // �������������� ������������ ����������
  {

    // ������� ����������� ������������
    // ����� ������� � 270 �� ���� ������� ������� ������������ �� 6000000 = 5B8000
    // ����� ������� � 270 �� ���� ������� ������� ������������ �� 5000000 = 4C4B40 03.02.2016 �� �������� �� �������
    if ((Lev_x>=4150000) && (Range<3))                     //����� ������������ �� �� ����� ������ ���������, (����� 8000000 � ������� ��������)
    {
      Range++;                                          //������������� �� ����� �������������� ��������
      SetRange(Range);
      //CreatDelay (5000000);
    }
    if (Lev_x < 30000 && Range>0)                       //����� �������� ������� ����� �������� �� �� ����� �������������� �������,
    {
      Range--;                                          //������������� �� ����� �������������� ��������
      SetRange(Range);
      //CreatDelay (5000000);
    }
  }
  if (Range!=OldRange)
  {
  //    GPIOE->BSRRH = GPIO_PIN_5; // (PE5) reset pin
  //   HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
     OldRange=Range;
  }
  
  
  
  
  //    switch(GetCfgPM())
  //    {
  //    case 1:
  //      if (Lev_W<1e-9)Lev_W = 1e-9;                          //����������� ��������� �� ������ 1pW 
  //      break;
  //    case 2:
  //      if (Lev_W<1e-6)Lev_W = 1e-6;                          //����������� ��������� �� ������ 1pW 
  //      break;
  //    }
  
  return Lev_W;
}

// ��������� � ������ ������ �������� ������ ��� �������� ��� P1
void SetNewBaseLevel (void)
{
           // ����� ������� � ������ �����������
         uint8_t yLW = SearchIndxNW (PMWavelenght);
         if(yLW) 
         {
           SpecCoeffPM.PointSpecKalib[yLW-(WAVE_LENGTHS_NUM-1)]= CurrBaseLevel;
         }
         else  
           // ������ ����� ������������
         {
           for(int j=0;j<WAVE_LENGTHS_NUM;j++)
           {
             if(PMWavelenght == CalibrLW[j])
             {
               UserMeasConfig.BaseLvl[j] = CurrBaseLevel;
               break;
             }
           }
         }

  
}

// ��������� �������� ��������
//void SettingZeroPoints (void)
//{
//  int BufMed[20];
//  int Medium[4];
//  // Print Msg Go
//  sprintf((void*)Tx_Buff,"Go "); // // ��������� ��������� ��������; //
//  HAL_UART_Transmit_DMA(&huart3, (void*)Tx_Buff,strlen((void*)Tx_Buff)); // ������ ����� ���������
//  TransmitU = DATA_TX;
//  // set MANUAL Mode
//  SetModePM(0);
//  SetRange(0);
//  osDelay(3000);
//  for (int i=0; i<4; ++i)
//  {
//    // set New Range
//    SetRange(i);
//    osDelay(1500);
//    memset(&BufMed, 0, sizeof(BufMed));  
//    Medium[i]=0;
//    
//    for (int j=0; j<20; ++j)
//    {
//      BufMed[i] = GetPMDataNorm();
//      Medium[i] += BufMed[i];
//      //      sprintf((void*)Tx_Buff,"%d\n",BufMed[i]); // // ��������� ��������� ��������; //
//      //      HAL_UART_Transmit_DMA(&huart3, (void*)Tx_Buff,strlen((void*)Tx_Buff)); // ������ ����� ���������
//      //      TransmitU = DATA_TX;
//      osDelay(500);
//    }
//    // get Level on Current Range
//    CoeffPM.ShZeroRng[i]=Medium[i]/20;
//    // Print Level on LowRange
//    //      sprintf((void*)Tx_Buff,"%d,k=%d\n ",Medium[i],CoeffPM.ShZeroRng[i]); // // ��������� ��������� ��������; //
//    sprintf((void*)Tx_Buff,"%d, ",CoeffPM.ShZeroRng[i]); // // ��������� ��������� ��������; //
//    HAL_UART_Transmit_DMA(&huart3, (void*)Tx_Buff,strlen((void*)Tx_Buff)); // ������ ����� ���������
//    TransmitU = DATA_TX;
//    
//  }
//  // set AUTO Mode
//  SetModePM(1);
//  // Save Setting
//  EEPROM_write(&CoeffPM, ADR_CoeffPM, sizeof(CoeffPM));
//  
//  osDelay(50);
//  // Print Msg End
//  sprintf((void*)Tx_Buff,"End\r"); // // ��������� ��������� ��������; //
//  HAL_UART_Transmit_DMA(&huart3, (void*)Tx_Buff,strlen((void*)Tx_Buff)); // ������ ����� ���������
//  TransmitU = DATA_TX;
//}
//
//
void InitTableCoeff (void) // ������������� ������������� ��������� ����������
{
  int i;
  for (i=0; i<4;i++)
  {
    CoeffPM.ShZeroRng[i] = 0;
    CoeffPM.CoefStykRange[i] = 1.0;
  }
  for (i=0; i<6;i++)
  {
    CoeffPM.CoefSpctrL[i] = 1.0;
  }
  for (i=0; i<10;i++)
  {
    CoeffPM.CoefSpctrM[i] = 1.0;
  }
  for (i=0; i<13;i++)
  {
    CoeffPM.CoefSpctrH[i] = 1.0;
  }
  for (i=0; i<WAVE_LENGTHS_NUM;i++)
  {
    CoeffPM.CoefPointKlb[i] = 1.0;
  }

}

        // ������� �������� � ������ ��� �2
void CalkP2Mode (void)
{
      // ������� �������� � ������ ��� �2
      P2SUM=0;
      P2SUMmW=0;
      
      for (int n=0;n<ADCSIZE-1;n++)
      {
        BuffP2[n] = BuffP2[n+1];
        P2SUM +=BuffP2[n];
      }
      BuffP2[ADCSIZE-1]=IN_BuffADC[1];       //au?aneaiiia a aAi , aey aaeuiaeoae ia?aaioee
      P2SUM +=BuffP2[ADCSIZE-1];
      MediumP2 = (float)P2SUM/ADCSIZE; // ������� �������� ����� ���
      if(MediumP2<4096.0)// ��������� �������� ��� ��������� � ������!
      {
        // ����������� ������ � ����� �������� ��� ��������� �� �.�. (09.08.2022)
        if(MediumP2>P2UP_Lim)
        {
          MediumP2_Ind = MediumP2_Ind + (MediumP2 - P2UP_Lim);
          P2UP_Lim = MediumP2;
          P2DN_Lim = P2UP_Lim - DELTAP2; 
        }
        else if (MediumP2<P2DN_Lim)
        {
          MediumP2_Ind = MediumP2_Ind - (P2DN_Lim - MediumP2 );
          P2DN_Lim = MediumP2;
          P2UP_Lim = P2DN_Lim  + DELTAP2; 
        }
        // �������� �2 � ��� ��� ��������� ������ ����� ������ �� ������ ��������� � 
        P2dBmValue = (MediumP2_Ind - (float)DeviceConfig.SmPx[IndxP2LW])*DeviceConfig.Kprop;
        P2dBValue =  P2dBmValue - UserMeasConfig.BaseLvlP2[IndxP2LW];
      }
      //
      TestCntWait++; // ��������� �������� ��������� �2
      // ��������� �������� ORL (� ��������� � ����� ����� ORL (����� ��� ��������� � ������ �������)
      PowP2_mW = pow(10.0,(P2dBmValue/10.0));
      //calk medium P2 in mW
            for (int n=0;n<ADCSIZE-1;n++)
      {
        BuffP2mW[n] = BuffP2mW[n+1];
        P2SUMmW +=BuffP2mW[n];
      }
      BuffP2mW[ADCSIZE-1]=PowP2_mW;       //� ��������� ����� ��� ������ ��� ���������
      P2SUMmW +=BuffP2mW[ADCSIZE-1];
      MedP2mW = (float)P2SUMmW/ADCSIZE;
      // ������������ �������
      //PowP2_mW = MedP2mW;
      if ((MedP2mW - PowClbr_mW)>0)
        MedP2mW_corr = MedP2mW - PowClbr_mW;
      else
        MedP2mW_corr = 0.00000001; // 10pW)
        
      // ������ ����� ��������� ��������� �� ��������
      //if(MedP2mW<0.001)// ��������� �������� ��� ��������� � ������!(<1����)
      //{
        if((MedP2mW_Ind>MedP2mW+DELTAP2mW)||(MedP2mW_Ind<MedP2mW-DELTAP2mW))
          MedP2mW_Ind = MedP2mW;
        // ����������� ������ � ����� �������� ��� ��������� �� �.�. (09.08.2022)
        if(MedP2mW>P2UPmW_Lim)
        {
          MedP2mW_Ind = MedP2mW_Ind + (MedP2mW - P2UPmW_Lim);
          P2UPmW_Lim = MedP2mW;
          P2DNmW_Lim = P2UPmW_Lim - DELTAP2mW; 
        }
        else if (MedP2mW<P2DNmW_Lim)
        {
          MedP2mW_Ind = MedP2mW_Ind - (P2DNmW_Lim - MedP2mW );
          P2DNmW_Lim = MedP2mW;
          P2UPmW_Lim = P2DNmW_Lim  + DELTAP2mW; 
        }
        // �������� �2 � ��� ��� ��������� ������ ����� ������ �� ������ ��������� � 
        //P2dBmValue = (MediumP2_Ind - (float)DeviceConfig.SmPx[IndxP2LW])*DeviceConfig.Kprop;
        //P2dBValue =  P2dBmValue - UserMeasConfig.BaseLvlP2[IndxP2LW];
        //  PowP2_mW = MedP2mW_Ind;
      //}
//      // ������ ������� ��
//      ORLCalcFree = CoeffPM.BaseRet[indx_LW_ORL]-(10*log10(PowP2_mW - PowClbr_mW)); // ����������
//      ORLCalc = ORL_LVL_P; // ����������� ���� �������
//      if ((PowP2_mW - PowClbr_mW)>0)
//      {
//        ORLCalc = CoeffPM.BaseRet[indx_LW_ORL]-(10*log10(PowP2_mW - PowClbr_mW)); // ����������
//        // ���� ����� ����������� ��� �� �� ��� � �������� ����� 60 ����� ������� �� 60
//        if(ORLCalc >= ORL_LVL_P) 
//          ORLCalc = ORL_LVL_P; // ����������� ���� �������
//      }
      // ������ ������� ��
      //ORLCalcFree = CoeffPM.BaseRet[indx_LW_ORL]-(10*log10(PowP2_mW)); // ����������
      ORLCalcFree = BaseRetCurrORL[indx_LW_ORL]-(10*log10(PowP2_mW)); // ����������
      ORLCalc = ORL_LVL_P; // ����������� ���� �������
      if ((MedP2mW_Ind - PowClbr_mW)>0)
      {
        //ORLCalc = CoeffPM.BaseRet[indx_LW_ORL]-(10*log10(MedP2mW_Ind - PowClbr_mW)); // ����������
        ORLCalc = BaseRetCurrORL[indx_LW_ORL]-(10*log10(MedP2mW_Ind - PowClbr_mW)); // ����������
        // ���� ����� ����������� ��� �� �� ��� � �������� ����� 60 ����� ������� �� 60
        if(ORLCalc >= ORL_LVL_P) 
          ORLCalc = ORL_LVL_P; // ����������� ���� �������
      }
        //ORLCalcCorr = CoeffPM.BaseRet[indx_LW_ORL]-(10*log10(MedP2mW_corr)); // ������� ��������
        ORLCalcCorr = BaseRetCurrORL[indx_LW_ORL]-(10*log10(MedP2mW_corr)); // ������� ��������

}
//void InitScreenPM (void) // ������������� ��������� ���������� ����������
//{
//  for (int i =0;i<LSPLACENUM;++i)
//  {
//    g_ScreenPM[i].iStateCH = S_FREE; // ������� ��� �� ���� �������� �� �����
//    g_ScreenPM[i].iLW_CH = 0;        // ������������ ����� �����
//    g_ScreenPM[i].fRef_CH = 0.;      // ����� �� ������ ���������� � ����������� �� ������ ��� ������ ����� �����
//    g_ScreenPM[i].fRef_CH_FA = 0.;      // ����� �� ������ ���������� � ����������� �� ������ ��� ������ ����� ����� jn bcnjxybrf
//    g_ScreenPM[i].fValuePM_CH = 0.;  // �������� �������� � ���������, ��� ������� � ��
//    g_ScreenPM[i].fValueDBM_CH = -100.;  // �������� � ���
//    //
//    g_RecAutoPM[i].fValuePM_CH =0.; // �������� ��������, ��� ������ ����� �����
//    g_RecAutoPM[i].iCountStateCH = TIME_MAXOFF; // ����� �� ����������� ���� �������� ����� ����� (old 100)
//    g_RecAutoPM[i].iLW_CH = 0; // ��� ����� �����, ������� ������� � ��������� �����
//    g_RecAutoPM[i].fPowScFA_CH = 0.;  // �������� �������� ��������� ��� ������ � ������ ������ ����
//
//  }
//}
//
//void SetLasersAsMode(void)
//{
//  static uint8_t SetActivLS = 1;
//  static int16_t iIndxLSCurr;
//  static int16_t Oldg_iActivePlace = 0;
//  
//  if(GetMultipageMode() == MPAGE_TESTER_MODE)  // ������� � ������ �������
//  {
//    if(g_iActivePlace>=0)
//    {
//      if(SetActivLS == 0) // ������ �������� ����� (���� �������������)
//      {
//        SetActivLS = 1;
//        SetModeLSDefault (1);
//        //g_CountRepetCWADD=0;
//        // ����� �������  ��� ����� ������� �����
//        SetOnLaserPlace (g_iActivePlace); //��������� ������ �� �����
//        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
//        Oldg_iActivePlace = g_iActivePlace;
//      }
//      if(Oldg_iActivePlace != g_iActivePlace) // ���������� ����� ����� ���������
//      {
//        SetOnLaserPlace (g_iActivePlace); //��������� ������ �� �����
//        Oldg_iActivePlace = g_iActivePlace;
//      }
//      // �������� ������ � ��������� ������
//      // ���� ���� ����� CW* �� ���������� �������� �������� �����
//      if(g_aPlaceMode[g_iActivePlace] == 2) //2=LS_MODE_CW_ADD
//      {
//        if(g_CountRepetCWADD>TIME_CHNG_SC) // 100- �� ������ ������ � �������� , ���������� 3+ ��� (100) ������� ���� ������
//          // ���� � ������� 0.4*8��� = 2.8S = 0.15*19 
//        {
//          g_CountRepetCWADD = 0; // ���� ������� ����� ������ ����� � ����������� �����
//          for(iIndxLSCurr=g_iActivePlace+1; iIndxLSCurr < g_iActivePlace+5; ++iIndxLSCurr)
//          {
//            if(g_aPlaceMode[iIndxLSCurr&0x3]==2)//LS_MODE_CW_ADD
//            {
//              g_iActivePlace = iIndxLSCurr&0x3;
//              UpdateLabels();  // ����� ����������� ���� �������� �����     
//              break;
//            }
//          }
//          // ����� �������  ��� ����� ������� �����
//          SetOnLaserPlace (g_iActivePlace); //��������� ������ �� �����
//          osDelay(100);
//        //}
//        //if(g_CountRepetCWADD==1) // Send optic data
//        //{
//          // Send LW to RS_Optics
//          //sprintf((char*)Tx_BuffOptic, "########SL%04d:%04d\r\xffUUUUU",g_ConfigDevice.PlaceLS[g_iActivePlace], 9999);
//          //sprintf((char*)Tx_BuffOptic, "########SL%04d %.2f\r\xffUUUUU",g_ConfigDevice.PlaceLS[g_iActivePlace], g_SetLS_InOutParam.fPW_outLS[g_iActivePlace]);
//          // ��������� ��� � ��������� ��������
//          sprintf((char*)Tx_BuffOptic, "UUUU####SL%04d %.2f\r\xffUUU",g_ConfigDevice.PlaceLS[g_iActivePlace], g_SetLS_InOutParam.fPW_outLS[g_iActivePlace]);
//          //CntRXByte = 0;
//          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
//                   osDelay(20); // ��� ���� �������� � ������� ��� ���� �����
//          HAL_UART_Transmit_DMA(&huart6, Tx_BuffOptic, strlen((char*)Tx_BuffOptic)); // ������ ����� ���������
//          TransmitUOpt = DATA_TX;
//        }
//        
//      }
//      g_StatLSOut =  g_ConfigDevice.PlaceLS[g_iActivePlace] + 0x8000;
//    }
//    else // 
//    {
//      if(SetActivLS == 1) // ������ ��������� ����� (���� �������������)
//      {
//        SetActivLS = 0;
//        SetModeLSDefault (1);
//        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);
//      }
//      g_StatLSOut = g_ConfigDevice.PlaceLS[Oldg_iActivePlace];
//    }
//    //CntrlExpUnit (OFF, EN_CH1); // ���������� laser ch1
//    //CntrlExpUnit (OFF, EN_CH2); // ���������� laser ch2
//    //g_aPlaceMode[] // ��������� �������
//    //  g_iActivePlace = -1; // ���� �������� �����
//  }
//  
//}
//
//void SetOnLaserPlace (int Mode) //��������� ������ �� �����
//{
//  switch(Mode)
//  {
//  case 0:
//    CntrlExpUnit (ON, EN_CH1); // ��������� laser ch1
//    CntrlExpUnit (OFF, EN_CH2); // ���������� laser ch2
//    CntrlExpUnit (OFF, EN_CH3); // ���������� laser ch3
//    CntrlExpUnit (OFF, EN_CH4); // ���������� laser ch4
//    break;
//  case 1:
//    CntrlExpUnit (OFF, EN_CH1); // ��������� laser ch1
//    CntrlExpUnit (ON, EN_CH2); // ��������� laser ch2
//    CntrlExpUnit (OFF, EN_CH3); // ���������� laser ch3
//    CntrlExpUnit (OFF, EN_CH4); // ���������� laser ch4
//    break;
//  case 2:
//    CntrlExpUnit (OFF, EN_CH1); // ��������� laser ch1
//    CntrlExpUnit (OFF, EN_CH2); // ���������� laser ch2
//    CntrlExpUnit (ON, EN_CH3); // ��������� laser ch3
//    CntrlExpUnit (OFF, EN_CH4); // ���������� laser ch4
//    break;
//  case 3:
//    CntrlExpUnit (OFF, EN_CH1); // ���������� laser ch1
//    CntrlExpUnit (OFF, EN_CH2); // ���������� laser ch2
//    CntrlExpUnit (OFF, EN_CH3); // ���������� laser ch3
//    CntrlExpUnit (ON, EN_CH4); // ��������� laser ch4
//    break;
//  default:
//    CntrlExpUnit (OFF, EN_CH1); // ���������� laser ch1
//    CntrlExpUnit (OFF, EN_CH2); // ���������� laser ch2
//    CntrlExpUnit (OFF, EN_CH3); // ���������� laser ch3
//    CntrlExpUnit (OFF, EN_CH4); // ���������� laser ch4
//    break;
//  }
//  
//}
//
//uint16_t SetUartCmdLS (uint8_t Mode, uint8_t Channel)
//{
//  uint16_t Status = 0;
//  for (int i =0;i<LSPLACENUM;++i) // ��� ���������
//  {
//    g_aPlaceMode[i] = 0;
//    g_iActivePlace = -1;
//    //g_aPlaceMode[g_iActivePlace]
//  }
//  if (Mode)
//  {
//    Status = g_ConfigDevice.PlaceLS[Channel-1];
//    if(Status) // ���� �� �����        
//    {
//      g_iActivePlace = Channel-1;
//      g_aPlaceMode[g_iActivePlace] = 1;
//    }
//    
//  }
//  SetMultipageMode(MPAGE_TESTER_MODE, 0); // �������� ����� ���������
//  //UpdateLabels();  // ����� ����������� ���� �������� �����     
//  return Status;
//}