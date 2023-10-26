// ������� ��������� ���������� , �������� ��� MSP430, ��� ���������� � ��������� � ����������� �� ������
//                                   ������ �������                    
//   on/off   0x08   0�01                  0x77    0x7E  
//
//    0�40    0�20   0�02         0x3F     0x5F    0x7D              
//
//    0�10           0�04         ---      ---     0x7B             
//
//    on/off   Up    dB/dBm/W
//
//    Left    Down   Right 
//
//    Mode            S
//

// ���������� ������ ������ ������ ��������!
// 0x08 + 0x01 
#include "main.h"
      uint16_t ImpZ[4]={6,6,6,6};     

        uint8_t DayMax;
static volatile uint32_t dCh;
static volatile uint32_t Vel;
static volatile uint8_t Indx;
static volatile uint16_t AdrWriteMem; 
uint8_t CountPON=0; // ������� ��� �������� ����� ����� ��� ������ ��� ������������ ����� PON
     uint32_t ttt;
uint8_t yesLV;
int i,j;
void KeyBoardAnsver( uint8_t CodeKeys)
{
  
  switch (CodeKeys)
  {
           //_____________________________________________________________________________________________ 
  //case 0x6B: // ���������� ������ 0x04-0x10 (������) ��������� ���������� ���� ����������
  case 0x57: // ���������� ������ 0x08-0x20 �����-����(������) ��������� ���������� ���� ����������
    if(UserMeasConfig.AutoOff)
      UserMeasConfig.AutoOff=0;
    else
      UserMeasConfig.AutoOff=1;
                    NeedSaveChange |=0x10; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
    myBeep(50);
    HAL_Delay(200);
    myBeep(50);
      
break;
       //_____________________________________________________________________________________________ 
  case 0x76: // ���������� ������ 0x01-0x08 (������) ����� ���������� � ������ �������
          switch (CondWork)
          {
          case At: // ������ ������ ���������� ���������� � ������ ������������� �����
            if ((Mod_At == Wt)||(Mod_At == View)||(Mod_At == Err_Ans)||(Mod_At == Clb_Err))
            {
              FirstInput = 1;// ������ ��� ����� 
              Mod_At = SavPw; // �������� ����� ����������
            }
            break;
          case P1a: // ����������� ��������� � ���� ����... 
            for(i=0; i < ARRAY_SIZE(RPON); ++i) // ��������� ������ RPON �� ������� ��������
            {
              for(j=0; j < 3; ++j) // ��������� ����� ���� ���������
              {
                if(DeviceConfig.PlaceLS[j] == CalibrLW[i]) // ��� ���������� ���� ���� ������� � ���������,
                  // �������� ��������, ���� �� ����� ����������
                {
                  if((RPON[i]>-3.0)&&(RPON[i]<3.0)) // � ������ ���������, ����� ��������
                  {
                    SettingPrm.PowSc[j] = RPON[i];
                    NeedSaveChange |=0x10; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
                    
                  }
                }
              }
            }
            //SettingPrm.PowSc[0]
            break;
          }
    myBeep(500);
    HAL_Delay(800);
    myBeep(500);
    break;

   //_____________________________________________________________________________________________ 
  case 0x72: // ���������� ������ 0x02-0x20-0x40-0x10 (��� ����� Setting � ������ �������)
    myBeep(500);
    HAL_Delay(800);
    myBeep(500);
    break;
    //____________________________________________________________________________________________
  case 0x01: // ������ "dBm/dB/W"
    switch (CondWork)
    {
    case P1: // ���������� �1
      //case P1a:
      switch (ModeI)
      {
      case W :
        ModeI=dB;
        break;
      case dBm :
        ModeI=W;
        break;
      case dB :
        ModeI=dBm;
        break;
        //        case WrHold: // auoia ec caiene a ?a?eia HOLD
        //          Displ=0;
        //          ShowMsg(9); // yoi ?enoeo ianoi aaa auei "Caienu ?"
        //        case Hold:
        //          Displ=0;
        //          ShowMsg(35);
        //          Displ=2;
        //          Mode=OldMode;
        //          break;
        //        default :
        //          Mode=OldMode;
        //          Displ=0;
        //          ShowMsg(9);
        //          ShowMsg(23);
        //          if (PowerMode==On) Displ=2;
        //          break;
      }
      //        Num2Lenght();
      break;
    case P1a:
      switch (ModeIP1a)
      {
      case W : // ��������� ��������� ��� �������� � �������� ������
        ModeIP1a=dBm;
        break;
      case dBm :
        ModeIP1a=dB;
        break;
      case dB :
        ModeIP1a=W;
        break;
      }
      break;
    case P2:
      if(P2Mode == dBm) P2Mode = dB;
      else P2Mode = dBm;
      NeedReDraw = 1;
      break;
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sAutoOff:
        if(UserMeasConfig.AutoOff)
          UserMeasConfig.AutoOff=0;
        else
          UserMeasConfig.AutoOff=1;
        break;
        
      case sNumF:
        if(SettingPrm.EnaAutoInkrNF) SettingPrm.EnaAutoInkrNF=0;
        else SettingPrm.EnaAutoInkrNF=1;
        break;
      case sLOSS:
        if(SettingPrm.EnaCmpLOSS) SettingPrm.EnaCmpLOSS=0;
        else SettingPrm.EnaCmpLOSS=1;
        break;
      case sORL:
        if(SettingPrm.EnaCmpORL) SettingPrm.EnaCmpORL=0;
        else SettingPrm.EnaCmpORL=1;
        break;
      case sSWm:
        if(IndxEnaSave)
        {
          Indx = 1<<(IndxEnaSave-1);
          if(SettingPrm.EnaWrtMem & Indx) SettingPrm.EnaWrtMem &=~Indx;
          else SettingPrm.EnaWrtMem |=Indx;
        }
        break;
      case sMem:
        if(IndxClrMEM)
        {
          //������ ������
          SettingPrm.CountMemory=0;
          
        }
        IndxClrMEM = 0;
        break;
      }
        NeedSaveChange |=0x10; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
      break;
    }
    break;
    //_______________________________________________________________________________
  case 0x77: // �����  ����� ������ 
    switch(CondWork)
    {
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sSCp:
        if(IndxPowSC)
        {
          if((SettingPrm.PowSc[IndxPowSC-1]-0.1)<5.0 ) SettingPrm.PowSc[IndxPowSC-1] +=0.1;
          else SettingPrm.PowSc[IndxPowSC-1]=-10.;
        }
        break;
      }
      NeedReDraw=1;
      
    }
    
    break;
    //_______________________________________________________________________________
  case 0x5F: // ����  ����� ������ 
    switch(CondWork)
    {
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sSCp:
        if(IndxPowSC)
        {
          if((SettingPrm.PowSc[IndxPowSC-1])>-9.9 ) SettingPrm.PowSc[IndxPowSC-1] -=0.1;
          else SettingPrm.PowSc[IndxPowSC-1]=5.;
        }
        break;
      }
      NeedReDraw=1;
      
    }
    
    break;
    //_______________________________________________________________________________
  case 0x02: // ������ , ������ 
    switch(CondWork)
    {
    case P1: // ������ ���������� ������������ �� ������ ���� ����������
      // ������� ������������� ����� ����
      // � ��������� �������� ������ ���������������
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght+5,0);//������� ����� �����. � ���������� ���������
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case RE: // ������� ���� ����������� ������ ������ 0-OFFs, 1-SCW, 2-S27W(1Hz),
      switch(ReMode)
      {
      case OFFs:
        ReMode=SCW;
        ControlRE(1);// ���.RE
        break;
      case SCW:
        ReMode=S27W;
        break;
      case S27W:
        ReMode=OFFs;
        ControlRE(0);// ����.RE
        break;
      default:
        ReMode=OFFs;
        ControlRE(0);// ����.RE
        break;
      }
      break;
    case Sc: // ����������� �������� (��������� ������ � �������� ����� )
      switch(ScMode)
      {
      case OFFs:
        ScMode=SCW;
        break;
      case SCW:
        ScMode=S27W;
        break;
      case S27W:
        ScMode=S2KW;
        break;
      case S2KW:
        ScMode=SCW_P;
        break;
      case SCW_P:
        ScMode=OFFs;
        break;
      default:
        ScMode=OFFs;
        break;
      }
      ControlSC (ScMode); //// ���������� ���������� 
      break;  
      // ���������� �����
    case Ln:
      if(DataLN[1][IndxShowLN+1] != 0)
      {
        IndxShowLN++;
      }
      else
        IndxShowLN=0;
      break;
  //============ �������    
    case At:
      if(Mod_At == View)
      {
        switch (Mod_View)
        {
        case LOSS:
          Mod_View = ORL;
          break;
        case ORL:
          Mod_View = LNM;
          break;
        case LNM:
          Mod_View = LOSS;
          break;
        }
      }
      break;  
      
    case SetFn:
      {
        switch (ConSet)
        {
        case P1:
          /* 
          if (DataDevice[17]!='2') // ia i?inoi ecia?eoaeu => ia?aee??aainy ia enoi?iee eceo?aiey
          {
          ConSet = Sc; 
          sprintf(Str4, " Enoi?iee ");
          sprintf(Str5, "eceo?aie\xff ");
        }
          else
          */
          if ((DeviceConfig.CfgRE & 0x1) &&(N_LS==0)) //���� ������� ���� � ������ ����������
          {
            ConSet = RE; 
          }
          else
          {
            ConSet = P1a; 
          }
          break;
        case RE:
          if (N_LS == 0) // ������ ����������
          {
            ConSet = P1a; 
          }
          else //  ��������
          {
            ConSet = Sc; 
          }
          break;
        case P1a:
          if (1)//!En_Cfg_P1a
          {
            if (N_LS) //�� ������ ���������� ������ ��������
            {
              if (DeviceConfig.CfgRE & 0x1) //���� ������� ���� � ������ ����������
              {
                ConSet = RE; 
              }
              else
              {
                ConSet = Sc; 
              }
            }
            else
            {
              ConSet = P1; 
            }
          }
          else
          {
            //              Wind_Cfg = 1; 
            //              switch (Inv_Bas)
            //              {
            //              case 0x02: // 1.31
            //                if (Lim_LS[0] < -0.5) Lim_LS[0] +=0.5;
            //                else Lim_LS[0] = 0.0; 
            //                LimX = (int)(Lim_LS[0]*2);
            //                Lim_LS[0] = (float)LimX/2;
            //                break;
            //              case 0x08: // 1.49
            //                if (Lim_LS[1] < -0.5) Lim_LS[1] +=0.5;
            //                else Lim_LS[1] = 0.0; 
            //                LimX = (int)(Lim_LS[1]*2);
            //                Lim_LS[1] = (float)LimX/2;
            //                break;
            //              case 0x20: //1.55
            //                if (Lim_LS[2] < -0.5) Lim_LS[2] +=0.5;
            //                else Lim_LS[2] = 0.0; 
            //                LimX = (int)(Lim_LS[2]*2);
            //                Lim_LS[2] = (float)LimX/2;
            //                break;
            //              default: 
            //                break;
            //              }
            //Show_Wind_Cfg(Wind_Cfg);
          }
          break;
        case Sc:
          if (!DeviceConfig.CfgL) // ��� ���������� ������
          {
            if (DeviceConfig.CfgPM)// ���� ���������� � ������� ����
            {
              ConSet = P1; 
            }
            else
            {
              if (DeviceConfig.CfgRE & 0x1) //a���� ������� ����
              {
                ConSet = RE; 
              }
              else
              {
                ConSet = Sc; 
              }
            }
          }
          else
          {
            ConSet = At;
          }
          
          break;
        case At: // ����� �� ������ �������
          ConSet = P2;
          break;
        case P2:
          ConSet = Ln;
          break;
        case Ln:
          ConSet = Rl;
          break;
          // case Rl:
          //   ConSet = Ls;
          //   sprintf(Str4, "  Ecia?eoaeu  ");
          //   sprintf(Str5, "  caoooaie\xff   ");
          //   break;
        case Rl:
          if (DeviceConfig.CfgPM) // ���� ����������
          {
            ConSet = P1;
          }
          else 
          {
            if (DeviceConfig.CfgRE & 0x1) //���� ������� ����
            {
              ConSet = RE; 
            }
            else
            {
              ConSet = Sc; 
            }
            
          } 
          break;
        }
        ShowChooseMode();
        NeedReDraw=1;
        
      }
      break;
      // P1 Case ConSet 
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sTime:
        if(IndxEditClock<6)IndxEditClock++;
        else IndxEditClock = 0;
        break;
      case sAutoOff:
        if(UserMeasConfig.AutoOffMod<3)UserMeasConfig.AutoOffMod++;
        else UserMeasConfig.AutoOffMod = 0;
        break;
      case sNumF:
        if(IndxEditNumFbr>0)IndxEditNumFbr--;
        else IndxEditNumFbr = 5;
        break;
      case sLOSSm:
        if((UserMeasConfig.Lim_LS[IndxEditLOSSm]-0.5)<10.0 ) UserMeasConfig.Lim_LS[IndxEditLOSSm] +=0.5;
        else UserMeasConfig.Lim_LS[IndxEditLOSSm]=-100.;
        break;
      case sLOSS: // ������� ������ ���
        //if(IndxEditLOSS)
        {
          if((UserMeasConfig.Lim_LS[0]-0.5)<10.0 ) UserMeasConfig.Lim_LS[0] +=0.5;
          else UserMeasConfig.Lim_LS[0]=-100.;
        }
        break;
      case sORL:
        //if(IndxEditORL)
        {
          if((UserMeasConfig.Lim_ORL[0]-0.5)<60.0 ) UserMeasConfig.Lim_ORL[0] +=0.5;
          else UserMeasConfig.Lim_ORL[0]=0.;
        }
        break;
      case sSCp:
        if(IndxPowSC)
        {
          if((SettingPrm.PowSc[IndxPowSC-1]-0.01)<5.0 ) SettingPrm.PowSc[IndxPowSC-1] +=0.01;
          else SettingPrm.PowSc[IndxPowSC-1]=-10.;
        }
        break;
      }
      NeedReDraw=1;
      break;      
      
    }
    break;
    //_________________________________________________________________________________________________
  case 0x04: // ������ "S"
    switch(CondWork)
    {
    case SetPrm: //sTime, sNumF, sLOSS, sORL, sSCp, sMem, sSWm
      switch(SetupMod)
      {
      case sTime:
        SetupMod=sAutoOff;
        break;
      case sAutoOff:
        if((DeviceConfig.CfgPM)||(DeviceConfig.CfgL)) // ���� ���������� or AL w/o PM
        {
          SetupMod=sNumF; 
        }
       else 
       {
         if(N_LS) // ���� ���������
       SetupMod=sSCp; 
       else
       SetupMod=sVer; 
       }
        break;
      case sNumF:
       SetupMod=sMem;
       IndxClrMEM=0;
        break;
      case sMem:
       SetupMod=sLOSS; 
        break;
      case sLOSSm:
       SetupMod=sLOSS; 
        break;
      case sLOSS:
                 if(N_LS) // ���� ���������
       SetupMod=sSCp; 
       else
       SetupMod=sVer; 
        break;
      case sSCp:
                 if(DeviceConfig.CfgL) // ���� ������ ������� (���������� �����)
       SetupMod=sORL; 
       else
       SetupMod=sVer; 
        break;
      case sORL:
       //SetupMod=sSWm; 
       SetupMod=sVer; // ����� ������� �� ������ 30/09/2022
        break;
      case sSWm:
       SetupMod=sVer; 
        break;
      case sVer:
       SetupMod=sTime; 
        break;
      default:
       SetupMod=sTime; 
        break;
      }
      NeedReDraw=1;

      break;
    case Ln:
      //memset(DataLN,0, ARRAY_SIZE(DataLN)); // ������ ������ ������ ������
      SW5V(1);// ON 5V ������� 
     ttt=3000000;
     while(ttt--);
      
      memset(DataLN,0, NUMMEASLN*SEQMEASLN*2); // ������ ������ ������ ������
      lSofn = 1; //������
      CntLCD_LN=2; // ����������� ������� 12 �� ��� �������� ���������� ���������� ��� ����������� �����������
      CntLnCount=0;
      IndxShowLN = 0; // ����� �� ������ ���������
      for (int i=0;i<NUMMEASLN;i++)
      {
      CountLN=0; // ������ ������� ���������
     ttt=30000;
     CntLnCount=i;
     TIM2->PSC = 0; // �������� ������ ��������� ����� (���� ������� 25 �����)
     TIM3->PSC = 0;        
     // ������ ������ 2 �� ����� ������ ������, ������� � ������ �����
     //TIM2->ARR = 20+xImp;
     TIM2->ARR = 20+ImpZ[i];
     TIM2->CCR1 = 20; // Zimp = 100-20
     
     TIM2->CNT = 0; // �������� ������ ��������� ����� (���� ������� 25 �����)
     TIM3->CNT = 0;        

     HAL_TIM_PWM_Start (&htim3, TIM_CHANNEL_3 ); // ����� ��� ���� ��������� �������
     Beg_Evnt = DWT_CYCCNT; // ���������� ������� �������
     // ��������� �������
     HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_1 ); // ����� ��� ���� ��������� �������
     // ������ ��������
     while(ttt--);
     //CntLnCount++;
      }
      SW5V(0);// OFF 5V �������� 

      break;
    case At: // ������ ��������� � ������ �������
        if ((Mod_At == Wt)||(Mod_At == View))
        {
          FirstInput = 1;// ������ ��� ����� 
          Mod_At = Orl; // �������� ����� ��������� ORL
          Ini_At_var(); // ������������� ���������� ������ At
          
//          Cnt_SPw = 0; // ������ �������� ���������� � �������
//          Pow_SC_Tx = 0.0; // �������� �������� ��������� �� ���������
//          Err_Cmd_At=0; //����� ������ ������ � ������ At
//          Start_Cmd = 2;
//          Mod_At_cnt=0; // ����� �������� ������� �������
          //NumIzm++; // ����� ���������
        }

      break;
    }
    break;
    //____________________________________________________________________________________________________
  case 0x40: // ������, �����
    switch(CondWork)
    {
    case P1: // ������ ���������� ������������ �� ������ ���� ����������
      // ������� ������������� ����� ����
      // � ��������� �������� ������ ���������������
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght-5,0);//������� ����� �����. � ���������� ���������
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case RE: // ������� ���� ����������� ������ ������ 0-OFFs, 1-SCW, 2-S27W(1Hz),
      switch(ReMode)
      {
      case OFFs:
        ReMode=S27W;
        break;
      case SCW:
        ReMode=OFFs;
        ControlRE(0);// ����.RE
        break;
      case S27W:
        ReMode=SCW;
        ControlRE(1);// ���.RE
        break;
      default:
        ReMode=OFFs;
        ControlRE(0);// ����.RE
        break;
      }
      break;
    case Sc: // ����������� �������� (��������� ������ � �������� ����� )
      switch(ScMode)
      {
      case OFFs:
        ScMode=SCW_P;
        break;
      case SCW:
        ScMode=OFFs;
        break;
      case S27W:
        ScMode=SCW;
        break;
      case S2KW:
        ScMode=S27W;
        break;
      case SCW_P:
        ScMode=S2KW;
        break;
      default:
        ScMode=OFFs;
        break;
      }
              ControlSC (ScMode); //// ���������� ���������� 

      break;  
    case SetFn: // ������� �������
      switch(ConSet)
      {
      case P1:
        if(!DeviceConfig.CfgL) // ��� ���������� ������
        {
          if(N_LS) // ���� ���������
          {
            ConSet = Sc;
          }
          else
          {
            ConSet = P1a;
          }
        }
        else
        {
          ConSet = Rl;
        }
        break;
      case P1a:
        if (1)//!En_Cfg_P1a
        {
          if ((DeviceConfig.CfgRE & 0x1)&&(N_LS == 0)) //���� ������� ���� � ��� ����������
          {
            ConSet = RE; 
          }
          else
          {
            ConSet = P1; 
          }
        }
        //        else
        //        {
        //          Wind_Cfg = 1;
        //          switch (Inv_Bas)
        //          {
        //          case 0x02: // 1.31
        //            if (Lim_LS[0] > -59.5) Lim_LS[0] -=0.5;
        //            else Lim_LS[0] = -60.0; 
        //            LimX = (int)(Lim_LS[0]*2);
        //            Lim_LS[0] = (float)LimX/2;
        //            break;
        //          case 0x08: // 1.49
        //            if (Lim_LS[1] > -59.5) Lim_LS[1] -=0.5;
        //            else Lim_LS[1] = -60.0; 
        //            LimX = (int)(Lim_LS[1]*2);
        //            Lim_LS[1] = (float)LimX/2;
        //            break;
        //          case 0x20: //1.55
        //            if (Lim_LS[2] > -59.5) Lim_LS[2] -=0.5;
        //            else Lim_LS[2] = -60.0; 
        //            LimX = (int)(Lim_LS[2]*2);
        //            Lim_LS[2] = (float)LimX/2;
        //            break;
        //          default: 
        //            break;
        //          }
        //          Show_Wind_Cfg(Wind_Cfg);
        //        }
        break;
      case RE:
        if (N_LS == 0) // ������ ����������
        {
          ConSet = P1; 
        }
        else //  ��������
        {
          if (!DeviceConfig.CfgPM) // 0- �������� ��� ����������
          {
            if (!DeviceConfig.CfgL) // ������� ���������� ������ ���
            {
              ConSet = Sc;
            }
            else
            {
              ConSet = Rl;
            }
          }
          else // 3- ���������� � ��������
          {
            ConSet = P1a; 
          }
        }
        break;
      case Sc: // ��������� ���� ������� ���� � ������ ��������
        if (DeviceConfig.CfgRE & 0x1) //���� ������� ���� 
        {
          ConSet = RE; 
        }
        else //  ��������
        {
          if (!DeviceConfig.CfgPM) // 0- �������� ��� ����������
          {
            if (!DeviceConfig.CfgL) // ������� ���������� ������ ���
            {
              ConSet = Sc;
            }
            else
            {
              ConSet = Rl;
            }
          }
          else // 3- ���������� � ��������
          {
            ConSet = P1a; 
          }
        }
        break;
      case P2:
        ConSet = At; 
        break;
      case At:
        ConSet = Sc; 
        break;
      case Ln:
        ConSet = P2;
        break;
      case Rl:
        ConSet = Ln;
        break;
      case Ls:
        break;
        
      }
      ShowChooseMode();
      NeedReDraw=1;
      
      break; // sw SetFn
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sTime:
      if(IndxEditClock>0)IndxEditClock--;
      else IndxEditClock = 6;
            break;
      case sNumF:
      if(IndxEditNumFbr<5)IndxEditNumFbr++;
      else IndxEditNumFbr = 0;
            break;
      case sLOSSm:
          if((UserMeasConfig.Lim_LS[IndxEditLOSSm]+0.5)>-100.0 ) UserMeasConfig.Lim_LS[IndxEditLOSSm] -=0.5;
          else UserMeasConfig.Lim_LS[IndxEditLOSSm]=10.;
        break;
      case sLOSS:
        //if(IndxEditLOSS)
        {
          if((UserMeasConfig.Lim_LS[0]+0.5)>-100.0 ) UserMeasConfig.Lim_LS[0] -=0.5;
          else UserMeasConfig.Lim_LS[0]=10.;
        }
        break;
      case sORL:
        //if(IndxEditORL)
        {
          if((UserMeasConfig.Lim_ORL[0])>.5 ) UserMeasConfig.Lim_ORL[0] -=0.5;
          else UserMeasConfig.Lim_ORL[0]=60.;
        }
        break;
      case sSCp:
        if(IndxPowSC)
        {
          if((SettingPrm.PowSc[IndxPowSC-1])>-9.99 ) SettingPrm.PowSc[IndxPowSC-1] -=0.01;
          else SettingPrm.PowSc[IndxPowSC-1]=5.;
        }
        break;
      }
            NeedReDraw=1;
    break;      
    }
    break; // sw Left
    //____________________________________________________________________________________________
  case 0x08: // Up
    switch (CondWork)
    {
    case P1: // ������ ���������� ������������ �� ������ ���� ����������
      // ������� ������������� ����� ����
      // � ��������� �������� ������ ���������������
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght,1);//������� ����� �����. � ���������� ���������
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case P2: // ����������� ����� ����� � ����������������� ������� ������� (�������) - ���� ������ ������ ������ �������� �� ����� ����� ����������
      if (IndxP2LW<(WAVE_LENGTHS_NUM -1)) IndxP2LW++;
      else IndxP2LW = 0 ;
      IN_BuffADC_one = (IN_BuffADC[1]>ADC_Calibr)?(IN_BuffADC[1]-ADC_Calibr):(IN_BuffADC[1]);
      memset(BuffP2,IN_BuffADC_one, ARRAY_SIZE(BuffP2)); // ����� ����������� ������
      //memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // ����� ����������� ������
      NeedReDraw = 1;
      break;
      case Sc: // Source place switch
        // ����������� ������
        GetSetLW_SC (1, 0) ; // ��������� ������ ���������� ��������� ��� ���������
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
        indx_LW_ORL = Ind_LW ; // ��������� ������ ���������� ��������� ��� ���������

