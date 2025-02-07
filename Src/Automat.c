// Функция обработки режима AT
// важно следить за прерываниями UART оптических
// правильно принимать сигналы по OpticUART
#include "main.h"

// временные точки установок в режимов в тиках 50 мС
// SavPw
#define tsSavPw 5     // Передача команды Slave
#define tseSavPw 10    // время включения источника 20
#define tsdSavPw 60    // время выключения источника 60
#define teSavPw 100     // предельное время ожидания ответов 100
// LnR_Clb
#define tsLnRClb 6     // Передача команды Slave
#define tziLnRClb 40    // время передачи зондирующего импульса Мастером
#define teLnRClb 70     // время выхода из режима
// CalPw - ответы Slave на запрос мощности
#define tsCalPw 48     // время измерения мощности от мастера 50
#define tansCalPw 60     // время ответа мастеру 60
// LnX
#define tsLnX 30     // время перевода в режим ожидания импульса от Мастера
#define teLnX 40     // время выхода из режима 
// ORL
#define tsORL 5     // время подачи команды Slave от Мастера
#define tseLsORL 20    // время включения источника
#define tsdLsORL 60    // время выключения источника
#define teORL 100     // предельное время ожидания ответов
// CmdSw
#define tsCmdSw 1     // время подачи команды Slave от Мастера
#define tsoCmdSw 5    // время включения значка источника
#define teCmdSw 20     // предельное время ожидания ответов
// Sl_Px Slave "главный" задает мастеру измерять мощности на 20 тике (2 сек) сам излучает в течении 3 сек

// AnsOrl
#define tnAnsOrl 10    // время новой передачи ORL от Slave в цикле
#define teAnsOrl 30    // предельное время ожидания ответа ORL от Slave то что он намерил 1.5 сек на передачу данных можно попробовать меньше (20)

// длительность импульза для измерителя длины
//#define xImp 25


const char* Msg[]={"    ожидание    ", // 0
                   "     команд     ", // 1
                   "   калибровка   ", // 2
                   "    источника   ", // 3
                   "     ошибка     ", // 4
                   "   калибровки   ", // 5
                   "   калибровка   ", // 6
                   "     длины      ", // 7
                   "   завершена    ", // 8
                   "     длины      ", // 9
                   "                ", // 10 пустая строка
                   "    измеряем    ", // 11 
                   "     длину      ", // 12 
                   "   измерение    ", // 13 
                   "    мощности    ", // 14 
                   "   нет св\xffзи     ", //15
                   "     главный    ", //16
                   "    излучение   ", //17
                   "    передача    ", //18
                   "   обр.отраж.   ", //19
                   "-о-о-о-о-о-о-о-о", //20
                   "-- чо-то не то? ", //21
                   "   не понятное  "};//22

int Mod_At_cnt; // Счетчик автоматического режима когда что делать
static uint16_t tSt_Cnt=0;
static uint8_t En_TX_Cmd;// Разрешение передачи команды по оптике в режиме автомата, передаем по OpticUART
uint8_t MsgStat[18]; // сообщение статуса режима Автомат тире-кружочки
uint8_t MsgAuto[3][20];
//переманные автоматического режима
uint8_t FirstInput; // переменная первого входа по кнопкам в режиме автоматов ( для успешной инициализации подрежима)
uint8_t Start_Cmd; // время когда стартовать команду по счетчику подрежима для разных режимов свое время
uint8_t Inv=0; // aaeo oeacaiey eiaa?nee i?e eiaeeaoee PON
uint8_t Inv_Bas=0; // aaeo oeacaiey eiaa?nee i?e eiaeeaoee i?aaaeia PON
uint8_t Cnt_SPw; // Счетчик источников в цикле формирования команд
uint8_t Cnt_Izm; // счетчик измерений выполненых противоположной стороной В или А (зависит от числа источников)
uint8_t Cnt_BA; // счетчик измерений выполненых противоположной стороной В или А (зависит от числа источников)
uint8_t Err_Cmd_At; // признак ошибки при командах в режиме Atв режиме автомат для переключения управления
uint8_t NumChoiceLS; // число разрешенных лазеров в Автомате (число на 1 выходе)(считаем один раз)
uint8_t Uni_At=0; // индекс сохраняемой переменной
volatile uint8_t  NeedTransAnsver=0; // необходимость ответа Slave на принятый импульс от мастера
uint8_t DisRecImp; // запрет приема пока передали импульс (думаю на 3-5 мС) (через системный таймер)
uint8_t IndxValidLN; // индекс последовательности принятых данных измерения в котором верные данные измерителя длины
uint32_t WaitCnt;
uint32_t TestCntWait;// тестовый счетчик измерений (в данном случае P2) 

volatile uint32_t WaitCntLnX=0; // счетчик мС для ожидания передачи ответа мастеру от Slave в режиме LnX
// продолжаем поддерживать ответы (взводим снова       NeedTransAnsver=1; // необходимость ответа Slave на принятый импульс от мастера

volatile uint32_t Beg_Evnt; // 
volatile uint32_t End_Evnt; // 
volatile uint32_t Time_Evnt; // 

uint32_t Summx=0; // переменные для расчета значений измерения длины и калибровки в тиках
uint32_t cntr;

//
char RX1_DAT;
uint8_t NeedReadPWSlave;
uint16_t LWSlaveSRC[3]; // длина волны источника Slave в режиме АВТОМАТ

float Pow_SC_Slave; // мощность источника Slave в режиме АВТОМАТ
float Pow_SC_Tx; // Мощности источников для передачи в режиме CalPw

TAB_PONAUTO PonA; // структура  данных PON AUTO


uint16_t CalibLnAuto; // смещение длины после калибровки в режиме автомат!
uint8_t StrAnsverData[32]; // строка тестовая для передачи данных о принятых ответов
int Indxi; //вспомогательный индекс для уменьшения расчетов 
// выделим область для строк приема в режиме мастер или слэйв
//char MemBufReciev[16][32];
//uint32_t CntAutoMode = 0; // счетчик принятых ответов по оптике в режиме тестер автомат
// футкция тестового отбражение
void TestDraw (void)
{
  //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
  //sprintf((char*)Stre, "%d(%d)-%d(%02X):%d",Mod_At_cnt,NeedReDraw,CntErrI2C,CntErrKbrd,CntErrAuto);//тест вывод
}
uint32_t CntErrI2C; // счетчик ошибок I2C  
uint32_t CntErrKbrd; // счетчик ошибок I2C чтение клавиатуры! 
uint32_t CntErrAuto; // счетчик ошибок авто режима! 
// копируем строку приема в память
//void BufOptCopyMem (void)
//{
//  if(CntAutoMode < 15)
//  {
//    CntAutoMode++;
//    //memcpy(MemBufReciev,
//  }
//}

