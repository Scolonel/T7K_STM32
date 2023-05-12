// ������� ��������� ������ AT
// ����� ������� �� ������������ UART ����������
// ��������� ��������� ������� �� OpticUART
#include "main.h"

// ��������� ����� ��������� � ������� � ����� 50 ��
// SavPw
#define tsSavPw 5     // �������� ������� Slave
#define tseSavPw 10    // ����� ��������� ��������� 20
#define tsdSavPw 60    // ����� ���������� ��������� 60
#define teSavPw 100     // ���������� ����� �������� ������� 100
// LnR_Clb
#define tsLnRClb 6     // �������� ������� Slave
#define tziLnRClb 40    // ����� �������� ������������ �������� ��������
#define teLnRClb 70     // ����� ������ �� ������
// CalPw - ������ Slave �� ������ ��������
#define tsCalPw 48     // ����� ��������� �������� �� ������� 50
#define tansCalPw 60     // ����� ������ ������� 60
// LnX
#define tsLnX 30     // ����� �������� � ����� �������� �������� �� �������
#define teLnX 40     // ����� ������ �� ������ 
// ORL
#define tsORL 5     // ����� ������ ������� Slave �� �������
#define tseLsORL 20    // ����� ��������� ���������
#define tsdLsORL 60    // ����� ���������� ���������
#define teORL 100     // ���������� ����� �������� �������
// CmdSw
#define tsCmdSw 1     // ����� ������ ������� Slave �� �������
#define tsoCmdSw 5    // ����� ��������� ������ ���������
#define teCmdSw 20     // ���������� ����� �������� �������
// AnsOrl
#define tnAnsOrl 10    // ����� ����� �������� ORL �� Slave d wbrkt

// ������������ �������� ��� ���������� �����
//#define xImp 25


const char* Msg[]={"    ��������    ", // 0
                   "     ������     ", // 1
                   "   ����������   ", // 2
                   "    ���������   ", // 3
                   "     ������     ", // 4
                   "   ����������   ", // 5
                   "   ����������   ", // 6
                   "     �����      ", // 7
                   "   ���������    ", // 8
                   "     �����      ", // 9
                   "                ", // 10 ������ ������
                   "    ��������    ", // 11 
                   "     �����      ", // 12 
                   "   ���������    ", // 13 
                   "    ��������    ", // 14 
                   "   ��� ��\xff��     ", //15
                   "     �������    ", //16
                   "    ���������   ", //17
                   "    ��������    ", //18
                   "   ���.�����.   ", //19
                   "-�-�-�-�-�-�-�-�"};

int Mod_At_cnt; // ������� ��������������� ������ ����� ��� ������
static uint16_t tSt_Cnt=0;
static uint8_t En_TX_Cmd;// ���������� �������� ������� �� ������ � ������ ��������, �������� �� OpticUART
uint8_t MsgStat[18]; // ��������� ������� ������ ������� ����-��������
uint8_t MsgAuto[3][20];
//���������� ��������������� ������
uint8_t FirstInput; // ���������� ������� ����� �� ������� � ������ ��������� ( ��� �������� ������������� ���������)
uint8_t Start_Cmd; // ����� ����� ���������� ������� �� �������� ��������� ��� ������ ������� ���� �����
uint8_t Inv=0; // aaeo oeacaiey eiaa?nee i?e eiaeeaoee PON
uint8_t Inv_Bas=0; // aaeo oeacaiey eiaa?nee i?e eiaeeaoee i?aaaeia PON
uint8_t Cnt_SPw; // ������� ���������� � ����� ������������ ������
uint8_t Cnt_Izm; // ������� ��������� ���������� ��������������� �������� � ��� � (������� �� ����� ����������)
uint8_t Cnt_BA; // ������� ��������� ���������� ��������������� �������� � ��� � (������� �� ����� ����������)
uint8_t Err_Cmd_At; // ������� ������ ��� �������� � ������ At� ������ ������� ��� ������������ ����������
uint8_t NumChoiceLS; // ����� ����������� ������� � �������� (����� �� 1 ������)(������� ���� ���)
uint8_t Uni_At=0; // ������ ����������� ����������
volatile uint8_t  NeedTransAnsver=0; // ������������� ������ Slave �� �������� ������� �� �������
uint8_t DisRecImp; // ������ ������ ���� �������� ������� (����� �� 3-5 ��) (����� ��������� ������)
uint8_t IndxValidLN; // ������ ������������������ �������� ������ ��������� � ������� ������ ������ ���������� �����
uint32_t WaitCnt;
uint32_t TestCntWait;// �������� ������� ��������� (� ������ ������ P2) 

volatile uint32_t WaitCntLnX=0; // ������� �� ��� �������� �������� ������ ������� �� Slave � ������ LnX
// ���������� ������������ ������ (������� �����       NeedTransAnsver=1; // ������������� ������ Slave �� �������� ������� �� �������

volatile uint32_t Beg_Evnt; // 
volatile uint32_t End_Evnt; // 
volatile uint32_t Time_Evnt; // 

uint32_t Summx=0; // ���������� ��� ������� �������� ��������� ����� � ���������� � �����
uint32_t cntr;

//
char RX1_DAT;
uint8_t NeedReadPWSlave;
uint16_t LWSlaveSRC[3]; // ����� ����� ��������� Slave � ������ �������

float Pow_SC_Slave; // �������� ��������� Slave � ������ �������
float Pow_SC_Tx; // �������� ���������� ��� �������� � ������ CalPw

TAB_PONAUTO PonA; // ���������  ������ PON AUTO


uint16_t CalibLnAuto; // �������� ����� ����� ���������� � ������ �������!
uint8_t StrAnsverData[32]; // ������ �������� ��� �������� ������ � �������� �������
int Indxi; //��������������� ������ ��� ���������� �������� 


// ����� ������� ������������� ����� ����� �� �� ��������
int iClbFromLW (uint16_t LWi)
{
  int i=0;
    for (i=0;i<WAVE_LENGTHS_NUM;i++)
  {
    if(LWi == CalibrLW[i]) return i;
  }
  return i;
}