//         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;

         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������ ������ ����� ����� ��� ��������� � ������ ������������� ���� ����
        break;
      case Rl: // Source place switch
        // ����������� ������
        GetSetLW_SC (1, 1) ; // ��������� ������ ���������� ��������� ��� ���������
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
        indx_LW_ORL = Ind_LW ; // ��������� ������ ���������� ��������� ��� ���������

//         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;

         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������ ������ ����� ����� ��� ��������� � ������ ������������� ���� ����
        break;
        
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sNumF:
        if(IndxEditNumFbr)
        {
          Vel= (uint32_t)(pow(10,IndxEditNumFbr-1));
         dCh=(SettingPrm.NumberFbrCount/Vel)%10;
         SettingPrm.NumberFbrCount-=(uint16_t)(dCh*Vel);
         if(dCh<((IndxEditNumFbr<5)?(9):(5))) dCh++;
         else dCh = 0;
         SettingPrm.NumberFbrCount+=(uint16_t)(dCh*Vel);
        }
        break;
      case sTime:
      switch(IndxEditClock)
      {
      case 1: //Year
        if(Clocks.cYear<43) Clocks.cYear++;
        else  Clocks.cYear=21;
        break;
      case 2: //Month
        if(Clocks.cMonth<12) Clocks.cMonth++;
        else  Clocks.cMonth=1;
        break;
      case 3: //Day
        switch(Clocks.cMonth)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          DayMax = 31;
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          DayMax = 30;
          break;
        case 2:
          if(Clocks.cYear%4) DayMax = 28;
          else DayMax = 29;
          break;
        }
        if(Clocks.cDay<DayMax) Clocks.cDay++;
        else  Clocks.cDay=1;
        break;
      case 4: //Hour
        if(Clocks.cHour<23)Clocks.cHour++;
        else Clocks.cHour=0;
        break;
      case 5: //Minutes
        if(Clocks.cMinutes<59)Clocks.cMinutes++;
        else Clocks.cMinutes=0;
        break;
      case 6: //Seconds
        if(Clocks.cSeconds<59)Clocks.cSeconds++;
        else Clocks.cSeconds=0;
        break;
      }
      break;
      case sLOSSm:
      if(IndxEditLOSSm==0)IndxEditLOSSm=1;
      else IndxEditLOSSm = 0;
        break;
      case sLOSS:
      //if(IndxEditLOSS>0)IndxEditLOSS--;
      //else IndxEditLOSS = 3;
        break;
      case sORL:
      //if(IndxEditORL>0)IndxEditORL--;
      //else IndxEditORL = 3;
        break;
      case sSWm:
      if(IndxEnaSave>0)IndxEnaSave--;
      else IndxEnaSave = 3;
        break;
      case sSCp:
      if(IndxPowSC>0)IndxPowSC--;
      else IndxPowSC = 3;
        break;
      }
      
      NeedReDraw = 1;
      break;
    }
    //    if(Range<3) Range++;
    //    else Range = 0;
    //    SetRange(Range);
    //    //
    //    if(Ind_LW<2) Ind_LW++;
    //    else Ind_LW = 0;
    //    SWITCH_LW();
    //    ON_GEN(1);
    //    LD_R(0);
    break;
    //_____________________________________________________________________________________
  case 0x20: // Down
    switch (CondWork)
    {
    case P1: // ������ ���������� ������������ �� ������ ���� ����������
      // ������� ������������� ����� ����
      // � ��������� �������� ������ ���������������
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght,-1);//������� ����� �����. � ���������� ���������
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case P2: // ����������� ����� ����� � ����������������� ������� ������� (�������) - ���� ������ ������ ������ �������� �� ����� ����� ����������
      if (IndxP2LW>0) IndxP2LW--;
      else IndxP2LW = WAVE_LENGTHS_NUM -1 ;
      IN_BuffADC_one = (IN_BuffADC[1]>ADC_Calibr)?(IN_BuffADC[1]-ADC_Calibr):(IN_BuffADC[1]);
      memset(BuffP2,IN_BuffADC_one, ARRAY_SIZE(BuffP2)); // ����� ����������� ������
      //memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // ����� ����������� ������
      NeedReDraw = 1;
      break;
      case Sc: // Source place switch
        // ����������� ������
        GetSetLW_SC (1, 0) ; // ��������� ������ ���������� ��������� ��� ���������
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
        indx_LW_ORL = Ind_LW ; // ��������� ������ ���������� ��������� ��� ���������

//         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;

         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������ ������ ����� ����� ��� ��������� � ������ ������������� ���� ����
        break;
      case Rl: // Source place switch
        // ����������� ������
        GetSetLW_SC (1, 1) ;// ��������� ������ ���������� ��������� ��� ��������� Ind_LW
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
        indx_LW_ORL = Ind_LW ;// ��������� ������ ���������� ��������� ��� ��������� � P2

        //         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������ ������ ����� ����� ��� ��������� � ������ ������������� ���� ����
        break;
    case SetPrm:
      // ������������� �� ����������
      switch(SetupMod)
      {
      case sNumF:
        if(IndxEditNumFbr)
        {
          Vel= (uint32_t)(pow(10,IndxEditNumFbr-1));
         dCh=(SettingPrm.NumberFbrCount/Vel)%10;
         SettingPrm.NumberFbrCount-=(uint16_t)(dCh*Vel);
         if(dCh>0) dCh--;
         else dCh =(IndxEditNumFbr<5)?(9):(5);
         SettingPrm.NumberFbrCount+=(uint16_t)(dCh*Vel);
        }
        break;
      case sTime:
      switch(IndxEditClock)
      {
      case 1: //Year
        if(Clocks.cYear>22) Clocks.cYear--;
        else  Clocks.cYear=43;
        break;
      case 2: //Month
        if(Clocks.cMonth>1) Clocks.cMonth--;
        else  Clocks.cMonth=12;
        break;
      case 3: //Day
        switch(Clocks.cMonth)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
          DayMax = 31;
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          DayMax = 30;
          break;
        case 2:
          if(Clocks.cYear%4) DayMax = 28;
          else DayMax = 29;
          break;
        }
        if(Clocks.cDay>1) Clocks.cDay--;
        else  Clocks.cDay=DayMax;
        break;
      case 4: //Hour
        if(Clocks.cHour>0)Clocks.cHour--;
        else Clocks.cHour=23;
        break;
      case 5: //Minutes
        if(Clocks.cMinutes>0)Clocks.cMinutes--;
        else Clocks.cMinutes=59;
        break;
      case 6: //Seconds
        if(Clocks.cSeconds>0)Clocks.cSeconds--;
        else Clocks.cSeconds=59;
        break;
      }
      break;
            case sLOSSm:
      if(IndxEditLOSSm)IndxEditLOSSm=0;
      else IndxEditLOSSm = 1;
        break;
            case sLOSS:
      //if(IndxEditLOSS<3)IndxEditLOSS++;
      //else IndxEditLOSS = 0;
        break;
            case sORL:
      //if(IndxEditORL<3)IndxEditORL++;
      //else IndxEditORL = 0;
        break;
      case sSWm:
      if(IndxEnaSave<3)IndxEnaSave++;
      else IndxEnaSave = 0;
        break;
      case sSCp:
      if(IndxPowSC<3)IndxPowSC++;
      else IndxPowSC = 0;
        break;
      case sMem:
      if(IndxClrMEM)IndxClrMEM=0;
      else IndxClrMEM = 1;
        break;
      }
      NeedReDraw = 1;
      break;
    }
    break;
    //____________________________________________________________________________________
  case 0x10:   //������ (������������ �������) Fn (MODE) (<_->)
    if (CondWork != SetFn)
    {
      OldCWork = CondWork; // ���������� ����� � ������ ������� ��������������
      ConSet = CondWork; // ������������� ���� ����� � ���������� �������������
      CondWork = SetFn; // ������������� ������� ����� ��� ����� �������
      SW_RS232(1); // ������ ���������� ���������� ������� � ��������� (AD7782) (� ������� �������� ������ � ��������)

      if (ModeI != dBm) ModeI=dBm;
      SSD1305_Fill(0); /* ������� ������*/
      switch (ConSet)
      {
      case P1:
      case P1a:
        if (ModeI == Hold)
        {
          ModeI=OldMode;// ����� �� ������ HOLD
        }
        OldMode = ModeI;
        EnaOpticUART=0;
        break;
      case RE:
      case Sc:
        break;
      case P2:
      case Ln:
      case Rl:
      case Ls:
      case At:
        // �������� ���� ��������� ��������
        ControlSC (0); // ���������� ���������� -  ���������
        
        break;
      case SetPrm: // ����� �� ������ ��������� �������� ���� ���-�� ���������
        switch(SetupMod)
        {
        case sTime:
        // ��������� ������������� ����� 
        //RTC_WriteTimeCounter(&hrtc,TotalSec()); // for F103
        ReSetRegRTC(1,&hrtc);// ������� ����� ���� F303

        break;
        case sNumF:
        NeedSaveChange |=0x10; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
          break;
        case sLOSSm:
        case sLOSS:
        case sORL:
        case sSWm:
        case sSCp:
        NeedSaveChange |=0x11; // ��������� ���������������� ��������� (����� ������� � ������� ���� ����������)
          break;
        }
        CondWork = SetFn; // ������� � ����� ������ �������
        if(DeviceConfig.CfgPM)ConSet=P1; 
        else ConSet=Sc;
        
        break;
      }
      
      //DrawMod (CondWork);
      //ShowBatLvl(BatLevel);
      ShowChooseMode();
      SW5V(0);// OFF 5V

      NeedReDraw=1;
    }
    else // ����� ������ ��� �������� ������� 
      // ������������ ��������� ������� � ����� ������
      // ������������ �� ����� ��� ������ <> � ����� ������
    {
      SetNeedCondWork (ConSet);
      
//////      if(OldCWork==Rl)
//////      {
//////        ScMode = OFFs; // ���������� ���������� -  ��������� ������ ��� ���� � ORL ()
//////      }
//////      switch(ConSet)
//////      {
//////      case P1:
//////        CondWork = P1;
//////        SetModePM(1); // Auto ����� P1 -
//////        break;
//////      case P1a:
//////        CondWork = P1a;
//////        for (int i=0; i<ARRAY_SIZE(RPON); i++) RPON[i]=-100.0;
//////        //SW_RS232(1); // ������ ���������� ���������� ������� � ��������� (AD7782)- ��������� � ������������ �������)
//////        P1aWavelenght=0;  // ������� ����� ����� P1a - �������
//////        // ������������� ������ ����� � ���� �������� ������ , ������� �����, � ������� � ���������� ���� ����� �� ���������
//////        CntWaitLW = 0; // ����� �������� �������� ������ ����� �����
//////        SetModePM(0); // ������ ����� P1 -
//////        SetRange(1); // ���� �������������� ��������
//////        EnaOpticUART=1;
//////        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
//////        YesLW=0;
//////        
//////        break;
//////      case Sc:
//////        CondWork = Sc;
//////        //ControlSC (ScMode); // ���������� ���������� - ���. ��� ����� � ������ ��� ��������
//////        //ShowSource();
//////        break;
//////      case RE:
//////        CondWork = RE;
//////        //ShowSource();
//////        break;
//////      case Rl:
//////        CondWork = Rl;
//////        //SWITCH_LW(); // ��������� ��������� ����� ����� ���������
//////         ScMode = SCW; // ���������� ���������� - ���. �� ����������
//////         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������ ������ ����� ����� ��� ��������� � ������ ������������� ���� ����
//////         IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
//////         indx_LW_ORL = Ind_LW; // // ��������� ������ ���������� ��������� ��� ���������
//////         PowClbr_mW = CoeffPM.Pow_mW_Clbr;
//////        //ShowSource();
//////        break;
//////      case Ln:
//////        CondWork = Ln;
//////        ScMode = OFFs; // ���������� ���������� -  ���������
//////        //SW5V(1);// ON 5V ������� ����� ����� �������� �����
//////        //ShowSource();
//////        break;
//////      case P2:
//////        CondWork = P2;
//////        ScMode = OFFs; // ���������� ���������� -  ���������
//////        //ShowSource();
//////        break;
//////      case At:
//////        // ���������� �� ������ ���������� �����
//////        CalibLnAuto = UserMeasConfig.CalibLnAutoMem;
//////        CondWork = At;
//////        Mod_At = Wt; // ������������ � ����� 
//////        ScMode = OFFs; // ���������� ���������� -  ���������
//////        SW_RS232(0); // ������ ���������� ���������� ������� � ��������� (AD8304)
//////        // ������ ����� �������� �������
//////        GetNumActivLS();
//////        huart2.Init.BaudRate = 1200;
//////        if (HAL_UART_Init(&huart2) != HAL_OK)
//////        {
//////          Error_Handler();
//////        }
//////        HAL_UART_DMAStop(&huart2);
//////        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
//////        Dummy = huart2.Instance->DR ; // ������ ������ ������ 
//////
//////
//////        FirstInput = 1;// ������ ��� ����� 
//////
//////        //ShowSource();
//////        break;
//////      default:
//////        // ���� �������� � ����������
//////        CondWork = P1;
//////        ScMode = OFFs; // ���������� ���������� -  ���������
//////        break; 
//////      }
//////        ControlSC (ScMode); // ���������� ���������� -  ������������� �������� ������
//////        
//////      NeedReDraw=1;
      //      CondWork = ConSet;
      //      /*
      //      if ((CondWork == P1a)||(CondWork == P2)) 
      //      {
      //      UCA0BR0 = 0x1B;   //�������� �������� 2400=0x0d  1200=0x1B
      //      UCA0BR1 = 0x00;   //~1200 ��� ��� P2
      //      UCA0MCTL = 0x04;  //��� ������ 32768 2400=0x6b 1200=0x04
      //    }
      //        else
      //      {
      //      UCA0BR0 = 0x06;   //�������� ��������
      //      UCA0BR1 = 0x00;   //~4800 ���
      //      UCA0MCTL = 0x0E;  //��� ������ 32768
      //      
      //    }
      //      */
      //      //Str1[0]= ' ';
      //      //Str1[1]= ' ';
      //      Displ = 0;
      //      ShowMsg(6); 
      //      ShowMsg(9);
      //      ShowMsg(25);
      //      ShowMsg(30); // ������� ���� ������� �� ����������
      //      ShowMsg(39); // ������� 4 ������
      //      //Paint();
      //      Displ = 2;
      //      Str1[0]='0'+CondWork; // ������ ��������� �������
      //      ShowMsg(32);
      //      // ����� ��������� ����� ����� � ����������� �� �������
      //      switch (CondWork)
      //      {
      //      case Ln:
      //        ScMode = OFF;
      //        Ind_LW = 1; // 850 nm
      //        P3OUT |=sw_L; // 1 - �� ����� P3.2 
      //        P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //        break;
      //      case Rl: // ��� ���������� �����  ��� �������� �������� ����� 
      //      case Ls: // ����� �� ��� �� ����� ����������� ��� 
      //        ScMode = OFF;
      //        switch (DataDevice[19]-'0')
      //        {
      //        case 1:
      //          Ind_LW = 1; // 850 nm
      //          P3OUT |=sw_L; // 1 - �� ����� P3.2 
      //          P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //          break;
      //        case 2:
      //          Ind_LW = 1; // 1310 nm
      //          P3OUT |=sw_L; // 1 - �� ����� P3.2 
      //          P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //          break;
      //        case 3:
      //          Ind_LW = 1; // 1550 nm
      //          P3OUT |=sw_L; // 0 - �� ����� P3.2 // ������� � �� �� ����� ��� � 1310
      //          P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //          break;
      //        case 4:
      //          Ind_LW = 1; // 850 nm
      //          P3OUT |=sw_L; // 1 - �� ����� P3.2 
      //          P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //          break;
      //        case 5:
      //          Ind_LW = 0; // 1310 nm
      //          P3OUT &=~sw_L; // 0 - �� ����� P3.2 
      //          P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //          break;
      //        case 6:
      //        case 7:
      //          Ind_LW = 0; // 1310 nm
      //          P3OUT &=~sw_L; // 0 - �� ����� P3.2 
      //          P6OUT &=~sw_H; // 0 - �� ����� P6.3  ���������� ADG804        
      //          //           P2OUT &=~LD_R; // 0 - �� ����� P2.7 off 1.49 
      //          break;
      //        default:
      //          break;
      //          
      //        }
      //        break;
      //      }
      //      sprintf(Str7, "       ");
      //      
      //      switch (CondWork)
      //      {
      //      case RE:
      //        if (ReMode == OFF)
      //        {
      //          sprintf(Str6, "@ OFF   ");
      //          sc_on = 0;
      //          //P3OUT &=~CW;
      //          P2OUT &=~LD_R; // 0 - �� ����� P2.7 
      //        }
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        
      //        sprintf(Str7, " VFL ");
      //        ShowMsg(25);
      //        ShowMsg(22);
      //        break;
      //      case Sc:
      //        if (ScMode == OFF)
      //        {
      //          sprintf(Str6, "@ OFF   ");
      //          sc_on = 0;
      //          P3OUT &=~CW;
      //          //            P2OUT &=~LD_R; // 0 - �� ����� P2.7 off 1.49 
      //        }
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        
      //        sprintf(Str7, "%4.2f ",PwS[Ind_LW]);
      //        ShowMsg(25);
      //        ShowMsg(22);
      //        break;
      //      case P1a: // �������� ����� ���������� � �������
      //        UC1IE |=UCA0RXIE;//��������� USART1 RX  ����������
      //        
      //        MD_P1A = 2; // ������ ��������� � ��������
      //        Ind_PON = 0; // 1310
      //        Range = 2;
      //        SetRange();
      //        EQ_LW = 0; 
      //        CH_Po = 0;
      //        Pow_Yes = 0;
      //        N_PON = 0; // ����� ��������� � ������ �������
      //        if ((OldCWork == Ls) || (OldCWork == Rl))
      //        {
      //          sc_on = 0;
      //          P3OUT &=~CW; 
      //          //            P2OUT &=~LD_R;
      //        }
      //        //Mode = OldMode; // ���� ���������
      //        Mode = dB; // ������������� dB
      //        ShowMsg_PON(128);
      //        Ini_ind_PON();
      //        break;
      //      case P1: // �������� ����� ����������
      //        //if (CondWork == P1a) CH_Po = 0;
      //        if ((OldCWork == Ls) || (OldCWork == Rl))
      //        {
      //          sc_on = 0;
      //          P3OUT &=~CW; 
      //          //            P2OUT &=~LD_R;// off 1.49
      //        }
      //        Mode = OldMode; // ���� ���������
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        Displ = 2;
      //        Str1[0]='0'+CondWork; // ������ ��������� �������
      //        ShowMsg(32);
      //        sprintf(Str7, "%4.2f ",BaseLevel);
      //        ShowMsg(25);
      //        break;
      //      case Rl:
      //        sc_on = 1;
      //        P3OUT |= CW;
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        sprintf(Str7, "%4.2f ",BaseRet[Ind_LW]);
      //        ShowMsg(25);
      //        break;
      //      case Ls:
      //        sc_on = 1;
      //        P3OUT |= CW; 
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        sprintf(Str7, "%4.2f ",BaseLoss[Ind_LW]);
      //        ShowMsg(25);
      //        break;
      //      case P2:
      //        ScMode = OFF;
      //        sc_on = 0;
      //        P3OUT &=~CW; 
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        sprintf(Str7, "%4.2f ",BaseLevelP2);
      //        ShowMsg(25);
      //        break;
      //      case Ln:
      //        sc_on = 0;
      //        P3OUT &=~CW; 
      //        //P2OUT &=~LD_R;
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        break;
      //      case At:// ����� ��������������� ���������
      //        ScMode = OFF;
      //        P6OUT &=~sw_RS; // ����������� ���� RS1k2
      //        sc_on = 0;
      //        P3OUT &=~CW; // �������� �������� 
      //        sprintf(Str4, "   ��������   ");
      //        sprintf(Str5, "    ������    ");
      //        ShowMsg(30);
      //        sprintf(Str7, "�%03d  ",NumIzm); // ��������� ������ ������
      //        ShowMsg(25); // ����� �� ����� �������� ������
      //        Mod_At = Wt; // �� - ����� �������� ������
      //        Mod_At_cnt = MaxAT; // ���� ��� ������������ ���� ��������� � ��������
      //        Reciev1k2 = Stop; // ���� ���� "�������" � End �� ������ � ���� .. 05.07.2010
      //        UC1IE |=UCA0RXIE;//��������� USART1 RX  ����������
      //        break;
      //      default:
      //        break;
      //      }
      //      if (CondWork != P1a)Num2Lenght();
      //      //Paint();
      //      OldCWork = CondWork;
    }
    
    //Paint();
    // �������� ������������ ������� �������        
    break; // CASE 0x10
    //__________________________________________________________________________________________
  case 0x7B: // ������ "S" - ������ �������! 
    // � ������ �������� ������� ������� ��������� ������ ����������, �����, 
           switch (CondWork)
           {
           case SetFn:
             // �������� ���� �� ���������� � �� ��� �� ������ ��� ��������
             if(((ConSet==P1)&&(DeviceConfig.CfgPM))||((!(DeviceConfig.CfgPM))&&(ConSet==Sc))) 
             {
               CondWork = SetPrm;
               SetupMod = sTime;
               NeedReDraw = 1;
               IndxEditClock = 0; 
             }
             break;
           case P1: // ������ ������ ������
           case P1a: // ������ ������ ������
           case P2: // ���������� ����������� ���������
           case Rl: // ���������� ����������� ���������
           case Ln: // ���������� ����������� ���������
             if(SettingPrm.CountMemory<512)
             {
               WriteBlockMEM (1); // ������ ����� ������ � ������, �������� �������� ��� UART EXT

             }
             CountDrawMsg = 7;
             break;
           case At: // ���������� ����������� ���������
             if(Mod_At == View)
             {
             if(SettingPrm.CountMemory<512)
             {
               WriteBlockMEM (1); // ������ ����� ������ � ������, �������� �������� ��� UART EXT
             }
               Mod_At = Wt;
               FirstInput=1;
               NeedReDraw = 1;
             }
             
             break;
           }
    break; // CASE 0x7B
    //_________________________________________________________________________________________
  case 0x7E: // ������ "dBm/dB/W" - ������ �������!
       switch (CondWork)
       {
       case P1:
         CurrBaseLevel = Opt_dBm; // UserMeasConfig.BaseLvl[j];
         ModeI = dB;
         // ��������� � ������ ������ �������� ������ ��� �������� ��� P1
         SetNewBaseLevel ();
         ShowPM(ModeI, PMWavelenght);
         NeedReDraw = 1;
         break;
       case P1a:
         for(int i=0; i < ARRAY_SIZE(RPON); ++i) // ��������� ������ RPON �� ������� ��������
         {
           if (RPON[i]>-100.0) // ���� �������� ��� ���������
           {
             UserMeasConfig.BaseLvl[i]=RPON[i];
           }
         }
         ModeIP1a = dB;
         NeedReDraw = 1;
         break;
       case P2:
         UserMeasConfig.BaseLvlP2[IndxP2LW]=P2dBmValue;
         P2Mode = dB;
         NeedReDraw = 1;
         break;
       case Rl: //ORL
         // ������ ������� ���. ���� ������������� "�����" ����� �������
         if(P2dBmValue < LIM_ORL_CORR_NOISE) // ������ ����� ������������
         {
           if(PowClbr_mW != CoeffPM.Pow_mW_Clbr)
         PowClbr_mW = CoeffPM.Pow_mW_Clbr;
         else
         PowClbr_mW = MedP2mW;
         NeedReDraw = 1;
         }
         // �������� �� ���. ���� ������������� "�� �����" 14.7 ��
         if((ORLCalc>LIM_ORL_UP)&&(ORLCalc<LIM_ORL_DN))
         {
    if( BaseRetCurrORL[indx_LW_ORL] != CoeffPM.BaseRet[indx_LW_ORL])
      // ���� ���� �������������� ���������� � ����������
         BaseRetCurrORL[indx_LW_ORL] = CoeffPM.BaseRet[indx_LW_ORL];
     else 
     { // ���� ����������� ��� 14.7
       BaseRetCurrORL[indx_LW_ORL] = CoeffPM.BaseRet[indx_LW_ORL] + (14.7 - ORLCalc);
     }    
         NeedReDraw = 1;
         }
         break;
       } 
         NeedSaveChange |=0x01; // ��������� ���������������� ��������� (������� ������)
        break;
