// Функции обработки клавиатуры , алгоритм как MSP430, код клавиатуры и ветвление в зависимости от режима
//                                   долгое нажатие                    
//   on/off   0x08   0х01                  0x77    0x7E  
//
//    0х40    0х20   0х02         0x3F     0x5F    0x7D              
//
//    0х10           0х04         ---      ---     0x7B             
//
//    on/off   Up    dB/dBm/W
//
//    Left    Down   Right 
//
//    Mode            S
//

// Комбинация клавиш обычно долгим нажатием!
// 0x08 + 0x01 
#include "main.h"
      uint16_t ImpZ[4]={6,6,6,6};     

        uint8_t DayMax;
static volatile uint32_t dCh;
static volatile uint32_t Vel;
static volatile uint8_t Indx;
static volatile uint16_t AdrWriteMem; 
uint8_t CountPON=0; // счетчик для подсчета числа ячеек для записи при сканировании ячеек PON
     uint32_t ttt;
uint8_t yesLV;
int i,j;
void KeyBoardAnsver( uint8_t CodeKeys)
{
  
  switch (CodeKeys)
  {
           //_____________________________________________________________________________________________ 
  //case 0x6B: // комбинация клавиш 0x04-0x10 (долгое) включение выключение авто выключения
  case 0x57: // комбинация клавиш 0x08-0x20 вверх-вниз(долгое) включение выключение авто выключения
    if(UserMeasConfig.AutoOff)
      UserMeasConfig.AutoOff=0;
    else
      UserMeasConfig.AutoOff=1;
                    NeedSaveChange |=0x10; // сохраняем пользовательские изменения (Номер волокна и признак авто инкремента)
    myBeep(50);
    HAL_Delay(200);
    myBeep(50);
      
break;
       //_____________________________________________________________________________________________ 
  case 0x76: // комбинация клавиш 0x01-0x08 (долгое) Вызов калибровки в режиме автомат
          switch (CondWork)
          {
          case At: // запуск режима калибровки источников в режиме двухсторонней связи
            if ((Mod_At == Wt)||(Mod_At == View)||(Mod_At == Err_Ans)||(Mod_At == Clb_Err))
            {
              FirstInput = 1;// первый раз вошли 
              Mod_At = SavPw; // вызываем режим калибровки
            }
            break;
          case P1a: // перезапишем источники в сами себя... 
            for(i=0; i < ARRAY_SIZE(RPON); ++i) // переберем ячейки RPON на наличие значений
            {
              for(j=0; j < 3; ++j) // переберем длины волн источника
              {
                if(DeviceConfig.PlaceLS[j] == CalibrLW[i]) // ест совпадение длин волн источка и измерения,
                  // проверим значение, если Ок можно переписать
                {
                  if((RPON[i]>-3.0)&&(RPON[i]<3.0)) // в нужном диапазоне, можно записать
                  {
                    SettingPrm.PowSc[j] = RPON[i];
                    NeedSaveChange |=0x10; // сохраняем пользовательские изменения (Номер волокна и признак авто инкремента)
                    
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
  case 0x72: // комбинация клавиш 0x02-0x20-0x40-0x10 (был вызов Setting в разных режимах)
    myBeep(500);
    HAL_Delay(800);
    myBeep(500);
    break;
    //____________________________________________________________________________________________
  case 0x01: // кнопка "dBm/dB/W"
    switch (CondWork)
    {
    case P1: // Измеритель Р1
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
      case W : // индикация показаний при привязке к внешнему уровню
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
      // переключаемся по установкам
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
          //чистим память
          SettingPrm.CountMemory=0;
          
        }
        IndxClrMEM = 0;
        break;
      }
        NeedSaveChange |=0x10; // сохраняем пользовательские изменения (Номер волокна и признак авто инкремента)
      break;
    }
    break;
    //_______________________________________________________________________________
  case 0x77: // вверх  долго нажата 
    switch(CondWork)
    {
    case SetPrm:
      // переключаемся по установкам
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
  case 0x5F: // вниз  долго нажата 
    switch(CondWork)
    {
    case SetPrm:
      // переключаемся по установкам
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
  case 0x02: // Больше , вправо 
    switch(CondWork)
    {
    case P1: // просто измеритель переключение по длинам волн калибровки
      // включая специфические длины волн
      // и установка базового уровня соответствующая
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght+5,0);//получим новый коэфф. и перестроим указатели
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case RE: // красный глаз переключаем индекс Режима 0-OFFs, 1-SCW, 2-S27W(1Hz),
      switch(ReMode)
      {
      case OFFs:
        ReMode=SCW;
        ControlRE(1);// вкл.RE
        break;
      case SCW:
        ReMode=S27W;
        break;
      case S27W:
        ReMode=OFFs;
        ControlRE(0);// выкл.RE
        break;
      default:
        ReMode=OFFs;
        ControlRE(0);// выкл.RE
        break;
      }
      break;
    case Sc: // переключаем источник (обработка режима в основном цикле )
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
      ControlSC (ScMode); //// управление источником 
      break;  
      // измеритель длины
    case Ln:
      if(DataLN[1][IndxShowLN+1] != 0)
      {
        IndxShowLN++;
      }
      else
        IndxShowLN=0;
      break;
  //============ АВТОМАТ    
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
          if ((DeviceConfig.CfgRE & 0x1) &&(N_LS==0)) //есть красный глаз и просто измеритель
          {
            ConSet = RE; 
          }
          else
          {
            ConSet = P1a; 
          }
          break;
        case RE:
          if (N_LS == 0) // просто измеритель
          {
            ConSet = P1a; 
          }
          else //  источник
          {
            ConSet = Sc; 
          }
          break;
        case P1a:
          if (1)//!En_Cfg_P1a
          {
            if (N_LS) //не просто измеритель значит источник
            {
              if (DeviceConfig.CfgRE & 0x1) //есть красный глаз и просто измеритель
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
          if (!DeviceConfig.CfgL) // нет измерителя длинны
          {
            if (DeviceConfig.CfgPM)// есть измеритель и красный глаз
            {
              ConSet = P1; 
            }
            else
            {
              if (DeviceConfig.CfgRE & 0x1) //aесть красный глаз
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
        case At: // далее по списку режимов
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
          if (DeviceConfig.CfgPM) // есть измеритель
          {
            ConSet = P1;
          }
          else 
          {
            if (DeviceConfig.CfgRE & 0x1) //есть красный глаз
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
      // переключаемся по установкам
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
      case sLOSS: // оставим только его
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
  case 0x04: // кнопка "S"
    switch(CondWork)
    {
    case SetPrm: //sTime, sNumF, sLOSS, sORL, sSCp, sMem, sSWm
      switch(SetupMod)
      {
      case sTime:
        SetupMod=sAutoOff;
        break;
      case sAutoOff:
        if((DeviceConfig.CfgPM)||(DeviceConfig.CfgL)) // есть измеритель or AL w/o PM
        {
          SetupMod=sNumF; 
        }
       else 
       {
         if(N_LS) // есть источники
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
                 if(N_LS) // есть источники
       SetupMod=sSCp; 
       else
       SetupMod=sVer; 
        break;
      case sSCp:
                 if(DeviceConfig.CfgL) // есть ТЕСТЕР АВТОМАТ (измеритель длины)
       SetupMod=sORL; 
       else
       SetupMod=sVer; 
        break;
      case sORL:
       //SetupMod=sSWm; 
       SetupMod=sVer; // сразу прыгаем на версию 30/09/2022
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
      //memset(DataLN,0, ARRAY_SIZE(DataLN)); // чистим массив данных приема
      SW5V(1);// ON 5V включим 
     ttt=3000000;
     while(ttt--);
      
      memset(DataLN,0, NUMMEASLN*SEQMEASLN*2); // чистим массив данных приема
      lSofn = 1; //справа
      CntLCD_LN=2; // проверочный счетчик 12 мС для проверки прорисовки инликатора для отображения результатов
      CntLnCount=0;
      IndxShowLN = 0; // снова на первое измерение
      for (int i=0;i<NUMMEASLN;i++)
      {
      CountLN=0; // чистим счетчик отражений
     ttt=30000;
     CntLnCount=i;
     TIM2->PSC = 0; // обнуляем таймер измерения длины (один импульс 25 тиков)
     TIM3->PSC = 0;        
     // ставим таймер 2 на режим мастер запрос, импульс в начале счета
     //TIM2->ARR = 20+xImp;
     TIM2->ARR = 20+ImpZ[i];
     TIM2->CCR1 = 20; // Zimp = 100-20
     
     TIM2->CNT = 0; // обнуляем таймер измерения длины (один импульс 25 тиков)
     TIM3->CNT = 0;        

     HAL_TIM_PWM_Start (&htim3, TIM_CHANNEL_3 ); // вроде как надо запустить таймеры
     Beg_Evnt = DWT_CYCCNT; // запоминаем текущий счетчик
     // запускаем импульс
     HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_1 ); // вроде как надо запустить таймеры
     // надобы потупить
     while(ttt--);
     //CntLnCount++;
      }
      SW5V(0);// OFF 5V выключим 

      break;
    case At: // Запуск измерений в режиме АВТОМАТ
        if ((Mod_At == Wt)||(Mod_At == View))
        {
          FirstInput = 1;// первый раз вошли 
          Mod_At = Orl; // вызываем режим измерения ORL
          Ini_At_var(); // инициализация переменных режима At
          
//          Cnt_SPw = 0; // индекс перебора источников в команде
//          Pow_SC_Tx = 0.0; // мощность принятго источника по умолчанию
//          Err_Cmd_At=0; //сброс ошибок приема в режиме At
//          Start_Cmd = 2;
//          Mod_At_cnt=0; // сброс счетчика таймера команды
          //NumIzm++; // новое измерение
        }

      break;
    }
    break;
    //____________________________________________________________________________________________________
  case 0x40: // Меньше, влево
    switch(CondWork)
    {
    case P1: // просто измеритель переключение по длинам волн калибровки
      // включая специфические длины волн
      // и установка базового уровня соответствующая
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght-5,0);//получим новый коэфф. и перестроим указатели
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case RE: // красный глаз переключаем индекс Режима 0-OFFs, 1-SCW, 2-S27W(1Hz),
      switch(ReMode)
      {
      case OFFs:
        ReMode=S27W;
        break;
      case SCW:
        ReMode=OFFs;
        ControlRE(0);// выкл.RE
        break;
      case S27W:
        ReMode=SCW;
        ControlRE(1);// вкл.RE
        break;
      default:
        ReMode=OFFs;
        ControlRE(0);// выкл.RE
        break;
      }
      break;
    case Sc: // переключаем источник (обработка режима в основном цикле )
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
              ControlSC (ScMode); //// управление источником 

      break;  
    case SetFn: // перебор режимов
      switch(ConSet)
      {
      case P1:
        if(!DeviceConfig.CfgL) // нет измерителя длинны
        {
          if(N_LS) // есть источники
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
          if ((DeviceConfig.CfgRE & 0x1)&&(N_LS == 0)) //есть красный глаз и нет источников
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
        if (N_LS == 0) // просто измеритель
        {
          ConSet = P1; 
        }
        else //  источник
        {
          if (!DeviceConfig.CfgPM) // 0- источник без измерителя
          {
            if (!DeviceConfig.CfgL) // признак измерителя длинны нет
            {
              ConSet = Sc;
            }
            else
            {
              ConSet = Rl;
            }
          }
          else // 3- измеритель и источник
          {
            ConSet = P1a; 
          }
        }
        break;
      case Sc: // проверить если красный глаз и просто источник
        if (DeviceConfig.CfgRE & 0x1) //есть красный глаз 
        {
          ConSet = RE; 
        }
        else //  источник
        {
          if (!DeviceConfig.CfgPM) // 0- источник без измерителя
          {
            if (!DeviceConfig.CfgL) // признак измерителя длинны нет
            {
              ConSet = Sc;
            }
            else
            {
              ConSet = Rl;
            }
          }
          else // 3- измеритель и источник
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
      // переключаемся по установкам
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
    case P1: // просто измеритель переключение по длинам волн калибровки
      // включая специфические длины волн
      // и установка базового уровня соответствующая
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght,1);//получим новый коэфф. и перестроим указатели
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case P2: // переключаем длину волны и переустанавливаем базовый уровень (текущий) - пока просто меняем индекс указания на длину волны калибровки
      if (IndxP2LW<(WAVE_LENGTHS_NUM -1)) IndxP2LW++;
      else IndxP2LW = 0 ;
      IN_BuffADC_one = (IN_BuffADC[1]>ADC_Calibr)?(IN_BuffADC[1]-ADC_Calibr):(IN_BuffADC[1]);
      memset(BuffP2,IN_BuffADC_one, ARRAY_SIZE(BuffP2)); // Резко перезапишем буффер
      //memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // Резко перезапишем буффер
      NeedReDraw = 1;
      break;
      case Sc: // Source place switch
        // переключаем ЛАЗЕРЫ
        GetSetLW_SC (1, 0) ; // установим индекс выбранного источника для измерения
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс настроенного P2, для правильной работы ORL
        indx_LW_ORL = Ind_LW ; // установим индекс выбранного источника для измерения

//         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;

         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // найдем индекс длины волны уст источника в списке калибровочных длин волн
        break;
      case Rl: // Source place switch
        // переключаем ЛАЗЕРЫ
        GetSetLW_SC (1, 1) ; // установим индекс выбранного источника для измерения
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс настроенного P2, для правильной работы ORL
        indx_LW_ORL = Ind_LW ; // установим индекс выбранного источника для измерения

//         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;

         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // найдем индекс длины волны уст источника в списке калибровочных длин волн
        break;
        
    case SetPrm:
      // переключаемся по установкам
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
    case P1: // просто измеритель переключение по длинам волн калибровки
      // включая специфические длины волн
      // и установка базового уровня соответствующая
      CurrSpectrKoeff = GetCoeffSpctr(PMWavelenght,-1);//получим новый коэфф. и перестроим указатели
      ShowPM(ModeI, PMWavelenght);
      NeedReDraw = 1;
      break;
    case P2: // переключаем длину волны и переустанавливаем базовый уровень (текущий) - пока просто меняем индекс указания на длину волны калибровки
      if (IndxP2LW>0) IndxP2LW--;
      else IndxP2LW = WAVE_LENGTHS_NUM -1 ;
      IN_BuffADC_one = (IN_BuffADC[1]>ADC_Calibr)?(IN_BuffADC[1]-ADC_Calibr):(IN_BuffADC[1]);
      memset(BuffP2,IN_BuffADC_one, ARRAY_SIZE(BuffP2)); // Резко перезапишем буффер
      //memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // Резко перезапишем буффер
      NeedReDraw = 1;
      break;
      case Sc: // Source place switch
        // переключаем ЛАЗЕРЫ
        GetSetLW_SC (1, 0) ; // установим индекс выбранного источника для измерения
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс настроенного P2, для правильной работы ORL
        indx_LW_ORL = Ind_LW ; // установим индекс выбранного источника для измерения

//         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;

         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // найдем индекс длины волны уст источника в списке калибровочных длин волн
        break;
      case Rl: // Source place switch
        // Переключаем ЛАЗЕРЫ
        GetSetLW_SC (1, 1) ;// установим индекс выбранного источника для измерения Ind_LW
        IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс настроенного P2, для правильной работы ORL
        indx_LW_ORL = Ind_LW ;// установим индекс выбранного источника для измерения в P2

        //         if(++Ind_LW>2) Ind_LW=0;
//   
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
//            
//            if(DeviceConfig.PlaceLS[Ind_LW]==0)
//                       if(++Ind_LW>2) Ind_LW=0;
         //SWITCH_LW();
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // найдем индекс длины волны уст источника в списке калибровочных длин волн
        break;
    case SetPrm:
      // переключаемся по установкам
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
  case 0x10:   //Кнопка (переключение режимов) Fn (MODE) (<_->)
    if (CondWork != SetFn)
    {
      OldCWork = CondWork; // запоминаем режим в которм вызвали переключеатель
      ConSet = CondWork; // устанавливаем этот режим в переменной переключателя
      CondWork = SetFn; // устанавливаем текущий режим как Выбор Режимов
      SW_RS232(1); // сигнал управления оптическим приемом с усилителя (AD7782) (с другого пользуем только в Автомате)

      if (ModeI != dBm) ModeI=dBm;
      SSD1305_Fill(0); /* очистка экрана*/
      switch (ConSet)
      {
      case P1:
      case P1a:
        if (ModeI == Hold)
        {
          ModeI=OldMode;// выход из режима HOLD
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
        // возможно надо выключить источник
        ControlSC (0); // управление источником -  выключаем
        
        break;
      case SetPrm: // выход из режима установок наверное надо что-то сохранить
        switch(SetupMod)
        {
        case sTime:
        // фиксируем установленное время 
        //RTC_WriteTimeCounter(&hrtc,TotalSec()); // for F103
        ReSetRegRTC(1,&hrtc);// запишем новую дату F303

        break;
        case sNumF:
        NeedSaveChange |=0x10; // сохраняем пользовательские изменения (Номер волокна и признак авто инкремента)
          break;
        case sLOSSm:
        case sLOSS:
        case sORL:
        case sSWm:
        case sSCp:
        NeedSaveChange |=0x11; // сохраняем пользовательские изменения (Номер волокна и признак авто инкремента)
          break;
        }
        CondWork = SetFn; // возврат в режим выбора режимов
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
    else // зашли второй раз изменяем функцию 
      // подтверждаем выбранную функцию в новой версии
      // переключение по кругу для кнопки <> в новой версии
    {
      SetNeedCondWork (ConSet);
      
//////      if(OldCWork==Rl)
//////      {
//////        ScMode = OFFs; // управление источником -  выключаем потому как были в ORL ()
//////      }
//////      switch(ConSet)
//////      {
//////      case P1:
//////        CondWork = P1;
//////        SetModePM(1); // Auto режим P1 -
//////        break;
//////      case P1a:
//////        CondWork = P1a;
//////        for (int i=0; i<ARRAY_SIZE(RPON); i++) RPON[i]=-100.0;
//////        //SW_RS232(1); // сигнал управления оптическим приемом с усилителя (AD7782)- перенесли в переключение режимов)
//////        P1aWavelenght=0;  // Текущая длина волны P1a - сбросим
//////        // устанавливаем ручной режим и ждем принятие данных , считаем паузы, и выходим в измеритель если долго не принимаем
//////        CntWaitLW = 0; // сброс счетчика ожидания приема длины волны
//////        SetModePM(0); // Ручной режим P1 -
//////        SetRange(1); // пред чувствительный диапазон
//////        EnaOpticUART=1;
//////        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
//////        YesLW=0;
//////        
//////        break;
//////      case Sc:
//////        CondWork = Sc;
//////        //ControlSC (ScMode); // управление источником - вкл. тот режим в котром был источник
//////        //ShowSource();
//////        break;
//////      case RE:
//////        CondWork = RE;
//////        //ShowSource();
//////        break;
//////      case Rl:
//////        CondWork = Rl;
//////        //SWITCH_LW(); // установка требуемой длины волны источника
//////         ScMode = SCW; // управление источником - вкл. на постояннку
//////         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // найдем индекс длины волны уст источника в списке калибровочных длин волн
//////         IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс настроенного P2, для правильной работы ORL
//////         indx_LW_ORL = Ind_LW; // // установим индекс выбранного источника для измерения
//////         PowClbr_mW = CoeffPM.Pow_mW_Clbr;
//////        //ShowSource();
//////        break;
//////      case Ln:
//////        CondWork = Ln;
//////        ScMode = OFFs; // управление источником -  выключаем
//////        //SW5V(1);// ON 5V включим потом когда измерять будем
//////        //ShowSource();
//////        break;
//////      case P2:
//////        CondWork = P2;
//////        ScMode = OFFs; // управление источником -  выключаем
//////        //ShowSource();
//////        break;
//////      case At:
//////        // востановим из памяти калибровку длины
//////        CalibLnAuto = UserMeasConfig.CalibLnAutoMem;
//////        CondWork = At;
//////        Mod_At = Wt; // переключимся в режим 
//////        ScMode = OFFs; // управление источником -  выключаем
//////        SW_RS232(0); // сигнал управления оптическим приемом с усилителя (AD8304)
//////        // Запись числа активных лазеров
//////        GetNumActivLS();
//////        huart2.Init.BaudRate = 1200;
//////        if (HAL_UART_Init(&huart2) != HAL_OK)
//////        {
//////          Error_Handler();
//////        }
//////        HAL_UART_DMAStop(&huart2);
//////        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
//////        Dummy = huart2.Instance->DR ; // чистим буффер приема 
//////
//////
//////        FirstInput = 1;// первый раз вошли 
//////
//////        //ShowSource();
//////        break;
//////      default:
//////        // пока вернемся в измеритель
//////        CondWork = P1;
//////        ScMode = OFFs; // управление источником -  выключаем
//////        break; 
//////      }
//////        ControlSC (ScMode); // управление источником -  устанавливаем согласно режима
//////        
//////      NeedReDraw=1;
      //      CondWork = ConSet;
      //      /*
      //      if ((CondWork == P1a)||(CondWork == P2)) 
      //      {
      //      UCA0BR0 = 0x1B;   //Скорость передачи 2400=0x0d  1200=0x1B
      //      UCA0BR1 = 0x00;   //~1200 бод для P2
      //      UCA0MCTL = 0x04;  //при кварце 32768 2400=0x6b 1200=0x04
      //    }
      //        else
      //      {
      //      UCA0BR0 = 0x06;   //Скорость передачи
      //      UCA0BR1 = 0x00;   //~4800 бод
      //      UCA0MCTL = 0x0E;  //при кварце 32768
      //      
      //    }
      //      */
      //      //Str1[0]= ' ';
      //      //Str1[1]= ' ';
      //      Displ = 0;
      //      ShowMsg(6); 
      //      ShowMsg(9);
      //      ShowMsg(25);
      //      ShowMsg(30); // стираем меню функций на индикаторе
      //      ShowMsg(39); // стираем 4 сточку
      //      //Paint();
      //      Displ = 2;
      //      Str1[0]='0'+CondWork; // рисуем выбранную функцию
      //      ShowMsg(32);
      //      // Выбор индикации длины волны в зависимости от режимов
      //      switch (CondWork)
      //      {
      //      case Ln:
      //        ScMode = OFF;
      //        Ind_LW = 1; // 850 nm
      //        P3OUT |=sw_L; // 1 - на выход P3.2 
      //        P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //        break;
      //      case Rl: // для измерителя длины  при проверке признака дубля 
      //      case Ls: // сидят ли они на одном ответвителе для 
      //        ScMode = OFF;
      //        switch (DataDevice[19]-'0')
      //        {
      //        case 1:
      //          Ind_LW = 1; // 850 nm
      //          P3OUT |=sw_L; // 1 - на выход P3.2 
      //          P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //          break;
      //        case 2:
      //          Ind_LW = 1; // 1310 nm
      //          P3OUT |=sw_L; // 1 - на выход P3.2 
      //          P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //          break;
      //        case 3:
      //          Ind_LW = 1; // 1550 nm
      //          P3OUT |=sw_L; // 0 - на выход P3.2 // паяется в то же место что и 1310
      //          P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //          break;
      //        case 4:
      //          Ind_LW = 1; // 850 nm
      //          P3OUT |=sw_L; // 1 - на выход P3.2 
      //          P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //          break;
      //        case 5:
      //          Ind_LW = 0; // 1310 nm
      //          P3OUT &=~sw_L; // 0 - на выход P3.2 
      //          P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //          break;
      //        case 6:
      //        case 7:
      //          Ind_LW = 0; // 1310 nm
      //          P3OUT &=~sw_L; // 0 - на выход P3.2 
      //          P6OUT &=~sw_H; // 0 - на выход P6.3  управление ADG804        
      //          //           P2OUT &=~LD_R; // 0 - на выход P2.7 off 1.49 
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
      //          P2OUT &=~LD_R; // 0 - на выход P2.7 
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
      //          //            P2OUT &=~LD_R; // 0 - на выход P2.7 off 1.49 
      //        }
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        
      //        sprintf(Str7, "%4.2f ",PwS[Ind_LW]);
      //        ShowMsg(25);
      //        ShowMsg(22);
      //        break;
      //      case P1a: // включаем снова измеритель в автомат
      //        UC1IE |=UCA0RXIE;//Разрешить USART1 RX  прерывания
      //        
      //        MD_P1A = 2; // начало измерений в автомате
      //        Ind_PON = 0; // 1310
      //        Range = 2;
      //        SetRange();
      //        EQ_LW = 0; 
      //        CH_Po = 0;
      //        Pow_Yes = 0;
      //        N_PON = 0; // число измерений в сеансе автомат
      //        if ((OldCWork == Ls) || (OldCWork == Rl))
      //        {
      //          sc_on = 0;
      //          P3OUT &=~CW; 
      //          //            P2OUT &=~LD_R;
      //        }
      //        //Mode = OldMode; // надо проверить
      //        Mode = dB; // принудительно dB
      //        ShowMsg_PON(128);
      //        Ini_ind_PON();
      //        break;
      //      case P1: // включаем снова измеритель
      //        //if (CondWork == P1a) CH_Po = 0;
      //        if ((OldCWork == Ls) || (OldCWork == Rl))
      //        {
      //          sc_on = 0;
      //          P3OUT &=~CW; 
      //          //            P2OUT &=~LD_R;// off 1.49
      //        }
      //        Mode = OldMode; // надо проверить
      //        //Displ = 0;
      //        //ShowMsg(25);
      //        //Displ = 2;
      //        Displ = 2;
      //        Str1[0]='0'+CondWork; // рисуем выбранную функцию
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
      //      case At:// режим автоматического измерения
      //        ScMode = OFF;
      //        P6OUT &=~sw_RS; // переключаем ключ RS1k2
      //        sc_on = 0;
      //        P3OUT &=~CW; // выключен источник 
      //        sprintf(Str4, "   ожидание   ");
      //        sprintf(Str5, "    команд    ");
      //        ShowMsg(30);
      //        sprintf(Str7, "№%03d  ",NumIzm); // индикация ячейки памяти
      //        ShowMsg(25); // пишем на место базового уровня
      //        Mod_At = Wt; // АТ - режим ожидания команд
      //        Mod_At_cnt = MaxAT; // пока так максимальный цикл измерений в автомате
      //        Reciev1k2 = Stop; // даже если "застыли" в End то вернем в стоп .. 05.07.2010
      //        UC1IE |=UCA0RXIE;//Разрешить USART1 RX  прерывания
      //        break;
      //      default:
      //        break;
      //      }
      //      if (CondWork != P1a)Num2Lenght();
      //      //Paint();
      //      OldCWork = CondWork;
    }
    
    //Paint();
    // алгоритм переключения функций прибора        
    break; // CASE 0x10
    //__________________________________________________________________________________________
  case 0x7B: // кнопка "S" - Долгое нажатие! 
    // в режиме перебора режимов вызывем установку зазных параметров, Часов, 
           switch (CondWork)
           {
           case SetFn:
             // проверим есть ли измеритель и на нем ли стоимЮ или источник
             if(((ConSet==P1)&&(DeviceConfig.CfgPM))||((!(DeviceConfig.CfgPM))&&(ConSet==Sc))) 
             {
               CondWork = SetPrm;
               SetupMod = sTime;
               NeedReDraw = 1;
               IndxEditClock = 0; 
             }
             break;
           case P1: // Запись ячейки памяти
           case P1a: // Запись ячейки памяти
           case P2: // сохранение результатов измерения
           case Rl: // сохранение результатов измерения
           case Ln: // сохранение результатов измерения
             if(SettingPrm.CountMemory<512)
             {
               WriteBlockMEM (1); // запись блока данных в память, отделной функцией для UART EXT

             }
             CountDrawMsg = 7;
             break;
           case At: // сохранение результатов измерения
             if(Mod_At == View)
             {
             if(SettingPrm.CountMemory<512)
             {
               WriteBlockMEM (1); // запись блока данных в память, отделной функцией для UART EXT
             }
               Mod_At = Wt;
               FirstInput=1;
               NeedReDraw = 1;
             }
             
             break;
           }
    break; // CASE 0x7B
    //_________________________________________________________________________________________
  case 0x7E: // кнопка "dBm/dB/W" - Долгое нажатие!
       switch (CondWork)
       {
       case P1:
         CurrBaseLevel = Opt_dBm; // UserMeasConfig.BaseLvl[j];
         ModeI = dB;
         // Установка и запись нового базового уровня при привязке для P1
         SetNewBaseLevel ();
         ShowPM(ModeI, PMWavelenght);
         NeedReDraw = 1;
         break;
       case P1a:
         for(int i=0; i < ARRAY_SIZE(RPON); ++i) // переберем ячейки RPON на наличие значений
         {
           if (RPON[i]>-100.0) // есть значение для индикации
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
         // долгое нажатие вкл. выкл корректировку "внизу" Малые сигналы
         if(P2dBmValue < LIM_ORL_CORR_NOISE) // только тогда корректируем
         {
           if(PowClbr_mW != CoeffPM.Pow_mW_Clbr)
         PowClbr_mW = CoeffPM.Pow_mW_Clbr;
         else
         PowClbr_mW = MedP2mW;
         NeedReDraw = 1;
         }
         // проверка на вкл. выкл корректировки "на верху" 14.7 дБ
         if((ORLCalc>LIM_ORL_UP)&&(ORLCalc<LIM_ORL_DN))
         {
    if( BaseRetCurrORL[indx_LW_ORL] != CoeffPM.BaseRet[indx_LW_ORL])
      // если были скорректрованы возвращаем к настроеным
         BaseRetCurrORL[indx_LW_ORL] = CoeffPM.BaseRet[indx_LW_ORL];
     else 
     { // надо переститать для 14.7
       BaseRetCurrORL[indx_LW_ORL] = CoeffPM.BaseRet[indx_LW_ORL] + (14.7 - ORLCalc);
     }    
         NeedReDraw = 1;
         }
         break;
       } 
         NeedSaveChange |=0x01; // сохраняем пользовательские изменения (базовые уровни)
        break;
//  case 0xXX: // кнопка "" - Долгое нажатие!
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
        ScMode = OFFs; // управление источником -  выключаем потому как были в ORL ()
      }
      if(OldCWork == P1a) // сбросим его разрешения UartOptic и счетчик ожидания приема
      {
        CntWaitLW = 6000; // переустановка счетчика ожидания приема длины волны
        EnaOpticUART=0;
        SetModePM(1); // возврат в автоматический режим P1 -
      }
      switch(SetConditionWork)
      {
      case P1:
        CondWork = P1;
        SetModePM(1); // Auto режим P1 -
        break;
      case P1a:
        CondWork = P1a;
        for (int i=0; i<ARRAY_SIZE(RPON); i++) RPON[i]=-100.0;
        //SW_RS232(1); // сигнал управления оптическим приемом с усилителя (AD7782)- перенесли в переключение режимов)
        P1aWavelenght=0;  // Текущая длина волны P1a - сбросим
        // устанавливаем ручной режим и ждем принятие данных , считаем паузы, и выходим в измеритель если долго не принимаем
        CntWaitLW = 0; // сброс счетчика ожидания приема длины волны
        SetModePM(0); // Ручной режим P1 -
        SetRange(1); // пред чувствительный диапазон
        EnaOpticUART=1;
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        HAL_UART_DMAStop(&huart2); // добавил сюда малоли что? винул при перелючении в авто ...
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
        YesLW=0;
        break;
      case Sc:
        CondWork = Sc;
        //ControlSC (ScMode); // управление источником - вкл. тот режим в котром был источник
        //ShowSource();
        break;
      case RE:
        CondWork = RE;
        //ShowSource();
        break;
      case Rl:
        CondWork = Rl;
        //SWITCH_LW(); // установка требуемой длины волны источника
         ScMode = SCW; // управление источником - вкл. на постояннку
         //indx_LW_ORL = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // найдем индекс длины волны уст источника в списке калибровочных длин волн
         IndxP2LW = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс настроенного P2, для правильной работы ORL
         indx_LW_ORL = Ind_LW; // // установим индекс выбранного источника для измерения
         PowClbr_mW = CoeffPM.Pow_mW_Clbr;
         // присваиваем текщим значениям смещения ORL из таблици настройки
         for(int i=0; i<3; i++)
         BaseRetCurrORL[i] = CoeffPM.BaseRet[i];

        //ShowSource();
        break;
      case Ln:
        CondWork = Ln;
        ScMode = OFFs; // управление источником -  выключаем
        //SW5V(1);// ON 5V включим потом когда измерять будем
        //ShowSource();
        break;
      case P2:
        CondWork = P2;
        ScMode = OFFs; // управление источником -  выключаем
        //ShowSource();
        break;
      case At:
        // востановим из памяти калибровку длины
        CalibLnAuto = UserMeasConfig.CalibLnAutoMem;
        CondWork = At;
        Mod_At = Wt; // переключимся в режим 
        ScMode = OFFs; // управление источником -  выключаем
        SW_RS232(0); // сигнал управления оптическим приемом с усилителя (AD8304)
        // Запись числа активных лазеров
        GetNumActivLS();
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
        //Dummy = huart2.Instance->DR ; // чистим буффер приема 
        Dummy = Clr_UART; // чистим буффер приема 


        FirstInput = 1;// первый раз вошли 

        //ShowSource();
        break;
      default:
        // пока вернемся в измеритель
        CondWork = P1;
        ScMode = OFFs; // управление источником -  выключаем
        break; 
      }
        ControlSC (ScMode); // управление источником -  устанавливаем согласно режима
        
      NeedReDraw=1;
 return CondWork;
}

uint16_t WriteBlockMEM (uint8_t Dir) // запись блока данных в память, отделной функцией для UART EXT
             {
               uint8_t yesLS;
               uint16_t EnaWrite=0;
               uint16_t AdrWriteMem=(uint16_t)(SettingPrm.CountMemory<<6) + ADR_SaveMemory; 
               if((SettingPrm.CountMemory%4)==0) // начало блока (чистим блок)256 byte
               {
                 memcpy(&MemBlock, 0, sizeof(MemBlock));
                 EEPROM_write(&MemBlock, AdrWriteMem , sizeof(MemBlock));
               }
               // зачистим-заполним поля по умолчанию
               MemoryData.mTime = ReSetRegRTC(0, &hrtc); // время записи
               MemoryData.mNumFiber=(SettingPrm.EnaAutoInkrNF)?(++SettingPrm.NumberFbrCount):(SettingPrm.NumberFbrCount);// Номер волокна
               MemoryData.mCode=0; // код записи для расшифровки
               for(int j=0;j<3;j++)
               {
                 for(int k=0;k<4;k++)
                   MemoryData.mRes[j][k]=0;
                 MemoryData.mLW[j]=0;// длина волны первой записи
               }
               MemoryData.mLen=0; // длина участка
               MemoryData.mFree=0; // резерв 
               
               switch(CondWork)
               {
               case P1: 
                 // заполняем блок данных
                 MemoryData.mCode=CondWork*10+ModeI; // код записи для расшифровки
                 MemoryData.mLW[0]=PMWavelenght; // длина волны первой записи
                 MemoryData.mRes[0][0]=GetPMValue()*CurrSpectrKoeff; // значение в мВт
                 MemoryData.mRes[0][1]=GetPMValuedBm(MemoryData.mRes[0][0]); // значение в дБм
                 MemoryData.mRes[0][2]=GetPMValuedB(MemoryData.mRes[0][0]); // значение в дБ
                 MemoryData.mRes[0][3]=CurrBaseLevel; // опорный уровень
                 EnaWrite=1;
                 break;
               case P1a:
                 MemoryData.mCode=CondWork*10+ModeIP1a; // код записи для расшифровки
                 CountPON=0;
                 for (int i=0; i<WAVE_LENGTHS_NUM; i++) // пробежим по значениям PON
                 {
                   if(RPON[i]>-100.) 
                   {
                 MemoryData.mLW[CountPON]=CalibrLW[i]; // длина волны первой записи
                 MemoryData.mRes[CountPON][0]=RPON[i]; // значение в dBm
                 MemoryData.mRes[CountPON][1]=RPON[i]-UserMeasConfig.BaseLvl[i]; // значение в дБ
                 MemoryData.mRes[CountPON][2]=UserMeasConfig.BaseLvl[i]; // значение базового уровня для этой длины волны
                 MemoryData.mRes[CountPON][3]=BaseExtLvl[i]; // опорный уровень
                    CountPON++; 
                   }
                   if(CountPON==3)
                     break;
                 }
                 EnaWrite=1;
                 break;
               case P2: 
                 MemoryData.mCode=CondWork*10+P2Mode; // код записи для расшифровки
                 MemoryData.mLW[0]=CalibrLW[IndxP2LW]; // длина волны 
                 MemoryData.mRes[0][0]=P2dBValue; // значение в дБ
                 MemoryData.mRes[0][1]=P2dBmValue; // значение в дБм
                 MemoryData.mRes[0][2]=UserMeasConfig.BaseLvlP2[IndxP2LW]; // 
                 EnaWrite=1;
                 break;
               case Ln: 
                 MemoryData.mCode=CondWork*10; // код записи для расшифровки
                 MemoryData.mLW[0]=DeviceConfig.PlaceLS[1]; // длина волны источника второго места
                 MemoryData.mLen=MeasLenght; // длина участка
                 EnaWrite=1;
                  break;
               case Rl:
                 MemoryData.mCode=CondWork*10; // код записи для расшифровки
                 MemoryData.mRes[0][0]=ORLCalc; // значение в дБ
                 MemoryData.mLW[0]=CalibrLW[IndxP2LW]; // длина волны 
                 EnaWrite=1;
                 
                  break;
               case At:
                 // запись результатов измерения в память
                MemoryData.mCode=CondWork*10; // код записи для расшифровки
                 CountPON=0;
                 for (int i=0; i<WAVE_LENGTHS_NUM; i++) // пробежим по значениям калибр длин волн
                 {
                   yesLS=0;
                   for(int x=0;x<3;x++)
                   {
                     if(PonA.LW_AB[x] == CalibrLW[i]) 
                       yesLS=1;
                   }
                   if(yesLS) 
                   {
                 MemoryData.mLW[CountPON]=CalibrLW[i]; // длина волны первой записи
                 MemoryData.mRes[CountPON][0]=PonA.LS_AB[i]; // значение затухания в dB с А->B
                 MemoryData.mRes[CountPON][1]=PonA.LS_BA[i]; // значение затухания в dB с B->A
                 MemoryData.mRes[CountPON][2]=PonA.ORL_AB[i]; // значение ORL dB с А->B
                 MemoryData.mRes[CountPON][3]=PonA.ORL_BA[i]; // значение ORL dB с B->A
                     yesLS=0;
                    CountPON++; 
                   }
                   if(CountPON==3)
                     break;
                 }
                 MemoryData.mLen=PonA.LnA; // длина участка
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