void Ini_At_var(void) // ������������� ���������� �� ������
{
    for (int i=0;i<18;i++)
  {
    MsgStat[i]='-';
  }
      MsgStat[15]=0;// ��������� ������

  for (int i=0;i<=WAVE_LENGTHS_NUM;i++)
  {
    PonA.PW_AB[i]=-100.0;
    PonA.PW_BA[i]=-100.0;
    PonA.PS_BA[i]=-100.0;
    PonA.LS_AB[i]=-100.0; // 
    PonA.LS_BA[i]=-100.0;
    PonA.ORL_AB[i]=-100.0; // 
    PonA.ORL_BA[i]=-100.0;
    if(i<3)
    {
    PonA.LW_BA[i]=0;
    PonA.LW_AB[i]=DeviceConfig.PlaceLS[i];
    }
    //LNCalc = 0;
  }
  PonA.LnA=0;
}



// ������ ����� �������� �������
void GetNumActivLS(void)
{
  uint8_t Num=0;
  for(int i=0;i<3;++i)
  {
    if(DeviceConfig.CfgOUT[i]) Num++;
  }
  NumChoiceLS = Num;
}
void CheckTimerAT (void)// �������� ������� � ������ ������� �� �������� (
{
    if (Mod_At_cnt < MaxAT)
  { 
    Mod_At_cnt++;
    if (Mod_At_cnt == Start_Cmd)
      En_TX_Cmd = 1;
  }
}