//  case 0xXX: // ������ "" - ������ �������!
//       switch (CondWork)
//        {
//        case P1:
//          
//          break;
//          
//    } 
//        break;


  } 
  //ShowPM(ModeI, PMWavelenght);
  
}

uint8_t SetNeedCondWork (uint8_t SetConditionWork)
{
       if(OldCWork==Rl)
      {
        ScMode = OFFs; // ���������� ���������� -  ��������� ������ ��� ���� � ORL ()
      }
      if(OldCWork == P1a) // ������� ��� ���������� UartOptic � ������� �������� ������
      {
        CntWaitLW = 6000; // ������������� �������� �������� ������ ����� �����
        EnaOpticUART=0;
        SetModePM(1); // ������� � �������������� ����� P1 -
      }
      switch(SetConditionWork)
      {
      case P1:
        CondWork = P1;
        SetModePM(1); // Auto ����� P1 -
        break;
      case P1a:
        CondWork = P1a;
        for (int i=0; i<ARRAY_SIZE(RPON); i++) RPON[i]=-100.0;
        //SW_RS232(1); // ������ ���������� ���������� ������� � ��������� (AD7782)- ��������� � ������������ �������)
        P1aWavelenght=0;  // ������� ����� ����� P1a - �������
        // ������������� ������ ����� � ���� �������� ������ , ������� �����, � ������� � ���������� ���� ����� �� ���������
        CntWaitLW = 0; // ����� �������� �������� ������ ����� �����
        SetModePM(0); // ������ ����� P1 -
        SetRange(1); // ���� �������������� ��������
        EnaOpticUART=1;
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        HAL_UART_DMAStop(&huart2); // ������� ���� ������ ���? ����� ��� ����������� � ���� ...
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
        YesLW=0;
        break;
      case Sc:
        CondWork = Sc;
        //ControlSC (ScMode); // ���������� ���������� - ���. ��� ����� � ������ ��� ��������
        //ShowSource();
        break;
      case RE:
        CondWork = RE;
        //ShowSource();
        break;
      case Rl:
        CondWork = Rl;
        //SWITCH_LW(); // ��������� ��������� ����� ����� ���������
         ScMode = SCW; // ���������� ���������� - ���. �� ����������
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������ ������ ����� ����� ��� ��������� � ������ ������������� ���� ����
         IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // ������� ������ ������������ P2, ��� ���������� ������ ORL
         indx_LW_ORL = Ind_LW; // // ��������� ������ ���������� ��������� ��� ���������
         PowClbr_mW = CoeffPM.Pow_mW_Clbr;
         // ����������� ������ ��������� �������� ORL �� ������� ���������
         for(int i=0; i<3; i++)
         BaseRetCurrORL[i] = CoeffPM.BaseRet[i];

        //ShowSource();
        break;
      case Ln:
        CondWork = Ln;
        ScMode = OFFs; // ���������� ���������� -  ���������
        //SW5V(1);// ON 5V ������� ����� ����� �������� �����
        //ShowSource();
        break;
      case P2:
        CondWork = P2;
        ScMode = OFFs; // ���������� ���������� -  ���������
        //ShowSource();
        break;
      case At:
        // ���������� �� ������ ���������� �����
        CalibLnAuto = UserMeasConfig.CalibLnAutoMem;
        CondWork = At;
        Mod_At = Wt; // ������������ � ����� 
        ScMode = OFFs; // ���������� ���������� -  ���������
        SW_RS232(0); // ������ ���������� ���������� ������� � ��������� (AD8304)
        // ������ ����� �������� �������
        GetNumActivLS();
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ���� �������� ������� ����� �� �������� ����
        //Dummy = huart2.Instance->DR ; // ������ ������ ������ 
        Dummy = Clr_UART; // ������ ������ ������ 


        FirstInput = 1;// ������ ��� ����� 

        //ShowSource();
        break;
      default:
        // ���� �������� � ����������
        CondWork = P1;
        ScMode = OFFs; // ���������� ���������� -  ���������
        break; 
      }
        ControlSC (ScMode); // ���������� ���������� -  ������������� �������� ������
        
      NeedReDraw=1;
 return CondWork;
}