// поиск индекса калибровочной длины волны по ее значению
int iClbFromLW (uint16_t LWi)
{
  int i=0;
    for (i=0;i<WAVE_LENGTHS_NUM;i++)
  {
    if(LWi == CalibrLW[i]) return i;
  }
  return i;
}

void Ini_At_var(void) // инициализация переменных Ат режима
{
    for (int i=0;i<18;i++)
  {
    MsgStat[i]='-';
  }
      MsgStat[15]=0;// ограничим строку

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
  // чистим память оптического приема
  //memset(MemBufReciev,512);
  //CntAutoMode = 0;
}



// Запись числа активных лазеров
void GetNumActivLS(void)
{
  uint8_t Num=0;
  for(int i=0;i<3;++i)
  {
    if(DeviceConfig.CfgOUT[i]) Num++;
  }
  NumChoiceLS = Num;
}
void CheckTimerAT (void)// контроль таймера в режиме АВТОМАТ по командам (
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
    //________________________________ ОЖИДАНИЕ
  case Wt:
    if(FirstInput) // надо инициализировать режим
    {
      memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // ожидание
      memcpy(MsgAuto[1],Msg[1],strlen(Msg[1])); // команд
      FirstInput=0; 
      NeedReDraw = 1;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    else
    { 
      if(OpticCMD)
      {
        switch(OpticCMD)
        {
          //-----------------
        case SMPcmd: //в режиме ожидание запуск измерения мощности от мастера
          Mod_At = CalPw; // переключимся в режим 
          memcpy(MsgAuto[0],Msg[2],strlen(Msg[2])); // калибровка (ответ на индикаторе SLAVE)
          memcpy(MsgAuto[1],Msg[10],strlen(Msg[10]));
          PMWavelenght = iParamOpt; // в строку вывода записываем длину волны передаваемого источника от МАСТЕРА
          IndxP2LW = GetIndxLW(PMWavelenght); // получим индекс длины волны для правильного измерения данных в P2
          Pow_SC_Tx = 0.0; // мощность принятого источника по умолчанию
          Mod_At_cnt = 0; // сброс счетчика таймера команды
          Err_Cmd_At = 0; //сброс ошибок приема в режиме  At
          En_TX_Cmd = 0;// время ответа мастеру
          Start_Cmd = tansCalPw; // 
          NeedReDraw = 2;
          TestDraw();
          //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
          TestCntWait=0;
          CntrlA(0); // 
          
          break;
          //---------------------
        case SWLcmd: //в режиме ожидание запуск измерения длины
          Mod_At = Lnx;
          SW5V(1);// ON 5V
          memcpy(MsgAuto[0],Msg[11],strlen(Msg[11])); // калибровка длины
          memcpy(MsgAuto[1],Msg[12],strlen(Msg[12]));
          Start_Cmd = MaxAT+2; // 
          Mod_At_cnt = 0; // сброс счетчика таймера команды
          Err_Cmd_At = 0; //сброс ошибок приема в режиме  At
          En_TX_Cmd = 0;
          NeedReDraw = 3;
          TestDraw();
          //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
          //SW5V(1);// ON 5V
          break;
          //-----------------
        case SLAcmd: //в режиме ожидание запуск измерения мощности от мастера в основном цикле АВТО измерений,
          // режим идентичен калибровке (можно просто переслать измеренную мощеность
          Mod_At = Px; // переключимся в режим Px (аналогично CalPW из калибровки)
          memcpy(MsgAuto[0],Msg[11],strlen(Msg[11])); // измеряем (ответ на индикаторе SLAVE)
          memcpy(MsgAuto[1],Msg[14],strlen(Msg[14])); // мощности
          PMWavelenght = iParamOpt; // в строку вывода записываем длину волны передаваемого источника от МАСТЕРА
          // второй параметр передаваемая мощность в принципе не важна, можно посчитать на стороне МАСТЕРА
          IndxP2LW = GetIndxLW(PMWavelenght); // получим индекс длины волны для правильного измерения данных в P2
          Pow_SC_Tx = 0.0; // мощность принятого источника по умолчанию
          Mod_At_cnt = 0; // сброс счетчика таймера команды
          Err_Cmd_At = 0; //сброс ошибок приема в режиме  At
          En_TX_Cmd = 0;// время ответа мастеру
          Start_Cmd = tansCalPw; // 
          NeedReDraw = 4;
          TestDraw();
          //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
          break;
          //-----------------
        case SWMcmd: //в режиме ожидания переключение Slave в 
          //формирование команды подтверждения переключения SOK
          Mod_At_cnt = 0;
          memcpy(MsgAuto[0],Msg[16],strlen(Msg[16])); // главный
          memcpy(MsgAuto[1],Msg[10],strlen(Msg[10]));
          Mod_At = AnsOK; // переключимся в режим AnsOK
          Start_Cmd = tsCmdSw; // почти сразу отвечаем и переключаемся режим передачи команд SLA, для измерения МАСТЕРОМ затухания со стороны В
          lSofn = 0; //
          En_TX_Cmd = 0;// время ответа мастеру
          NeedReDraw = 5;
          TestDraw();
          //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
          break;
          //-------------------
        case SRLcmd: //в режиме ожидания  Slave в 
          //формирование ответов об измеренных ORL
          Mod_At_cnt = 0;
          memcpy(MsgAuto[0],Msg[18],strlen(Msg[18])); // передача
          memcpy(MsgAuto[1],Msg[19],strlen(Msg[19])); // обр.отраж.
          Mod_At = AnsOrl; // переключимся в режим AnsOrl
          GetSetLW_SC (0, 1); //установим первый лазер которым светить 0-первый,1-с маской разрешенных
          Start_Cmd = tsCmdSw; // почти сразу отвечаем и переключаемся режим передачи 
          lSofn = 0; //
          Cnt_SPw = 0; // счетчик рабочих мест которые надо передать
          En_TX_Cmd = 0;// время ответа мастеру
          NeedReDraw = 6;
          TestDraw();
          //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
          break;
          //-------------------
          
        default:
          break;
        }
        OpticCMD = NOTcmd; // сброс приема команды
        
      }
      
    }
    break; // Wt (ожидание)
    //________________________________ КАЛИБРОВКА ИСТОЧНИКА И ДЛИНЫ ЛИНИИ
  case SavPw: // калибровка источника (МАСТЕР) шлет запросы
    if(FirstInput) // надо инициализировать режим
    {
      memcpy(MsgAuto[0],Msg[2],strlen(Msg[2])); // калибровка источника
      memcpy(MsgAuto[1],Msg[3],strlen(Msg[3]));
      Cnt_SPw = 0; // сброс индекса перебора лазеров
      Pow_SC_Tx = 0.0; // мощность принятого источника по умолчанию
      Err_Cmd_At=0; //сброс ошибок приема в режиме  At
      Start_Cmd = tsSavPw; // задаем тик цикла в котором передаем команду 0.25 сек
      Mod_At_cnt=0; // сброс счетчика таймера команды
      GetSetLW_SC (0, 1); //установим первый лазер которым светить 0-первый,1-с маской разрешенных
      FirstInput=0; 
      tSt_Cnt=0;
      NeedReDraw = 7;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    else // сам режим , цикл передачи длины волны излучения, с последующим постоянным излучением,
    {
      // 1. Надо передать длину волны первого источника, 
      // 2. Затем переключить в постоянное излучение
      // 3. дождаться по OpticUART приема ответа с мощностью источника
      // 4. Повторить для других длин волн
      // 5. Запустить тестовое измерение длины (сформировать импульс запроса)
      // 6. дождаться ответа на импульс, расчитать время мертвой зоны
      //while((DeviceConfig.PlaceLS==0)(
      if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // разрешено передавать команду и не все источники то передаем
      {   
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
        //Dummy = huart2.Instance->DR ; // чистим буффер приема 
        Dummy = Clr_UART; // чистим буффер приема 
        
        HAL_UART_DMAStop(&huart2);
        
        // лазер установлен, можно передать команду
        sprintf((char*)StrTXOpt, "####SMP%4u\rUUU",DeviceConfig.PlaceLS[Ind_LW]);//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
        TxOptBusy = 1;
        sprintf((char*)Strf,"%s",StrTXOpt) ;
        //TsT(1);
        Mod_At_cnt++; // +50мС
        tSt_Cnt++;
        En_TX_Cmd = 0;
        lSofn = 1; //
        PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
        NeedReDraw = 8;
        TestDraw();
        //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
        
      }
      else 
      {
        if ((Mod_At_cnt > tsdSavPw+10) && (Cnt_SPw == NumChoiceLS-1)) // все
        {
          // надо откалибровать длину...
          //myBeep(100);
          memcpy(MsgAuto[0],Msg[21],strlen(Msg[21])); // что-то непонятное, по следам темного индикатора в Автомате (январь 2025)
          //memcpy(MsgAuto[1],Msg[22],strlen(Msg[22])); 
          sprintf((char*)MsgAuto[1], "%s",MsgStat);
          
          NeedReDraw = 9;
          TestDraw();
          //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
          
          ;
        }
      }
      // контролируем счетчик для установки постоянного излучения (
      if(TxOptBusy==0)
      {
        // время включения источника
        // время выключения источника
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
      else
      {
        CW_S(0);
      }
    }
    // проверим есть ли ответ от Slave , если есть то запишем результат и запусти новый цикл
    if(OpticCMD == YPcmd)
    {
      SettingPrm.PowSc[Ind_LW] = fParamOpt;// запоминаем значение своего источника
      OpticCMD = NOTcmd;
      Cnt_SPw++;
      GetSetLW_SC (1, 1); //установим следующий лазер которым светить 0-первый,1-с маской разрешенных
      Mod_At_cnt=0; // сброс счетчика таймера команды
      
      // memory!!!
      NeedSaveChange = 0x10; // сохраним мощность источника своего переденного от Slave
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
      NeedReDraw = 10;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      if (Cnt_SPw == NumChoiceLS) // все передали и приняли
      {
        //myBeep(200); // можно измерить длину! (откалиброваться!!!)
        ControlSC (0); // управление источником -  выключаем
        Mod_At = LnR_Clb; // переключимся в режим и установим признак первого входа
        //Err_Cmd_At=0; // сброс ошибок приема в режиме  At
        //Start_Cmd = 6; // задаем тик цикла в котором передаем команду 0.25 сек
        //Mod_At_cnt=0; // сброс счетчика таймера команды
        // перестраиваем счетчики
        TIM2->CNT = 0; // обнуляем таймер измерения длины (один импульс 25 тиков)
        TIM3->CNT = 0;// счетчик ожидания ответных импульсов
        TIM2->PSC = 0; // 0- 72MHz
        TIM3->PSC = 0;
        
        En_TX_Cmd = 0;
        FirstInput=1; 
        CntLnCount = 0; // сброс счетчика циклов измерения длины
        //memset(DataLN,0, ARRAY_SIZE(DataLN)); // чистим массив данных приема
        memset(DataLN,0, NUMMEASLN*SEQMEASLN*2); // чистим массив данных приема
        
      }
      
    }
    // предельное время ожидания ответов
    if (Mod_At_cnt>teSavPw) // нет ответа о мощности источника 5 sec
    {
      Mod_At_cnt = MaxAT;
      Mod_At = Wt;
      OpticCMD=NOTcmd; //05.05.2023 сбросим принятые команды сами от себя... при неответе от Slave
      memcpy(MsgAuto[0],Msg[4],strlen(Msg[4])); // ошибка
      memcpy(MsgAuto[1],Msg[5],strlen(Msg[5])); // калибровки
      NeedReDraw = 11;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
      //Start_Cmd = 0;
      //sprintf(Str4, "    ошибка    ");
      //sprintf(Str5, "  калибровки  ");
    }
    //      sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,OpticCMD,Strf);//тест вывод
    
    break; // SavPw
    //_____________________->>> LnR_Clb калибровка длины МАСТЕР. и основной режим измерения длины
  case LnR_Clb:
  case LnR:
    if(FirstInput)
    {
      if(Mod_At == LnR_Clb)
      {
        memcpy(MsgAuto[0],Msg[6],strlen(Msg[6])); // калибровка длины
        memcpy(MsgAuto[1],Msg[7],strlen(Msg[7]));
      }
      SW5V(1);// ON 5V
      Err_Cmd_At=0; //сброс ошибок приема в режиме  At
      // Передача команды Slave
      Start_Cmd = tsLnRClb; // задаем тик цикла в котором передаем команду для переключения Slave в режим измерения длины
      Mod_At_cnt=0; // сброс счетчика таймера команды
      Ind_LW = 1; //установим лазер которым светить - второе место!!!
      // возможно надо выключить источник
      PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
      FirstInput=0; 
      NeedReDraw = 12;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
      
      // может тут надо включить питание для работы измерителя длины
    }
    else // цикл измерения длины
    {
      if(En_TX_Cmd)
      {
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
        //Dummy = huart2.Instance->DR ; // чистим буффер приема 
        Dummy = Clr_UART; // чистим буффер приема 
        HAL_UART_DMAStop(&huart2);
        
        // лазер установлен, можно передать команду
        sprintf((char*)StrTXOpt, "####SWL\rUUU");//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
        TxOptBusy = 1;
        //TsT(1);
        sprintf((char*)Strf,"%s",StrTXOpt) ;
        Mod_At_cnt++; // +50мС
        En_TX_Cmd = 0;
        lSofn = 1; //
        NeedReDraw = 13;
        TestDraw();
        //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      }
      // время запуска Зондирующего импульса, 
      if(Mod_At_cnt==tziLnRClb) // 1Сек
      {
        Start_Cmd = MaxAT+2; // 
        CountLN=0; // чистим счетчик отражений
        
        TIM2->CNT = 0; // обнуляем таймер измерения длины (один импульс 25 тиков)
        TIM3->CNT = 0;// счетчик ожидания ответных импульсов
        TIM2->PSC = 0;
        TIM3->PSC = 0;
        // ставим таймер 2 на режим мастер запрос, импульс в начале счета
        TIM2->ARR = xImp+20;
        TIM2->CCR1 = 20; // Zimp = 100-20
        uint32_t xWaitA=10000;
        //  WaitCnt =100000;
        NeedTransAnsver=1;// делаем софтовое прерывание и генерим ИМПУЛЬС
        // запускаем таймер DWT (возможно его надо выключить после использования)
        //SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT
        //DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик
        //DWT_CYCCNT = 0;// обнуляем счётчик
        //SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk; // тормознем системный таймер
        HAL_TIM_PWM_Start (&htim3, TIM_CHANNEL_3 ); // вроде как надо запустить таймеры
        //CntrlA(1); // дернем контрольную ножку что запустили таймер и вызываем прерывание для запуска импульса
        Beg_Evnt = DWT_CYCCNT; // запоминаем текущий счетчик
        // различия для разных процессоров
#if defined (STM32F103xE) || defined (STM32F103xG) || defined(STM32F303xE) || defined(STM32F205xx)
        EXTI->SWIER|=4; // запускаем программное прерывание как бы от внешнего сигнала для запуска TIM2
#endif        
#if defined (STM32L476xx) 
        EXTI->SWIER1|=4; // запускаем программное прерывание как бы от внешнего сигнала для запуска TIM2
#endif        
        //HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_1 ); // вроде как надо запустить таймеры
        // здесь бы потупить или минимизировать процессы
        while(xWaitA--); // ждем ответа 1.38 мС
        //SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk; // снова разрешим системный таймер
        //CntrlA(0); // опустим контрольную ножку что выждали время для ответа 
        
        //HAL_Delay(10); 
        // тестовый вывод измеренной информации
        //sprintf((void*)StrAnsverData,"%d,%d,%d,%d\r",DataLN[CntLnCount][0],DataLN[CntLnCount][1],DataLN[CntLnCount][2],DataLN[CntLnCount][3]);
        //HAL_UART_Transmit_DMA(&huart1, (void*)StrAnsverData,strlen((void*)StrAnsverData)); // выдаем 
        if(CntLnCount<NUMMEASLN-1)// повторим измерение длины (пока для калибровки), всего 8 раз
        {
          Mod_At_cnt--;// вернем время назад, снова запустим измерение
          // есть блок данных ответов от Slave , передадим их через UART1 наружу
          CntLnCount++;
        }
        else
          Mod_At_cnt++;
        
        
      }
      // время проверки ответов и рассчет
      IndxValidLN = 0; // найдем верные данные
      if(Mod_At_cnt==teLnRClb) // 1,5 Сек// время выхода из режима
      {
        // Здесь надо поветвится в зависимости от режима, калибровка или измерения
        //SCB_DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;// запрещаем использовать DWT
        //DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk; // выключаем счётчик
        
        //TIM2->PSC = 0;
        //TIM3->PSC = 0;
        //
        // вроде должны все принять посмотрим что там есть
        SW5V(0);// OFF 5V
        lSofn = 0; //
        cntr=0;
        Summx=0;
        if(Mod_At == LnR_Clb) // калибровка
        {
          for(int j=0;j<SEQMEASLN;j++) // поищем в третьем измерении
          {
            if((DataLN[2][j]>20300)&&(DataLN[2][j]<20700))
            {
              IndxValidLN = j;
              break;
            }
          }
          
          // Можно посчитать блок данных по линии , здесь тоже можно нулеой не смотреть
          for(int i=1;i<NUMMEASLN;i++)
          {
            if((DataLN[i][IndxValidLN]>20300)&&(DataLN[i][IndxValidLN]<20700))
            {
              Summx +=DataLN[i][IndxValidLN];
              cntr++;
            }
          }
          if(cntr)
            Summx=Summx/cntr;// подсчет мертвой зоны или результаты измерения
          CalibLnAuto=(uint16_t)Summx;
          // надо сохранить в памяти
          UserMeasConfig.CalibLnAutoMem = CalibLnAuto;
          // memory!!!
          NeedSaveChange |=0x01; // сохраняем пользовательские изменения (калибровку длины)
          memcpy(MsgAuto[0],Msg[6],strlen(Msg[6])); // калибровка завершена
          memcpy(MsgAuto[1],Msg[8],strlen(Msg[8]));
          Mod_At_cnt = MaxAT; // закончили
          Mod_At = Wt;
        }
        else // измерения, тут сложнее, надо обработать данные
        {
          for(int j=0;j<SEQMEASLN;j++) // поищем в третьем измерении
          {
            if((DataLN[2][j]>UserMeasConfig.CalibLnAutoMem))
            {
              IndxValidLN = j;
              break;
            }
          }
          // Можно посчитать блок данных по линии , нулевой отбрасываем так как обычно там всякая ФИГНЯ
          for(int i=1;i<NUMMEASLN;i++)
          {
            if((DataLN[i][IndxValidLN]>20300)&&(DataLN[i][IndxValidLN]<61700))
            {
              Summx +=DataLN[i][IndxValidLN];
              cntr++;
            }
          }
          if(cntr)
            Summx=Summx/cntr;// подсчет результата измерения
          // теперь сформируем зачения для индикации.
          MeasLenghtAuto=((uint16_t)Summx>CalibLnAuto)?((uint16_t)(((uint16_t)Summx-CalibLnAuto)*DeviceConfig.KGen)):(0);
          PonA.LnA =((uint16_t)Summx>CalibLnAuto)?((uint16_t)(((uint16_t)Summx-CalibLnAuto)*DeviceConfig.KGen)):(0);
          Mod_At_cnt = MaxAT; // закончили
          Mod_At = View;
        }
        
        NeedReDraw = 14;
        TestDraw();
        //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
        
      }
    }
    
    break;// LnR_Clb
    //-------------------->>>>  Lnx // измерение длины
  case Lnx:  // (Slave) 
    // время перевода в режим ожидания импульса от Мастера
    if (Mod_At_cnt==tsLnX) // задержка на включение запуск импульса после команды 
    {
      lSofn = 1; //излучение
      
      CountLN=0; // чистим счетчик отражений
      // выключаем питание max856
      //P3OUT |=CS_epm;//-CS -> 1
      //P1OUT &= ~on_Vout;
      //P3OUT |= on_Gen;
      //P1OUT &= ~Izm_ST;        TIM2->CNT = 0; // обнуляем таймер измерения длины (один импульс 25 тиков)
      TIM3->CNT = 0;// счетчик ожидания ответных импульсов
      TIM2->CNT = 0;// счетчик формирования ответного импульса
      TIM2->PSC = 0; // 0 - 72 MHz
      TIM3->PSC = 0;
      WaitCntLnX = 200; // необходимость ответа Slave на принятый импульс от мастера
      //uint32_t xWaitB = 1000000;
      // ставим таймер 2 на режим мастер запрос, импульс в середине счета 32000
      TIM2->ARR = xImp+20000;
      TIM2->CCR1 = 20000; // Zimp = 32000-31920 = 80
      NeedTransAnsver=1; // необходимость ответа Slave на принятый импульс от мастера
      TsT(1); // ответили, перенесем для норм индикации в системный 1мС, взводим в прерывании по приему
      //while(xWaitB--); // Тупим для корректного ответа 
      
      Mod_At_cnt++;
      //TsT(1); // дернем ногой что запустились на ожидание
      //WaitCnt =20000000;
      
      //HAL_TIM_PWM_Start (&htim3, TIM_CHANNEL_3 ); // вроде как надо запустить таймеры, но этот таймер в этом режиме не очень нужен
      //SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk; // тормознем системный таймер
      // здесь бы потупить или минимизировать процессы
      //while(WaitCnt--); // ждем для ответа ~0.2С
      //SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk; // снова разрешим системный таймер
      NeedReDraw = 16; // 123
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод 123
    }
    // время выхода из режима 
    if (Mod_At_cnt>teLnX) // нет больше передачи от slave выходим в View
    {
      NeedTransAnsver=0; // запрет ответа Slave на принятый импульс от мастера
      TsT(0); // ответили, перенесем для норм индикации в системный 1мС, взводим в прерывании по приему
      //memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // ожидание команд
      //memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
      //TIM2->PSC = 0; // stop timers
      //TIM3->PSC = 0;
      SW5V(0);// OFF 5V
      //TsT(0); // ответили 
      Mod_At = Wt;
      Mod_At_cnt=MaxAT;
      lSofn = 0; //
      FirstInput=1; 
      NeedReDraw = 15;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    
    break;  // Lnx
    //________________ CalPw ответ SLAVE в режиме ожидания на измерение мощности
    // источника мастера
    // тот же алгоритм для Px в АВТОМАТЕ
  case CalPw:
  case Px:
    
    if(En_TX_Cmd) // тик 40 (2сек) передаем ответ
    {
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
      //Dummy = huart2.Instance->DR ; // чистим буффер приема 
      Dummy = Clr_UART; // чистим буффер приема 
      
      // лазер установлен, можно передать команду
      sprintf((char*)StrTXOpt, "####YP%.2f\rUUU", Pow_SC_Tx);//
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
      TxOptBusy = 1;
      //TsT(1);
      En_TX_Cmd = 0;
      sprintf((char*)Strf,"%s",StrTXOpt) ;
      //memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // ожидание команд
      //memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
      Mod_At_cnt=MaxAT;
      Mod_At = Wt;
      lSofn = 0; //
      FirstInput=1; 
      NeedReDraw = 17;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    // время измерения мощности от мастера
    if (Mod_At_cnt==tsCalPw) //фиксируем в памяти что передавать
    {
      //P2OUT |= LD_R; 
      Pow_SC_Tx = P2dBmValue; // фиксируем значение P2 на нужной длине волны
      Mod_At_cnt++; // ускоряем переход
      lSofn = 1; //
      // test draw
      CntrlA(1); // дернем контрольную ножку 
      //      sprintf((char*)MsgAuto[2], "%d %.2f %d",PMWavelenght, Pow_SC_Tx, TestCntWait);//
      
      NeedReDraw = 18;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      //P2OUT &=~LD_R;
    }
    break; // CalPw    
    //_____________________->>> измерение ORL МАСТЕР
  case Orl:
    if(FirstInput)
    {
      memcpy(MsgAuto[0],Msg[13],strlen(Msg[13])); // измерение (првая строчка)
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
      //старт как при калибровке
      
      Cnt_SPw = 0; // сброс индекса перебора лазеров
      Pow_SC_Tx = 0.0; // мощность принятого источника по умолчанию
      Err_Cmd_At = 0; //сброс ошибок приема в режиме  At
      Start_Cmd = tsORL; // задаем тик цикла в котором передаем команду для переключения Slave в режим измерения мощности от Мастера
      Mod_At_cnt = 0; // сброс счетчика таймера команды
      GetSetLW_SC (0, 1); //установим первый лазер которым светить 0-первый,1-с маской разрешенных, здесь устанавливается Ind_LW
      FirstInput = 0; 
      NeedReDraw = 19;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      // 
    }
    else
    {
      // измерение ORL и получение затухания от Slave
      if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // разрешено передавать команду и не все источники то передаем
      {   
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
        //Dummy = huart2.Instance->DR ; // чистим буффер приема 
        Dummy = Clr_UART; // чистим буффер приема 
        
        // лазер установлен, можно передать команду
        sprintf((char*)StrTXOpt, "####SLA%4u %.2f\rUUU",DeviceConfig.PlaceLS[Ind_LW],SettingPrm.PowSc[Ind_LW]);//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
        TxOptBusy = 1;
        //TsT(1);
        sprintf((char*)Strf,"%s",StrTXOpt) ;
        Mod_At_cnt++; // +50мС
        tSt_Cnt++;
        En_TX_Cmd = 0;
        lSofn = 1; //
        PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
        IndxP2LW = GetIndxLW(PMWavelenght);
        Indxi = IndxP2LW; // // установим индекс выбранного источника для измерения
        
        MsgStat[Ind_LW] = 'o'; // 
        //MsgStat[Ind_LW] = 48+Ind_LW; // 0-1-2
        sprintf((char*)MsgAuto[1], "%s",MsgStat);
        //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
        NeedReDraw = 20;
        TestDraw();
        //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
        
      }
      else 
      {
      } 
      // контролируем счетчик для установки постоянного излучения (
      if(TxOptBusy==0)
      {
        // время включения источника
        // время выключения источника
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
      else
      {
        CW_S(0);
      }
    }
    // Здесь еще надо снять ORL 
    if(Mod_At_cnt==tseSavPw+35)
    {
      
      //PonA.ORL_AB[Ind_LW]=ORLCalc ; // так как он считается всегда! 
      PonA.ORL_AB[Indxi]=ORLCalc ; // так как он считается всегда! 
      Mod_At_cnt++;
      //  NeedReDraw = 40;
      //  sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    
    // проверим есть ли ответ от Slave , если есть то запишем результат и запусти новый цикл
    if(OpticCMD == YPcmd)
    {
      //PonA.PW_AB[Ind_LW] = fParamOpt;
      PonA.PW_AB[Indxi] = fParamOpt; //пишем в поле соотв калибровоной волны
      //PonA.LS_AB[Ind_LW] = PonA.PW_AB[Ind_LW] - SettingPrm.PowSc[Ind_LW];// запоминаем значение уровня помереянного Slave , вычислим затухание
      PonA.LS_AB[Indxi] = PonA.PW_AB[Indxi] - SettingPrm.PowSc[Ind_LW];// запоминаем значение уровня помереянного Slave , вычислим затухание
      
      OpticCMD = NOTcmd;
      Cnt_SPw++;
      GetSetLW_SC (1, 1); //установим следующий лазер которым светить 0-первый,1-с маской разрешенных
      Mod_At_cnt=0; // сброс счетчика таймера команды
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
      NeedReDraw = 21;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      if (Cnt_SPw == NumChoiceLS) // все передали и приняли
      {
        //myBeep(200); // можно продолжить измерения
        ControlSC (0); // управление источником -  выключаем
        Mod_At = CmdSw; // переключимся в режим переключения Slave в режим "главного" ,
        // управляет Мастером для измерения затухания и ORL со стороны Slave
        // команда от мастера должна быть подтверждена Slave
        Mod_At_cnt=0; // сброс счетчика таймера команды
        Start_Cmd = tsCmdSw; // 1
        MsgStat[3] = 'o'; // 
        sprintf((char*)MsgAuto[1], "%s",MsgStat);
        NeedReDraw = 22;
        TestDraw();
        //        sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      }
      
    }
    // предельное время ожидания ответов
    if (Mod_At_cnt>teSavPw) // нет ответа о мощности источника 5 sec
    {
      Mod_At_cnt = MaxAT;
      Mod_At = Wt;
      memcpy(MsgAuto[0],Msg[4],strlen(Msg[4])); // ошибка
      memcpy(MsgAuto[1],Msg[15],strlen(Msg[15])); // нет связи
      NeedReDraw = 23;
      CntErrAuto++;
      OpticCMD = NOTcmd; // сбросим ЭХО от собственного посыла
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
      //Start_Cmd = 0;
      //sprintf(Str4, "    ошибка    ");
      //sprintf(Str5, "  калибровки  ");
    }
    break; // Orl
    //_____________________->>> МАСТЕР передает управление Slave
  case CmdSw:
    if (En_TX_Cmd) // разрешено передавать команду 
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
      //Dummy = huart2.Instance->DR ; // чистим буффер приема 
      Dummy = Clr_UART; // чистим буффер приема 
      
      // лазер установлен, можно передать команду
      sprintf((char*)StrTXOpt, "####SWM\rUUU");//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
      TxOptBusy = 1;
      //TsT(1);
      sprintf((char*)Strf,"%s",StrTXOpt) ;
      Mod_At_cnt++; // +50мС
      En_TX_Cmd = 0;
      lSofn = 1; //
      MsgStat[4] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
      NeedReDraw = 24;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    if(Mod_At_cnt==tsoCmdSw) // 5
      // закончили передачу (выключим значек передачи)
    {
      lSofn = 0; //
      NeedReDraw = 1;
    }
    // проверим есть ли ответ от Slave , если есть 
    if(OpticCMD == SOKcmd)
    {
      Mod_At = Sl_Px; // перевод мастера в режим измерения мощности от Slave, 
      Cnt_BA = 0;
      Pow_SC_Tx = 0.0;
      Err_Cmd_At = 0;
      MsgStat[5] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 25;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    // предельное время ожидания ответов
    if (Mod_At_cnt>teCmdSw) // нет ответа 20 tick
    {
      Mod_At_cnt = MaxAT;
      Mod_At = Wt;
      memcpy(MsgAuto[0],Msg[4],strlen(Msg[4])); // ошибка
      memcpy(MsgAuto[1],Msg[15],strlen(Msg[15])); // нет связи
      NeedReDraw = 26;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    
    break; // CmdSw
    //_____________________->>> МАСТЕР измеряет мощности от Slave, ждет команды SLA
    // с параметрами передаваемого источника (длина волны и мощность, что бы рассчитать затухание)
  case Sl_Px:
    // проверим есть ли запрос от Slave , если есть то пустим ожидание считывания данных
    if(OpticCMD == SLAcmd)
    {
      NeedReadPWSlave = 1;
      PMWavelenght = iParamOpt; // в строку вывода записываем длину волны передаваемого источника от МАСТЕРА
      LWSlaveSRC[Cnt_BA] = PMWavelenght; // длина волны источника Slave в режиме АВТОМАТ
      IndxP2LW = GetIndxLW(PMWavelenght); // получим индекс длины волны для правильного измерения данных в P2
      Indxi = IndxP2LW;
      PonA.LW_BA[Cnt_BA] = PMWavelenght; // длина волны источника Slave в режиме АВТОМАТ
      PonA.PS_BA[IndxP2LW] = fParamOpt; // мощность источника Slave в режиме АВТОМАТ
      // второй параметр передаваемая мощность в принципе не важна
      MsgStat[6+Cnt_BA] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 27;
      OpticCMD = NOTcmd;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
    }
    if((Mod_At_cnt==40) && NeedReadPWSlave)// рассчитаем и запишем затухание BA
    {
      NeedReadPWSlave = 0;
      PonA.PW_BA[Indxi] = P2dBmValue;
      PonA.LS_BA[Indxi] = PonA.PW_BA[Indxi]-PonA.PS_BA[Indxi]; // отрицательный
      // вот тут после приема не обязательно сбрасывать счетчик!!!
      // он сбросится в следующем приеме указаний измерения на следующей длине волны, если надо!
      // это должно быть не позднее 3.5 сек(~70) от предыдущих измерений
      Mod_At_cnt = 0; // можно закомитить
      Cnt_BA++;
    }
    // до 3 сек (~60) slave еще светит надо чуток потупить...
    // 123 - вот тут надо посмотреть не много ли времени отдаем на ожидание,
    // это мастер измеряет от Раба, мощности и измеряет затухание ... можно переключаться назад быстрее...
    // если выше закомментировано то ждем до 5 сек от последнего принятого (слэйф излучает на менее 3 сек, надо плюс подождать...)
    if(Mod_At_cnt>100) // долго ничего нет , выходим снова в режим "ГЛАВНЫЙ" и продолжаем измерения
      // надо запросить измернные ORL со стороны В
    {
      Mod_At = AskOrl; // перевод мастера в режим запроса ORL от Slave от Slave, 
      Start_Cmd = tsCmdSw; // 1
      Cnt_BA = 0;
      Pow_SC_Tx = 0.0;
      Err_Cmd_At = 0;
      MsgStat[9] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      Mod_At_cnt = 0;
      NeedReDraw = 28;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
    }
    break; // Sl_Px
    //_____________________->>>  Master, Формирует команду SRL
  case AskOrl:
    if (En_TX_Cmd) // разрешено передавать команду 
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
      //Dummy = huart2.Instance->DR ; // чистим буффер приема 
      Dummy = Clr_UART; // чистим буффер приема 
      
      // лазер установлен, можно передать команду
      sprintf((char*)StrTXOpt, "####SRL\rUUU");//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
      TxOptBusy = 1;
      //TsT(1);
      sprintf((char*)Strf,"%s",StrTXOpt) ;
      Mod_At_cnt++; // +50мС
      Start_Cmd = MaxAT+2; // чтобы больше ничего не передавать
      Cnt_BA=0;
      En_TX_Cmd = 0;
      
      lSofn = 1; //
      MsgStat[10] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      //memcpy(MsgAuto[1],MsgStat,strlen(MsgStat)); 
      NeedReDraw = 29;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    // ждем данных ответы от Slave (и пишем в память)
    if(OpticCMD == SLOcmd)
    {
      PMWavelenght = iParamOpt; // в строку вывода записываем длину волны передаваемого источника от МАСТЕРА
      //LWSlaveSRC[Cnt_BA] = PMWavelenght; // длина волны источника Slave в режиме АВТОМАТ
      IndxP2LW = GetIndxLW(PMWavelenght); // получим индекс длины волны для правильного измерения данных в P2
      PonA.ORL_BA[IndxP2LW] = fParamOpt; // мощность источника Slave в режиме АВТОМАТ
      MsgStat[11+Cnt_BA] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      // что-то приняли сбросим для другого приема... 
      Mod_At_cnt = 0;
      NeedReDraw = 30;
      OpticCMD = NOTcmd;
      Cnt_BA++;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    // в123 Вот тут также что-то ждем долго, можно сильно сократить
    // на время ожидания одной передачи значения ORL (20) 1 сек ????
    if(Mod_At_cnt>100) // долго ничего нет , выходим в режим  и продолжаем измерения
    //if(Mod_At_cnt>teAnsOrl) // долго ничего нет , выходим в режим  и продолжаем измерения
      // надо запустить измерение длины
    {
      ControlSC (0); // управление источником -  выключаем
      Mod_At = LnR; // переключимся в режим и установим признак первого входа
      //Err_Cmd_At=0; // сброс ошибок приема в режиме  At
      //Start_Cmd = 6; // задаем тик цикла в котором передаем команду 0.25 сек
      //Mod_At_cnt=0; // сброс счетчика таймера команды
      // перестраиваем счетчики
      TIM2->CNT = 0; // обнуляем таймер измерения длины (один импульс 25 тиков)
      TIM3->CNT = 0;// счетчик ожидания ответных импульсов
      TIM2->PSC = 0; // 0- 72MHz
      TIM3->PSC = 0;
      lSofn = 0; //
      
      En_TX_Cmd = 0;
      FirstInput=1; 
      CntLnCount = 0; // сброс счетчика циклов измерения длины
      //memset(DataLN,0, ARRAY_SIZE(DataLN)); // чистим массив данных приема
      memset(DataLN,0, NUMMEASLN*SEQMEASLN*2); // чистим массив данных приема
      MsgStat[14] = 'o'; // 
      sprintf((char*)MsgAuto[1], "%s",MsgStat);
      NeedReDraw = 31;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    
    break; // AskOrl
    //_____________________->>>  Slave, Формирует ответы на команду SRl
    // запрос измеренных значений ORL Slave, для передачи МАСТЕРУ
  case AnsOrl:
    if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // разрешено передавать команду и не все источники то передаем
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
      //Dummy = huart2.Instance->DR ; // чистим буффер приема 
      Dummy = Clr_UART; // чистим буффер приема 
      
      Indxi = GetIndxLW(DeviceConfig.PlaceLS[Ind_LW]); // получим индекс длины волны для правильного измерения данных в P2
      // лазер установлен, можно передать команду
      sprintf((char*)StrTXOpt, "####SLO%4u %.2f\rUUU",DeviceConfig.PlaceLS[Ind_LW],PonA.ORL_BA[Indxi]);//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
      TxOptBusy = 1;
      //TsT(1);
      sprintf((char*)Strf,"%s",StrTXOpt) ;
      Mod_At_cnt++; // +50мС
      En_TX_Cmd = 0;
      lSofn = 1; //
      NeedReDraw = 32;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    // проверим все ли передали, если да то снова в ожидание
    if(Mod_At_cnt==tnAnsOrl)
    {
      Cnt_SPw++;
      Mod_At_cnt=0; // сброс счетчика таймера команды
      GetSetLW_SC (1, 1); //установим первый лазер которым светить 0-первый,1-с маской разрешенных
      
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
      if (Cnt_SPw == NumChoiceLS) // все передали 
      {
        //myBeep(200); // можно продолжить измерения
        ControlSC (0); // управление источником -  выключаем
        memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // ожидание команд
        memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
        Mod_At_cnt=MaxAT;
        Mod_At = Wt;
        lSofn = 0; //
      }
      NeedReDraw = 33;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
    }
    
    
    break; // AnsOrl
    //_____________________->>>  Slave, Формирует команду SOK
  case AnsOK:
    if (En_TX_Cmd) // разрешено передавать команду 
    {   
      huart2.Init.BaudRate = 1200;
      if (HAL_UART_Init(&huart2) != HAL_OK)
      {
        Error_Handler();
      }
      //HAL_UART_DMAStop(&huart2);
      HAL_UART_DMAStop(&huart2);
      HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
      //Dummy = huart2.Instance->DR ; // чистим буффер приема 
      Dummy = Clr_UART; // чистим буффер приема 
      
      // лазер установлен, можно передать команду
      sprintf((char*)StrTXOpt, "####SOK\rUUU");//UUU ooo1300
      HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
      TxOptBusy = 1;
      //TsT(1);
      sprintf((char*)Strf,"%s",StrTXOpt) ;
      Mod_At_cnt++; // +50мС
      En_TX_Cmd = 0;
      lSofn = 1; //
      NeedReDraw = 34;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    if (Mod_At_cnt>6) // ответ ОК = переключаемся в излучение мощности
    {
      Mod_At = Sl_Orl;
      Cnt_SPw = 0;
      Start_Cmd = tsSavPw; // как при калибровке и ORL мастера, только теперь для Slave
      Mod_At_cnt = 0;
      FirstInput = 1;
      NeedReDraw = 35;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
    }
    break; // AnsOK
    //_____________________->>> измерение ORL SLAVE
  case Sl_Orl:
    if(FirstInput)
    {
      memcpy(MsgAuto[0],Msg[17],strlen(Msg[17])); // излучение 
      memcpy(MsgAuto[1],Msg[14],strlen(Msg[14])); // мощности
      //старт как при калибровке
      
      Cnt_SPw = 0; // сброс индекса перебора лазеров
      Err_Cmd_At = 0; //сброс ошибок приема в режиме  At
      Start_Cmd = tsORL; // задаем тик цикла в котором передаем команду для переключения Slave в режим измерения мощности от Мастера
      Mod_At_cnt = 0; // сброс счетчика таймера команды
      GetSetLW_SC (0, 1); //установим первый лазер которым светить 0-первый,1-с маской разрешенных, здесь устанавливается Ind_LW
      FirstInput = 0; 
      NeedReDraw = 36;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      // 
    }
    else
    {
      // измерение ORL 
      if ((En_TX_Cmd)&&(Cnt_SPw < NumChoiceLS)) // разрешено передавать команду и не все источники то передаем
      {   
        huart2.Init.BaudRate = 1200;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
          Error_Handler();
        }
        //HAL_UART_DMAStop(&huart2);
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_IT(&huart2, RxBufOpt,1); // ждем принятия первого байта из внешнего мира
        //Dummy = huart2.Instance->DR ; // чистим буффер приема 
        Dummy = Clr_UART; // чистим буффер приема 
        
        // лазер установлен, можно передать команду
        sprintf((char*)StrTXOpt, "####SLA%4u %.2f\rUUU",DeviceConfig.PlaceLS[Ind_LW],SettingPrm.PowSc[Ind_LW]);//UUU ooo1300
        HAL_UART_Transmit_DMA(&huart2, (void*)StrTXOpt,strlen((void*)StrTXOpt)); // выдаем 
        TxOptBusy = 1;
        //TsT(1);
        sprintf((char*)Strf,"%s",StrTXOpt) ;
        Mod_At_cnt++; // +50мС
        En_TX_Cmd = 0;
        lSofn = 1; //
        PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
        IndxP2LW = GetIndxLW(PMWavelenght);
        Indxi = IndxP2LW; // // установим индекс выбранного источника для измерения
        NeedReDraw = 37;
        TestDraw();
        //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      }
      else 
      {
      } 
      // контролируем счетчик для установки постоянного излучения (
      if(TxOptBusy==0)
      {
        // время включения источника
        // время выключения источника
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
      else
      {
        CW_S(0);
      }
      
    }
    // Здесь еще надо снять ORL 
    if(Mod_At_cnt==tseSavPw+35)
    {
      PonA.ORL_BA[Indxi]=ORLCalc ; // так как он считается всегда!
      Mod_At_cnt++;
    }
    //закончили светить лазером, переключим + 0.5с
    if(Mod_At_cnt==tsdSavPw+20)
    {
      Cnt_SPw++;
      GetSetLW_SC (1, 1); //установим следующий лазер которым светить 0-первый,1-с маской разрешенных
      Mod_At_cnt=0; // сброс счетчика таймера команды
      //PMWavelenght = DeviceConfig.PlaceLS[Ind_LW]; // в строку вывода записываем длину волны передаваемого источника
      if (Cnt_SPw == NumChoiceLS) // все передали 
      {
        //myBeep(200); // можно продолжить измерения
        ControlSC (0); // управление источником -  выключаем
        memcpy(MsgAuto[0],Msg[0],strlen(Msg[0])); // ожидание команд
        memcpy(MsgAuto[1],Msg[1],strlen(Msg[1]));
        Mod_At_cnt=MaxAT;
        Mod_At = Wt;
        lSofn = 0; //
      }
      NeedReDraw = 38;
      TestDraw();
      //sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
      
    }
    break; // Sl_Orl
  }
//   sprintf((char*)Stre, "%d(%d)%s",Mod_At_cnt,NeedReDraw,Strf);//тест вывод
//   NeedReDraw = 1;
}