void Run_At (void)
{
  switch (Mod_At)
  {
    //________________________________ ��������
  case Wt:
    if(FirstInput) // ���� ���������������� �����
    {
      memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // ��������
      memcpy(MsgAuto[1],Msg[1],strlen(Msg[1])); // ������
      FirstInput=0; 
      NeedReDraw = 1;
    }
    else
    { 
      if(OpticCMD)
      {
        switch(OpticCMD)
        {
          //-----------------
        case SMPcmd: //� ������ �������� ������ ��������� �������� �� �������
          Mod_At = CalPw; // ������������ � ����� 
          memcpy(MsgAuto[0],Msg[2],strlen(Msg[2])); // ���������� (����� �� ���������� SLAVE)
          memcpy(MsgAuto[1],Msg[10],strlen(Msg[10]));
          PMWavelenght = iParamOpt; // � ������ ������ ���������� ����� ����� ������������� ��������� �� �������
          IndxP2LW = GetIndxLW(PMWavelenght); // ������� ������ ����� ����� ��� ����������� ��������� ������ � P2
          Pow_SC_Tx = 0.0; // �������� ��������� ��������� �� ���������
          Mod_At_cnt = 0; // ����� �������� ������� �������
          Err_Cmd_At = 0; //����� ������ ������ � ������  At
          En_TX_Cmd = 0;// ����� ������ �������
          Start_Cmd = tansCalPw; // 
          NeedReDraw = 1;
          TestCntWait=0;
          CntrlA(0); // 
          
          break;
          //---------------------
        case SWLcmd: //� ������ �������� ������ ��������� �����
          Mod_At = Lnx;
          SW5V(1);// ON 5V
          memcpy(MsgAuto[0],Msg[11],strlen(Msg[11])); // ���������� �����
          memcpy(MsgAuto[1],Msg[12],strlen(Msg[12]));
          Start_Cmd = MaxAT+2; // 
          Mod_At_cnt = 0; // ����� �������� ������� �������
          Err_Cmd_At = 0; //����� ������ ������ � ������  At
          En_TX_Cmd = 0;
          NeedReDraw = 1;
          //SW5V(1);// ON 5V
          break;
          //-----------------
        case SLAcmd: //� ������ �������� ������ ��������� �������� �� ������� � �������� ����� ���� ���������,
          // ����� ��������� ���������� (����� ������ ��������� ���������� ���������
          Mod_At = Px; // ������������ � ����� Px (���������� CalPW �� ����������)
          memcpy(MsgAuto[0],Msg[11],strlen(Msg[11])); // ���������� (����� �� ���������� SLAVE)
          memcpy(MsgAuto[1],Msg[14],strlen(Msg[14]));
          PMWavelenght = iParamOpt; // � ������ ������ ���������� ����� ����� ������������� ��������� �� �������
          // ������ �������� ������������ �������� � �������� �� �����, ����� ��������� �� ������� �������
          IndxP2LW = GetIndxLW(PMWavelenght); // ������� ������ ����� ����� ��� ����������� ��������� ������ � P2
          Pow_SC_Tx = 0.0; // �������� ��������� ��������� �� ���������
          Mod_At_cnt = 0; // ����� �������� ������� �������
          Err_Cmd_At = 0; //����� ������ ������ � ������  At
          En_TX_Cmd = 0;// ����� ������ �������
          Start_Cmd = tansCalPw; // 
          NeedReDraw = 1;
          break;
          //-----------------
        case SWMcmd: //� ������ �������� ������������ Slave � 
          //������������ ������� ������������� ������������ SOK
          Mod_At_cnt = 0;
          memcpy(MsgAuto[0],Msg[16],strlen(Msg[16])); // �������
          memcpy(MsgAuto[1],Msg[10],strlen(Msg[10]));
          Mod_At = AnsOK; // ������������ � ����� AnsOK
          Start_Cmd = tsCmdSw; // ����� ����� �������� � ������������� ����� �������� ������ SLA, ��� ��������� �������� ��������� �� ������� �
          lSofn = 0; //
          En_TX_Cmd = 0;// ����� ������ �������
          NeedReDraw = 1;
          break;
          //-------------------
        case SRLcmd: //� ������ ��������  Slave � 
          //������������ ������� �� ���������� ORL
          Mod_At_cnt = 0;
          memcpy(MsgAuto[0],Msg[18],strlen(Msg[18])); // ��������
          memcpy(MsgAuto[1],Msg[19],strlen(Msg[19])); // ���.�����.
          Mod_At = AnsOrl; // ������������ � ����� AnsOrl
          GetSetLW_SC (0, 1); //��������� ������ ����� ������� ������� 0-������,1-� ������ �����������
          Start_Cmd = tsCmdSw; // ����� ����� �������� � ������������� ����� �������� 
          lSofn = 0; //
          Cnt_SPw = 0; // ������� ������� ���� ������� ���� ��������
          En_TX_Cmd = 0;// ����� ������ �������
          NeedReDraw = 1;
          break;
          //-------------------
          
        default:
          break;
        }
        OpticCMD = NOTcmd; // ����� ������ �������
        
      }
      
    }
    break; // Wt (��������)
    //________________________________ ���������� ��������� � ����� �����
  case SavPw: // ���������� ��������� (������) ���� �������
    if(FirstInput) // ���� ���������������� �����
    {
      memcpy(MsgAuto[0],Msg[2],strlen(Msg[2])); // ���������� ���������
      memcpy(MsgAuto[1],Msg[3],strlen(Msg[3]));
      Cnt_SPw = 0; // ����� ������� �������� �������
      Pow_SC_Tx = 0.0; // �������� ��������� ��������� �� ���������
      Err_Cmd_At=0; //����� ������ ������ � ������  At
      Start_Cmd = tsSavPw; // ������ ��� ����� � ������� �������� ������� 0.25 ���
      Mod_At_cnt=0; // ����� �������� ������� �������
      GetSetLW_SC (0, 1); //��������� ������ ����� ������� ������� 0-������,1-� ������ �����������
      FirstInput=0; 
      tSt_Cnt=0;
    }
    else // ��� ����� , ���� �������� ����� ����� ���������, � ����������� ���������� ����������,
    {
      // 1. ���� �������� ����� ����� ������� ���������, 
      // 2. ����� ����������� � ���������� ���������
      // 3. ��������� �� OpticUART ������ ������ � ��������� ���������
      // 4. ��������� ��� ������ ���� ����
      // 5. ��������� �������� ��������� ����� (������������ ������� �������)
      // 6. ��������� ������ �� �������, ��������� ����� ������� ����
      //while((DeviceConfig.PlaceLS==0)(
      if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // ��������� ���������� ������� � �� ��� ��������� �� ��������
      {   
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
        //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 

        HAL_UART_DMAStop(&huart2);
        
        // ����� ����������, ����� �������� �������
        sprintf((char*)StrTXOpt, "####SMP%4u\rUUU",DeviceConfig.PlaceLS[Ind_LW]);//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
        TxOptBusy = 1;
        //TsT(1);
        Mod_At_cnt++; // +50��
        tSt_Cnt++;
        En_TX_Cmd = 0;
        lSofn = 1; //
        PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
        NeedReDraw = 1;
        
      }
      else 
      {
        if ((Mod_At_cnt > tsdSavPw+10) && (Cnt_SPw == NumChoiceLS-1)) // ���
          // ���� ������������� �����...
          //myBeep(100);
          ;
      }
      // ������������ ������� ��� ��������� ����������� ��������� (
      if(TxOptBusy==0)
      {
        // ����� ��������� ���������
        // ����� ���������� ���������
        if((Mod_At_cnt>tseSavPw)&&(Mod_At_cnt<tsdSavPw))
        {
          CW_S(1);
          //NeedReDraw = 1;
        }
        else
        {
          CW_S(0);
          lSofn = 0; //
          //NeedReDraw = 1;
        }
      }
    }
    // �������� ���� �� ����� �� Slave , ���� ���� �� ������� ��������� � ������� ����� ����
    if(OpticCMD == YPcmd)
    {
      SettingPrm.PowSc[Ind_LW] = fParamOpt;// ���������� �������� ������ ���������
      OpticCMD = NOTcmd;
      Cnt_SPw++;
      GetSetLW_SC (1, 1); //��������� ��������� ����� ������� ������� 0-������,1-� ������ �����������
      Mod_At_cnt=0; // ����� �������� ������� �������
      NeedSaveChange = 0x10; // �������� �������� ��������� ������ ����������� �� Slave
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
      NeedReDraw = 1;
      if (Cnt_SPw == NumChoiceLS) // ��� �������� � �������
      {
        //myBeep(200); // ����� �������� �����! (���������������!!!)
        ControlSC (0); // ���������� ���������� -  ���������
        Mod_At = LnR_Clb; // ������������ � ����� � ��������� ������� ������� �����
        //Err_Cmd_At=0; // ����� ������ ������ � ������  At
        //Start_Cmd = 6; // ������ ��� ����� � ������� �������� ������� 0.25 ���
        //Mod_At_cnt=0; // ����� �������� ������� �������
        // ������������� ��������
        TIM2->CNT = 0; // �������� ������ ��������� ����� (���� ������� 25 �����)
        TIM3->CNT = 0;// ������� �������� �������� ���������
        TIM2->PSC = 0; // 0- 72MHz
        TIM3->PSC = 0;
        
        En_TX_Cmd = 0;
        FirstInput=1; 
        CntLnCount = 0; // ����� �������� ������ ��������� �����
        //memset(DataLN,0, ARRAY_SIZE(DataLN)); // ������ ������ ������ ������
        memset(DataLN,0, NUMMEASLN*SEQMEASLN*2); // ������ ������ ������ ������
        
      }
      
    }
    // ���������� ����� �������� �������
    if (Mod_At_cnt>teSavPw) // ��� ������ � �������� ��������� 5 sec
    {
      Mod_At_cnt = MaxAT;
      Mod_At = Wt;
      OpticCMD=NOTcmd; //05.05.2023 ������� �������� ������� ���� �� ����... ��� �������� �� Slave
      memcpy(MsgAuto[0],Msg[4],strlen(Msg[4])); // ������
      memcpy(MsgAuto[1],Msg[5],strlen(Msg[5])); // ����������
      NeedReDraw = 1;
      
      //Start_Cmd = 0;
      //sprintf(Str4, "    ������    ");
      //sprintf(Str5, "  ����������  ");
    }
    
    break; // SavPw
    //_____________________->>> LnR_Clb ���������� ����� ������. � �������� ����� ��������� �����
  case LnR_Clb:
  case LnR:
    if(FirstInput)
    {
      if(Mod_At == LnR_Clb)
      {
        memcpy(MsgAuto[0],Msg[6],strlen(Msg[6])); // ���������� �����
        memcpy(MsgAuto[1],Msg[7],strlen(Msg[7]));
      }
      SW5V(1);// ON 5V
      Err_Cmd_At=0; //����� ������ ������ � ������  At
      // �������� ������� Slave
      Start_Cmd = tsLnRClb; // ������ ��� ����� � ������� �������� ������� ��� ������������ Slave � ����� ��������� �����
      Mod_At_cnt=0; // ����� �������� ������� �������
      Ind_LW = 1; //��������� ����� ������� ������� - ������ �����!!!
      // �������� ���� ��������� ��������
      PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
      FirstInput=0; 
      NeedReDraw = 1;
      
      
      // ����� ��� ���� �������� ������� ��� ������ ���������� �����
    }
    else // ���� ��������� �����
    {
      if(En_TX_Cmd)
      {
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
        //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
        HAL_UART_DMAStop(&huart2);
        
        // ����� ����������, ����� �������� �������
        sprintf((char*)StrTXOpt, "####SWL\rUUU");//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
        TxOptBusy = 1;
        //TsT(1);
        Mod_At_cnt++; // +50��
        En_TX_Cmd = 0;
        lSofn = 1; //
        NeedReDraw = 1;
      }
      // ����� ������� ������������ ��������, 
      if(Mod_At_cnt==tziLnRClb) // 1���
      {
        Start_Cmd = MaxAT+2; // 
        CountLN=0; // ������ ������� ���������
        
        TIM2->CNT = 0; // �������� ������ ��������� ����� (���� ������� 25 �����)
        TIM3->CNT = 0;// ������� �������� �������� ���������
        TIM2->PSC = 0;
        TIM3->PSC = 0;
        // ������ ������ 2 �� ����� ������ ������, ������� � ������ �����
        TIM2->ARR = xImp+20;
        TIM2->CCR1 = 20; // Zimp = 100-20
        uint32_t xWaitA=10000;
        //  WaitCnt =100000;
        NeedTransAnsver=1;// ������ �������� ���������� � ������� �������
        // ��������� ������ DWT (�������� ��� ���� ��������� ����� �������������)
        //SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// ��������� ������������ DWT
        //DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // �������� �������
        //DWT_CYCCNT = 0;// �������� �������
        //SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk; // ��������� ��������� ������
        HAL_TIM_PWM_Start (&htim3, TIM_CHANNEL_3 ); // ����� ��� ���� ��������� �������
        //CntrlA(1); // ������ ����������� ����� ��� ��������� ������ � �������� ���������� ��� ������� ��������
        Beg_Evnt = DWT_CYCCNT; // ���������� ������� �������
        // �������� ��� ������ �����������
#if defined (STM32F103xE) || defined (STM32F103xG) || defined(STM32F303xE) || defined(STM32F205xx)
        EXTI->SWIER|=4; // ��������� ����������� ���������� ��� �� �� �������� ������� ��� ������� TIM2
#endif        
#if defined (STM32L476xx) 
        EXTI->SWIER1|=4; // ��������� ����������� ���������� ��� �� �� �������� ������� ��� ������� TIM2
#endif        
        //HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_1 ); // ����� ��� ���� ��������� �������
        // ����� �� �������� ��� �������������� ��������
        while(xWaitA--); // ���� ������ 1.38 ��
        //SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk; // ����� �������� ��������� ������
        //CntrlA(0); // ������� ����������� ����� ��� ������� ����� ��� ������ 
        
        //HAL_Delay(10); 
        // �������� ����� ���������� ����������
        //sprintf((void*)StrAnsverData,"%d,%d,%d,%d\r",DataLN[CntLnCount][0],DataLN[CntLnCount][1],DataLN[CntLnCount][2],DataLN[CntLnCount][3]);
        //HAL_UART_Transmit_DMA(&huart1, (void*)StrAnsverData,strlen((void*)StrAnsverData)); // ������ 
        if(CntLnCount<NUMMEASLN-1)// �������� ��������� ����� (���� ��� ����������), ����� 8 ���
        {
          Mod_At_cnt--;// ������ ����� �����, ����� �������� ���������
          // ���� ���� ������ ������� �� Slave , ��������� �� ����� UART1 ������
          CntLnCount++;
        }
        else
          Mod_At_cnt++;
        
        
      }
      // ����� �������� ������� � �������
      IndxValidLN = 0; // ������ ������ ������
      if(Mod_At_cnt==teLnRClb) // 1,5 ���// ����� ������ �� ������
      {
        // ����� ���� ���������� � ����������� �� ������, ���������� ��� ���������
        //SCB_DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;// ��������� ������������ DWT
        //DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk; // ��������� �������
        
        //TIM2->PSC = 0;
        //TIM3->PSC = 0;
        //
        // ����� ������ ��� ������� ��������� ��� ��� ����
        SW5V(0);// OFF 5V
        lSofn = 0; //
        cntr=0;
        Summx=0;
        if(Mod_At == LnR_Clb) // ����������
        {
          for(int j=0;j<SEQMEASLN;j++) // ������ � ������� ���������
          {
            if((DataLN[2][j]>20300)&&(DataLN[2][j]<20700))
            {
              IndxValidLN = j;
              break;
            }
          }
          
          // ����� ��������� ���� ������ �� ����� , ����� ���� ����� ������ �� ��������
          for(int i=1;i<NUMMEASLN;i++)
          {
            if((DataLN[i][IndxValidLN]>20300)&&(DataLN[i][IndxValidLN]<20700))
            {
              Summx +=DataLN[i][IndxValidLN];
              cntr++;
            }
          }
          if(cntr)
            Summx=Summx/cntr;// ������� ������� ���� ��� ���������� ���������
          CalibLnAuto=(uint16_t)Summx;
          // ���� ��������� � ������
          UserMeasConfig.CalibLnAutoMem = CalibLnAuto;
          NeedSaveChange |=0x01; // ��������� ���������������� ��������� (���������� �����)
          memcpy(MsgAuto[0],Msg[6],strlen(Msg[6])); // ���������� ���������
          memcpy(MsgAuto[1],Msg[8],strlen(Msg[8]));
          Mod_At_cnt = MaxAT; // ���������
          Mod_At = Wt;
        }
        else // ���������, ��� �������, ���� ���������� ������
        {
          for(int j=0;j<SEQMEASLN;j++) // ������ � ������� ���������
          {
            if((DataLN[2][j]>UserMeasConfig.CalibLnAutoMem))
            {
              IndxValidLN = j;
              break;
            }
          }
          // ����� ��������� ���� ������ �� ����� , ������� ����������� ��� ��� ������ ��� ������ �����
          for(int i=1;i<NUMMEASLN;i++)
          {
            if((DataLN[i][IndxValidLN]>20300)&&(DataLN[i][IndxValidLN]<61700))
            {
              Summx +=DataLN[i][IndxValidLN];
              cntr++;
            }
          }
          if(cntr)
            Summx=Summx/cntr;// ������� ���������� ���������
          // ������ ���������� ������� ��� ���������.
          MeasLenghtAuto=((uint16_t)Summx>CalibLnAuto)?((uint16_t)(((uint16_t)Summx-CalibLnAuto)*DeviceConfig.KGen)):(0);
          PonA.LnA =((uint16_t)Summx>CalibLnAuto)?((uint16_t)(((uint16_t)Summx-CalibLnAuto)*DeviceConfig.KGen)):(0);
          Mod_At_cnt = MaxAT; // ���������
          Mod_At = View;
        }
        
        NeedReDraw = 1;
        
      }
    }
    
    break;// LnR_Clb
    //-------------------->>>>  Lnx // ��������� �����
  case Lnx:  // (Slave) 
    // ����� �������� � ����� �������� �������� �� �������
    if (Mod_At_cnt==tsLnX) // �������� �� ��������� ������ �������� ����� ������� 
    {
      lSofn = 1; //���������
      
      CountLN=0; // ������ ������� ���������
      // ��������� ������� max856
      //P3OUT |=CS_epm;//-CS -> 1
      //P1OUT &= ~on_Vout;
      //P3OUT |= on_Gen;
      //P1OUT &= ~Izm_ST;        TIM2->CNT = 0; // �������� ������ ��������� ����� (���� ������� 25 �����)
      TIM3->CNT = 0;// ������� �������� �������� ���������
      TIM2->CNT = 0;// ������� ������������ ��������� ��������
      TIM2->PSC = 0; // 0 - 72 MHz
      TIM3->PSC = 0;
      WaitCntLnX = 200; // ������������� ������ Slave �� �������� ������� �� �������
      //uint32_t xWaitB = 1000000;
      // ������ ������ 2 �� ����� ������ ������, ������� � �������� ����� 32000
      TIM2->ARR = xImp+20000;
      TIM2->CCR1 = 20000; // Zimp = 32000-31920 = 80
      NeedTransAnsver=1; // ������������� ������ Slave �� �������� ������� �� �������
      TsT(1); // ��������, ��������� ��� ���� ��������� � ��������� 1��, ������� � ���������� �� ������
      //while(xWaitB--); // ����� ��� ����������� ������ 
      
      Mod_At_cnt++;
      //TsT(1); // ������ ����� ��� ����������� �� ��������
      //WaitCnt =20000000;
      
      //HAL_TIM_PWM_Start (&htim3, TIM_CHANNEL_3 ); // ����� ��� ���� ��������� �������, �� ���� ������ � ���� ������ �� ����� �����
      //SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk; // ��������� ��������� ������
      // ����� �� �������� ��� �������������� ��������
      //while(WaitCnt--); // ���� ��� ������ ~0.2�
      //SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk; // ����� �������� ��������� ������
      //NeedReDraw = 1;
    }
    // ����� ������ �� ������ 
    if (Mod_At_cnt>teLnX) // ��� ������ �������� �� slave ������� � View
    {
      NeedTransAnsver=0; // ������ ������ Slave �� �������� ������� �� �������
      TsT(0); // ��������, ��������� ��� ���� ��������� � ��������� 1��, ������� � ���������� �� ������
      //memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // �������� ������
      //memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
      //TIM2->PSC = 0; // stop timers
      //TIM3->PSC = 0;
      SW5V(0);// OFF 5V
      //TsT(0); // �������� 
      Mod_At = Wt;
      Mod_At_cnt=MaxAT;
      lSofn = 0; //
      FirstInput=1; 
      NeedReDraw = 1;
    }
    
    break;  // Lnx
    //________________ CalPw ����� SLAVE � ������ �������� �� ��������� ��������
    // ��������� �������
    // ��� �� �������� ��� Px � ��������
  case CalPw:
  case Px:
    
    if(En_TX_Cmd) // ��� 40 (2���) �������� �����
    {
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
      //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
      
      // ����� ����������, ����� �������� �������
      sprintf((char*)StrTXOpt, "####YP%.2f\rUUU", Pow_SC_Tx);//
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
      TxOptBusy = 1;
      //TsT(1);
      En_TX_Cmd = 0;
      //memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // �������� ������
      //memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
      Mod_At_cnt=MaxAT;
      Mod_At = Wt;
      lSofn = 0; //
      FirstInput=1; 
      NeedReDraw = 1;
    }
    // ����� ��������� �������� �� �������
    if (Mod_At_cnt==tsCalPw) //��������� � ������ ��� ����������
    {
      //P2OUT |= LD_R; 
      Pow_SC_Tx = P2dBmValue; // ��������� �������� P2 �� ������ ����� �����
      Mod_At_cnt++; // �������� �������
      lSofn = 1; //
      // test draw
      CntrlA(1); // ������ ����������� ����� 
//      sprintf((char*)MsgAuto[2], "%d %.2f %d",PMWavelenght, Pow_SC_Tx, TestCntWait);//
      
      NeedReDraw = 1;
      
      //P2OUT &=~LD_R;
    }
    break; // CalPw    
    //_____________________->>> ��������� ORL ������
  case Orl:
    if(FirstInput)
    {
      memcpy(MsgAuto[0],Msg[13],strlen(Msg[13])); // ��������� (����� �������)
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
      //����� ��� ��� ����������
      
      Cnt_SPw = 0; // ����� ������� �������� �������
      Pow_SC_Tx = 0.0; // �������� ��������� ��������� �� ���������
      Err_Cmd_At = 0; //����� ������ ������ � ������  At
      Start_Cmd = tsORL; // ������ ��� ����� � ������� �������� ������� ��� ������������ Slave � ����� ��������� �������� �� �������
      Mod_At_cnt = 0; // ����� �������� ������� �������
      GetSetLW_SC (0, 1); //��������� ������ ����� ������� ������� 0-������,1-� ������ �����������, ����� ��������������� Ind_LW
      FirstInput = 0; 
      NeedReDraw = 1;
      // 
    }
    else
    {
      // ��������� ORL � ��������� ��������� �� Slave
      if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // ��������� ���������� ������� � �� ��� ��������� �� ��������
      {   
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
        //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
        
        // ����� ����������, ����� �������� �������
        sprintf((char*)StrTXOpt, "####SLA%4u %.2f\rUUU",DeviceConfig.PlaceLS[Ind_LW],SettingPrm.PowSc[Ind_LW]);//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
        TxOptBusy = 1;
        //TsT(1);
        Mod_At_cnt++; // +50��
        tSt_Cnt++;
        En_TX_Cmd = 0;
        lSofn = 1; //
        PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
        IndxP2LW = GetIndxLW(PMWavelenght);
        Indxi = IndxP2LW; // // ��������� ������ ���������� ��������� ��� ���������
        
        MsgStat[Ind_LW] = 'o'; // 
        //MsgStat[Ind_LW] = 48+Ind_LW; // 0-1-2
        sprintf((char*)MsgAuto[1], "%s",MsgStat);
        //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
        NeedReDraw = 1;
        
      }
      else 
      {
      } 
      // ������������ ������� ��� ��������� ����������� ��������� (
      if(TxOptBusy==0)
      {
        // ����� ��������� ���������
        // ����� ���������� ���������
        if((Mod_At_cnt>tseSavPw)&&(Mod_At_cnt<tsdSavPw))
        {
          CW_S(1);
          //NeedReDraw = 1;
        }
        else
        {
          CW_S(0);
          lSofn = 0; //
          //NeedReDraw = 1;
        }
      }
      
    }
    // ����� ��� ���� ����� ORL 
    if(Mod_At_cnt==tseSavPw+35)
    {
      
      //PonA.ORL_AB[Ind_LW]=ORLCalc ; // ��� ��� �� ��������� ������! 
      PonA.ORL_AB[Indxi]=ORLCalc ; // ��� ��� �� ��������� ������! 
      Mod_At_cnt++;
    }
    
    // �������� ���� �� ����� �� Slave , ���� ���� �� ������� ��������� � ������� ����� ����
    if(OpticCMD == YPcmd)
    {
      //PonA.PW_AB[Ind_LW] = fParamOpt;
      PonA.PW_AB[Indxi] = fParamOpt; //����� � ���� ����� ������������ �����
      //PonA.LS_AB[Ind_LW] = PonA.PW_AB[Ind_LW] - SettingPrm.PowSc[Ind_LW];// ���������� �������� ������ ������������ Slave , �������� ���������
      PonA.LS_AB[Indxi] = PonA.PW_AB[Indxi] - SettingPrm.PowSc[Ind_LW];// ���������� �������� ������ ������������ Slave , �������� ���������
      
      OpticCMD = NOTcmd;
      Cnt_SPw++;
      GetSetLW_SC (1, 1); //��������� ��������� ����� ������� ������� 0-������,1-� ������ �����������
      Mod_At_cnt=0; // ����� �������� ������� �������
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
      NeedReDraw = 1;
      if (Cnt_SPw == NumChoiceLS) // ��� �������� � �������
      {
        //myBeep(200); // ����� ���������� ���������
        ControlSC (0); // ���������� ���������� -  ���������
        Mod_At = CmdSw; // ������������ � ����� ������������ Slave � ����� "��������" ,
        // ��������� �������� ��� ��������� ��������� � ORL �� ������� Slave
        // ������� �� ������� ������ ���� ������������ Slave
        Mod_At_cnt=0; // ����� �������� ������� �������
        Start_Cmd = tsCmdSw; // 1
        MsgStat[3] = 'o'; // 
        sprintf((char*)MsgAuto[1], "%s",MsgStat);
        NeedReDraw = 1;
      }
      
    }
    // ���������� ����� �������� �������
    if (Mod_At_cnt>teSavPw) // ��� ������ � �������� ��������� 5 sec
    {
      Mod_At_cnt = MaxAT;
      Mod_At = Wt;
      memcpy(MsgAuto[0],Msg[4],strlen(Msg[4])); // ������
      memcpy(MsgAuto[1],Msg[15],strlen(Msg[15])); // ��� �����
      NeedReDraw = 1;
      OpticCMD = NOTcmd; // ������� ��� �� ������������ ������
      
      //Start_Cmd = 0;
      //sprintf(Str4, "    ������    ");
      //sprintf(Str5, "  ����������  ");
    }
    break; // Orl
    //_____________________->>> ������ �������� ���������� Slave
  case CmdSw:
    if (En_TX_Cmd) // ��������� ���������� ������� 
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
      //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
      
      // ����� ����������, ����� �������� �������
      sprintf((char*)StrTXOpt, "####SWM\rUUU");//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
      TxOptBusy = 1;
      //TsT(1);
      Mod_At_cnt++; // +50��
      En_TX_Cmd = 0;
      lSofn = 1; //
      MsgStat[4] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
      NeedReDraw = 1;
    }
    if(Mod_At_cnt==tsoCmdSw) // 5
      // ��������� �������� (�������� ������ ��������)
    {
      lSofn = 0; //
      NeedReDraw = 1;
    }
    // �������� ���� �� ����� �� Slave , ���� ���� 
    if(OpticCMD == SOKcmd)
    {
      Mod_At = Sl_Px; // ������� ������� � ����� ��������� �������� �� Slave, 
      Cnt_BA = 0;
      Pow_SC_Tx = 0.0;
      Err_Cmd_At = 0;
      MsgStat[5] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 1;
    }
    // ���������� ����� �������� �������
    if (Mod_At_cnt>teCmdSw) // ��� ������ 20 tick
    {
      Mod_At_cnt = MaxAT;
      Mod_At = Wt;
      memcpy(MsgAuto[0],Msg[4],strlen(Msg[4])); // ������
      memcpy(MsgAuto[1],Msg[15],strlen(Msg[15])); // ��� �����
      NeedReDraw = 1;
    }
    
    break; // CmdSw
    //_____________________->>> ������ �������� �������� �� Slave, ���� ������� SLA
    // � ����������� ������������� ��������� (����� ����� � ��������, ��� �� ���������� ���������)
  case Sl_Px:
    // �������� ���� �� ������ �� Slave , ���� ���� �� ������ �������� ���������� ������
    if(OpticCMD == SLAcmd)
    {
      NeedReadPWSlave = 1;
      PMWavelenght = iParamOpt; // � ������ ������ ���������� ����� ����� ������������� ��������� �� �������
      LWSlaveSRC[Cnt_BA] = PMWavelenght; // ����� ����� ��������� Slave � ������ �������
      IndxP2LW = GetIndxLW(PMWavelenght); // ������� ������ ����� ����� ��� ����������� ��������� ������ � P2
      Indxi = IndxP2LW;
      PonA.LW_BA[Cnt_BA] = PMWavelenght; // ����� ����� ��������� Slave � ������ �������
      PonA.PS_BA[IndxP2LW] = fParamOpt; // �������� ��������� Slave � ������ �������
      // ������ �������� ������������ �������� � �������� �� �����
      MsgStat[6+Cnt_BA] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 1;
      OpticCMD = NOTcmd;
      
    }
    if((Mod_At_cnt==40) && NeedReadPWSlave)// ���������� � ������� ��������� BA
    {
      NeedReadPWSlave = 0;
      PonA.PW_BA[Indxi] = P2dBmValue;
      PonA.LS_BA[Indxi] = PonA.PW_BA[Indxi]-PonA.PS_BA[Indxi]; // �������������
      Mod_At_cnt = 0;
      Cnt_BA++;
    }
    
    if(Mod_At_cnt>100) // ����� ������ ��� , ������� ����� � ����� "�������" � ���������� ���������
      // ���� ��������� ��������� ORL �� ������� �
    {
      Mod_At = AskOrl; // ������� ������� � ����� ������� ORL �� Slave �� Slave, 
      Start_Cmd = tsCmdSw; // 1
      Cnt_BA = 0;
      Pow_SC_Tx = 0.0;
      Err_Cmd_At = 0;
      MsgStat[9] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 1;
      
    }
    break; // Sl_Px
    //_____________________->>>  Master, ��������� ������� SRL
  case AskOrl:
    if (En_TX_Cmd) // ��������� ���������� ������� 
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
      //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
      
      // ����� ����������, ����� �������� �������
      sprintf((char*)StrTXOpt, "####SRL\rUUU");//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
      TxOptBusy = 1;
      //TsT(1);
      Mod_At_cnt++; // +50��
      Start_Cmd = MaxAT+2; // ����� ������ ������ �� ����������
      Cnt_BA=0;
      En_TX_Cmd = 0;
      
      lSofn = 1; //
      MsgStat[10] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
      NeedReDraw = 1;
    }
    // ���� ������ ������ �� Slave (� ����� � ������)
    if(OpticCMD == SLOcmd)
    {
      PMWavelenght = iParamOpt; // � ������ ������ ���������� ����� ����� ������������� ��������� �� �������
      //LWSlaveSRC[Cnt_BA] = PMWavelenght; // ����� ����� ��������� Slave � ������ �������
      IndxP2LW = GetIndxLW(PMWavelenght); // ������� ������ ����� ����� ��� ����������� ��������� ������ � P2
      PonA.ORL_BA[IndxP2LW] = fParamOpt; // �������� ��������� Slave � ������ �������
      MsgStat[11+Cnt_BA] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 1;
      OpticCMD = NOTcmd;
      Cnt_BA++;
    }
    if(Mod_At_cnt>100) // ����� ������ ��� , ������� � �����  � ���������� ���������
      // ���� ��������� ��������� �����
    {
      ControlSC (0); // ���������� ���������� -  ���������
      Mod_At = LnR; // ������������ � ����� � ��������� ������� ������� �����
      //Err_Cmd_At=0; // ����� ������ ������ � ������  At
      //Start_Cmd = 6; // ������ ��� ����� � ������� �������� ������� 0.25 ���
      //Mod_At_cnt=0; // ����� �������� ������� �������
      // ������������� ��������
      TIM2->CNT = 0; // �������� ������ ��������� ����� (���� ������� 25 �����)
      TIM3->CNT = 0;// ������� �������� �������� ���������
      TIM2->PSC = 0; // 0- 72MHz
      TIM3->PSC = 0;
      lSofn = 0; //
      
      En_TX_Cmd = 0;
      FirstInput=1; 
      CntLnCount = 0; // ����� �������� ������ ��������� �����
      //memset(DataLN,0, ARRAY_SIZE(DataLN)); // ������ ������ ������ ������
      memset(DataLN,0, NUMMEASLN*SEQMEASLN*2); // ������ ������ ������ ������
      MsgStat[14] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      NeedReDraw = 1;
    }
    
    break; // AskOrl
    //_____________________->>>  Slave, ��������� ������ �� ������� SRl
    // ������ ���������� �������� ORL Slave, ��� �������� �������
  case AnsOrl:
    if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // ��������� ���������� ������� � �� ��� ��������� �� ��������
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
      //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
      
      Indxi = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ����� ����� ��� ����������� ��������� ������ � P2
      // ����� ����������, ����� �������� �������
      sprintf((char*)StrTXOpt, "####SLO%4u %.2f\rUUU",DeviceConfig.PlaceLS[Ind_LW],PonA.ORL_BA[Indxi]);//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
      TxOptBusy = 1;
      //TsT(1);
      Mod_At_cnt++; // +50��
      En_TX_Cmd = 0;
      lSofn = 1; //
      NeedReDraw = 1;
    }
    // �������� ��� �� ��������, ���� �� �� ����� � ��������
    if(Mod_At_cnt==tnAnsOrl)
    {
      Cnt_SPw++;
      Mod_At_cnt=0; // ����� �������� ������� �������
      GetSetLW_SC (1, 1); //��������� ������ ����� ������� ������� 0-������,1-� ������ �����������
      
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
      if (Cnt_SPw == NumChoiceLS) // ��� �������� 
      {
        //myBeep(200); // ����� ���������� ���������
        ControlSC (0); // ���������� ���������� -  ���������
        memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // �������� ������
        memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
        Mod_At_cnt=MaxAT;
        Mod_At = Wt;
        lSofn = 0; //
      }
      NeedReDraw = 1;
      
    }
    
    
    break; // AnsOrl
    //_____________________->>>  Slave, ��������� ������� SOK
  case AnsOK:
    if (En_TX_Cmd) // ��������� ���������� ������� 
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
      //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
      
      // ����� ����������, ����� �������� �������
      sprintf((char*)StrTXOpt, "####SOK\rUUU");//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
      TxOptBusy = 1;
      //TsT(1);
      Mod_At_cnt++; // +50��
      En_TX_Cmd = 0;
      lSofn = 1; //
      NeedReDraw = 1;
    }
    if (Mod_At_cnt>6) // ����� �� = ������������� � ��������� ��������
    {
      Mod_At = Sl_Orl;
      Cnt_SPw = 0;
      Start_Cmd = tsSavPw; // ��� ��� ���������� � ORL �������, ������ ������ ��� Slave
      Mod_At_cnt = 0;
      FirstInput = 1;
      NeedReDraw = 1;
    }
    break; // AnsOK
    //_____________________->>> ��������� ORL SLAVE
  case Sl_Orl:
    if(FirstInput)
    {
      memcpy(MsgAuto[0],Msg[17],strlen(Msg[17])); // ��������� 
      memcpy(MsgAuto[1],Msg[14],strlen(Msg[14])); // ��������
      //����� ��� ��� ����������
      
      Cnt_SPw = 0; // ����� ������� �������� �������
      Err_Cmd_At = 0; //����� ������ ������ � ������  At
      Start_Cmd = tsORL; // ������ ��� ����� � ������� �������� ������� ��� ������������ Slave � ����� ��������� �������� �� �������
      Mod_At_cnt = 0; // ����� �������� ������� �������
      GetSetLW_SC (0, 1); //��������� ������ ����� ������� ������� 0-������,1-� ������ �����������, ����� ��������������� Ind_LW
      FirstInput = 0; 
      NeedReDraw = 1;
      // 
    }
    else
    {
      // ��������� ORL 
      if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // ��������� ���������� ������� � �� ��� ��������� �� ��������
      {   
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
        //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 
        
        // ����� ����������, ����� �������� �������
        sprintf((char*)StrTXOpt, "####SLA%4u %.2f\rUUU",DeviceConfig.PlaceLS[Ind_LW],SettingPrm.PowSc[Ind_LW]);//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // ������ 
        TxOptBusy = 1;
        //TsT(1);
        Mod_At_cnt++; // +50��
        En_TX_Cmd = 0;
        lSofn = 1; //
        PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
        IndxP2LW = GetIndxLW(PMWavelenght);
        Indxi = IndxP2LW; // // ��������� ������ ���������� ��������� ��� ���������
        NeedReDraw = 1;
      }
      else 
      {
      } 
      // ������������ ������� ��� ��������� ����������� ��������� (
      if(TxOptBusy==0)
      {
        // ����� ��������� ���������
        // ����� ���������� ���������
        if((Mod_At_cnt>tseSavPw)&&(Mod_At_cnt<tsdSavPw))
        {
          CW_S(1);
          //NeedReDraw = 1;
        }
        else
        {
          CW_S(0);
          lSofn = 0; //
          //NeedReDraw = 1;
        }
      }
    }
    // ����� ��� ���� ����� ORL 
    if(Mod_At_cnt==tseSavPw+35)
    {
      PonA.ORL_BA[Indxi]=ORLCalc ; // ��� ��� �� ��������� ������!
      Mod_At_cnt++;
    }
    //��������� ������� �������, ���������� + 0.5�
    if(Mod_At_cnt==tsdSavPw+20)
    {
      Cnt_SPw++;
      GetSetLW_SC (1, 1); //��������� ��������� ����� ������� ������� 0-������,1-� ������ �����������
      Mod_At_cnt=0; // ����� �������� ������� �������
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // � ������ ������ ���������� ����� ����� ������������� ���������
      if (Cnt_SPw == NumChoiceLS) // ��� �������� 
      {
        //myBeep(200); // ����� ���������� ���������
        ControlSC (0); // ���������� ���������� -  ���������
        memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // �������� ������
        memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
        Mod_At_cnt=MaxAT;
        Mod_At = Wt;
        lSofn = 0; //
      }
      NeedReDraw = 1;
      
    }
    break; // Sl_Orl
  }
}