uint16_t WriteBlockMEM (uint8_t Dir) // ������ ����� ������ � ������, �������� �������� ��� UART EXT
             {
               uint8_t yesLS;
               uint16_t EnaWrite=0;
               uint16_t AdrWriteMem=(uint16_t)(SettingPrm.CountMemory<<6) + ADR_SaveMemory; 
               if((SettingPrm.CountMemory%4)==0) // ������ ����� (������ ����)256 byte
               {
                 memcpy(&MemBlock, 0, sizeof(MemBlock));
                 EEPROM_write(&MemBlock, AdrWriteMem , sizeof(MemBlock));
               }
               // ��������-�������� ���� �� ���������
               MemoryData.mTime = ReSetRegRTC(0, &hrtc); // ����� ������
               MemoryData.mNumFiber=(SettingPrm.EnaAutoInkrNF)?(++SettingPrm.NumberFbrCount):(SettingPrm.NumberFbrCount);// ����� �������
               MemoryData.mCode=0; // ��� ������ ��� �����������
               for(int j=0;j<3;j++)
               {
                 for(int k=0;k<4;k++)
                   MemoryData.mRes[j][k]=0;
                 MemoryData.mLW[j]=0;// ����� ����� ������ ������
               }
               MemoryData.mLen=0; // ����� �������
               MemoryData.mFree=0; // ������ 
               
               switch(CondWork)
               {
               case P1: 
                 // ��������� ���� ������
                 MemoryData.mCode=CondWork*10+ModeI; // ��� ������ ��� �����������
                 MemoryData.mLW[0]=PMWavelenght; // ����� ����� ������ ������
                 MemoryData.mRes[0][0]=GetPMValue()*CurrSpectrKoeff; // �������� � ���
                 MemoryData.mRes[0][1]=GetPMValuedBm(MemoryData.mRes[0][0]); // �������� � ���
                 MemoryData.mRes[0][2]=GetPMValuedB(MemoryData.mRes[0][0]); // �������� � ��
                 MemoryData.mRes[0][3]=CurrBaseLevel; // ������� �������
                 EnaWrite=1;
                 break;
               case P1a:
                 MemoryData.mCode=CondWork*10+ModeIP1a; // ��� ������ ��� �����������
                 CountPON=0;
                 for (int i=0; i<WAVE_LENGTHS_NUM; i++) // �������� �� ��������� PON
                 {
                   if(RPON[i]>-100.) 
                   {
                 MemoryData.mLW[CountPON]=CalibrLW[i]; // ����� ����� ������ ������
                 MemoryData.mRes[CountPON][0]=RPON[i]; // �������� � dBm
                 MemoryData.mRes[CountPON][1]=RPON[i]-UserMeasConfig.BaseLvl[i]; // �������� � ��
                 MemoryData.mRes[CountPON][2]=UserMeasConfig.BaseLvl[i]; // �������� �������� ������ ��� ���� ����� �����
                 MemoryData.mRes[CountPON][3]=BaseExtLvl[i]; // ������� �������
                    CountPON++; 
                   }
                   if(CountPON==3)
                     break;
                 }
                 EnaWrite=1;
                 break;
               case P2: 
                 MemoryData.mCode=CondWork*10+P2Mode; // ��� ������ ��� �����������
                 MemoryData.mLW[0]=CalibrLW[IndxP2LW]; // ����� ����� 
                 MemoryData.mRes[0][0]=P2dBValue; // �������� � ��
                 MemoryData.mRes[0][1]=P2dBmValue; // �������� � ���
                 MemoryData.mRes[0][2]=UserMeasConfig.BaseLvlP2[IndxP2LW]; // 
                 EnaWrite=1;
                 break;
               case Ln: 
                 MemoryData.mCode=CondWork*10; // ��� ������ ��� �����������
                 MemoryData.mLW[0]=DeviceConfig.PlaceLS[1]; // ����� ����� ��������� ������� �����
                 MemoryData.mLen=MeasLenght; // ����� �������
                 EnaWrite=1;
                  break;
               case Rl:
                 MemoryData.mCode=CondWork*10; // ��� ������ ��� �����������
                 MemoryData.mRes[0][0]=ORLCalc; // �������� � ��
                 MemoryData.mLW[0]=CalibrLW[IndxP2LW]; // ����� ����� 
                 EnaWrite=1;
                 
                  break;
               case At:
                 // ������ ����������� ��������� � ������
                MemoryData.mCode=CondWork*10; // ��� ������ ��� �����������
                 CountPON=0;
                 for (int i=0; i<WAVE_LENGTHS_NUM; i++) // �������� �� ��������� ������ ���� ����
                 {
                   yesLS=0;
                   for(int x=0;x<3;x++)
                   {
                     if(PonA.LW_AB[x] == CalibrLW[i]) 
                       yesLS=1;
                   }
                   if(yesLS) 
                   {
                 MemoryData.mLW[CountPON]=CalibrLW[i]; // ����� ����� ������ ������
                 MemoryData.mRes[CountPON][0]=PonA.LS_AB[i]; // �������� ��������� � dB � �->B
                 MemoryData.mRes[CountPON][1]=PonA.LS_BA[i]; // �������� ��������� � dB � B->A
                 MemoryData.mRes[CountPON][2]=PonA.ORL_AB[i]; // �������� ORL dB � �->B
                 MemoryData.mRes[CountPON][3]=PonA.ORL_BA[i]; // �������� ORL dB � B->A
                     yesLS=0;
                    CountPON++; 
                   }
                   if(CountPON==3)
                     break;
                 }
                 MemoryData.mLen=PonA.LnA; // ����� �������
                 EnaWrite=1;
                  break;
               default:
                EnaWrite=0;
                 break;
               }
               if(EnaWrite && Dir)
               {
               EEPROM_write(&MemoryData, AdrWriteMem , sizeof(MemoryData));
               SettingPrm.CountMemory++;
               EEPROM_write(&SettingPrm, ADR_SettingPrm, sizeof(SettingPrm));
               }
               return SettingPrm.CountMemory;
             }
