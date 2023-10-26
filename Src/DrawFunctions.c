// Здесь напишем разные функции заполнения строк индикатора

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

// константы единиц измерения при индикации
#define	u_MW    0
#define	u_MKW   1
#define	u_NW    2
#define	u_PW    3
#define	pSR    93 // точка начала установки индикатора излучения
   

   const char* StrP1[]= {"   Измеритель ","    мощности   "};   
   const char* StrP1a[]={"   Измеритель ","   мощн. авто  "};  
   const char* StrRE[]= {"   Визуальный ","    локатор   "};
   const char* StrSc[]= {"   Источник   ","   излучени\xff   "};
   const char* StrP2[]= {"   Индикатор  ","    мощности   "};
   const char* StrLn[]= {"   Измеритель ","    дл. линии   "};
   const char* StrRl[]= {"   Измеритель "," обр.отраж.(ORL)"};
   const char* StrLs[]= {"   Измеритель ","   затухани\xff   "};
   const char* StrAt[]= {"    ТЕСТЕР    ","    АВТОМАТ   "};
   const char* StrScMode[]={"@ OFF","@ CW","@ 270Hz","@ 2kHz","@ CW*"};
   const char* StrVFLMode[]={"@ OFF","@ CW","@ 1Hz"};
   const char* StrNFa[]={"Номер волокна"};
   const char* StrNFb[]={"Авто +"};
   const char* StrdB[]={"dBm","dB","dBe"};
   
     
uint8_t Str[64];
uint8_t Stra[32];
uint8_t Strb[32];
uint8_t Strc[32]; // 
uint8_t Strd[32];
uint8_t Strm[32]; // 
uint8_t Stri[8];
uint8_t Str2[32]; // разрядность индикации в режиме  
uint8_t StrDrM[4]; // строка отображения режима работы на индикаторе
uint8_t StrBat[4]; // строка отображения режима работы на индикаторе

uint8_t StPon1[3][5], StPon2[3][8], StPon3[3][8]; //строки для многострочных выводов из составляющих

uint8_t IndxEditNumFbr; // индекс указателя на редактор элемента
uint8_t IndxEditLOSS; // индекс указателя на редактор элемента LOSS
uint8_t IndxEditLOSSm; // индекс указателя на редактор элемента LOSS MM 850,1300
uint8_t IndxEditORL; // индекс указателя на редактор элемента ORL
uint8_t IndxEnaSave; // индекс указателя на редактор элемента разрешения сохранений
uint8_t IndxPowSC; // индекс указателя на редактор элемента мощности источника
uint8_t IndxClrMEM; // индекс указателя разрешения очистки памяти
uint8_t IndxEditORL; // индекс указателя на редактор элемента ORL
uint8_t IndxInvView; // индекс указателя на инверсию при выводе результатов в автомате (1-первое место, 2-второе, 4-третье)
uint8_t yesLS; // ести такой источник будем выводить
uint8_t CountDrawMsg; // счетчик индикации надписи ( P1 - запись в память)
int iS; // индекс источника при индикации результатов
uint16_t IndxShowLN=0; // индекс показаний измеренных расстояний до отражений

int CntPwrOffCount=20; // счетчик времени автовыключения

/* Экран установки порогов ORL*/
void ShowLimORL(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
  sprintf((char*)Str,"on/off   [%c]", (SettingPrm.EnaCmpORL)?('+'):(' ')) ;
  // значения
  sprintf((char*)Stra,"%.2f",UserMeasConfig.Lim_ORL[0]) ;
  //sprintf((char*)Strb,"%.2f",UserMeasConfig.Lim_ORL[1]) ;
  //sprintf((char*)Strm,"%.2f",UserMeasConfig.Lim_ORL[2]) ;
  sprintf((char*)Strb,"ORL") ;
  sprintf((char*)Strm,"Lim") ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы созранение затухания
  SSD1305_GotoXY(60,12);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,1);// мелкие буквы ORL для редактирования
  SSD1305_GotoXY(13,12);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,1);// мелкие буквы подпись ORL
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// мелкие буквы  lim
  //SSD1305_GotoXY(70,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditORL==2)?(0):(1));// мелкие буквы ORL для второй длины волны
  //SSD1305_GotoXY(70,16);
  //SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxEditORL==3)?(0):(1));// мелкие буквы ORL для третьей длины волны
 // прорисуем длины волн источников
  // значения
  //sprintf((char*)Strb,"%.2f",DeviceConfig.PlaceLS[1]/1000.) ;
  //sprintf((char*)Strm,"%.2f",DeviceConfig.PlaceLS[2]/1000.) ;

  //SSD1305_GotoXY(13,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы для второй длины волны
}
/* Экран установки автовыключения*/
void ShowSetAutoOff(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
  sprintf((char*)Str,"Авто Выкл.[%c]", (UserMeasConfig.AutoOff)?('+'):(' ')) ;
  // значения
  sprintf((char*)Strm,"Выбор ") ;
  sprintf((char*)Stra,"минут") ;
  sprintf((char*)Strb,"%d",2<<UserMeasConfig.AutoOffMod) ; // время авто выкл в минутах 2,4,8,16
  //sprintf((char*)Strm,"%.2f",UserMeasConfig.Lim_LS[4]) ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы созранение затухания
  SSD1305_GotoXY(13,12);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы надпись LOSS
  SSD1305_GotoXY(60,12);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,1);// мелкие буквы LOSS значение
  //SSD1305_GotoXY(70,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditLOSS==2)?(0):(1));// мелкие буквы LOSS для второй длины волны 1490
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,1);// мелкие буквы LOSS для третьей длины волны 1550
 // прорисуем длины волн источников
  // значения
  //sprintf((char*)Stra,"%.2f",CalibrLW[2]/1000.) ;
  //sprintf((char*)Strb,"%.2f",CalibrLW[3]/1000.) ;
  //sprintf((char*)Strm,"%.2f",CalibrLW[4]/1000.) ;

  //SSD1305_GotoXY(13,0);
  //SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы для первой длины волны 1310
  //SSD1305_GotoXY(13,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы для второй длины волны 1490
  //SSD1305_GotoXY(13,16);
  //SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// мелкие буквы  для третьей длины волны 1550
  
  
}

/* Экран установки порогов LOSS с 15.09.2021 для всех длин волн один порог*/
void ShowLimLOSS(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
  sprintf((char*)Str,"On/Off   [%c]", (SettingPrm.EnaCmpLOSS)?('+'):(' ')) ;
  // значения
  sprintf((char*)Strm,"Lim") ;
  sprintf((char*)Stra,"LOSS") ;
  sprintf((char*)Strb,"%.2f",UserMeasConfig.Lim_LS[0]) ; // общий порог для всех длин волн
  //sprintf((char*)Strm,"%.2f",UserMeasConfig.Lim_LS[4]) ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы созранение затухания
  SSD1305_GotoXY(13,12);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы надпись LOSS
  SSD1305_GotoXY(60,12);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,1);// мелкие буквы LOSS значение
  //SSD1305_GotoXY(70,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditLOSS==2)?(0):(1));// мелкие буквы LOSS для второй длины волны 1490
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxEditLOSS==3)?(0):(1));// мелкие буквы LOSS для третьей длины волны 1550
 // прорисуем длины волн источников
  // значения
  //sprintf((char*)Stra,"%.2f",CalibrLW[2]/1000.) ;
  //sprintf((char*)Strb,"%.2f",CalibrLW[3]/1000.) ;
  //sprintf((char*)Strm,"%.2f",CalibrLW[4]/1000.) ;

  //SSD1305_GotoXY(13,0);
  //SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы для первой длины волны 1310
  //SSD1305_GotoXY(13,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы для второй длины волны 1490
  //SSD1305_GotoXY(13,16);
  //SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// мелкие буквы  для третьей длины волны 1550
  
  
}
/* Экран установки порогов LOSS с 15.09.2021 для всех длин волн один порог*/
void ShowLimLOSSm(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
    sprintf((char*)Str,"LOSS  MM") ;

  // значения
  sprintf((char*)Stra,"%.2f",UserMeasConfig.Lim_LS[0]) ;
  sprintf((char*)Strb,"%.2f",UserMeasConfig.Lim_LS[1]) ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы для индикации LOSS MM
  SSD1305_GotoXY(70,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(IndxEditLOSSm==0)?(0):(1));// мелкие буквы LOSSm для первой длины волны 850
  SSD1305_GotoXY(70,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditLOSSm==1)?(0):(1));// мелкие буквы LOSSm для второй длины волны 1300
 // прорисуем длины волн источников
  // значения
  sprintf((char*)Stra,"%.2f",CalibrLW[0]/1000.) ;
  sprintf((char*)Strb,"%.2f",CalibrLW[1]/1000.) ;

  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы для первой длины волны 850
  SSD1305_GotoXY(13,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы для второй длины волны 1300
}  
  

/* Экран установки переключачелей разрешения сохранения LOSS, ORL, Ln*/
void ShowEnaSave(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
  sprintf((char*)Str,"Сохр. в реж.А") ;
  sprintf((char*)Stra,"Loss   ") ;
  sprintf((char*)Strb,"ORL    ") ;
  sprintf((char*)Strm,"Ln     ") ;
  // значения
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы для первой длины волны
  SSD1305_GotoXY(13,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы для второй длины волны
  SSD1305_GotoXY(13,16);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// мелкие буквы  для третьей длины волны
  SSD1305_GotoXY(13,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2,(1));// мелкие буквы  для третьей длины волны

  
  sprintf((char*)Stra,"[%c]", (SettingPrm.EnaWrtMem&0x1)?('+'):(' ')) ;
  sprintf((char*)Strb,"[%c]", (SettingPrm.EnaWrtMem&0x2)?('+'):(' ')) ;
  sprintf((char*)Strm,"[%c]", (SettingPrm.EnaWrtMem&0x4)?('+'):(' ')) ;

  SSD1305_GotoXY(70,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(IndxEnaSave==1)?(0):(1));// мелкие буквы LOSS для первой длины волны
  SSD1305_GotoXY(70,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEnaSave==2)?(0):(1));// мелкие буквы LOSS для второй длины волны
  SSD1305_GotoXY(70,16);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxEnaSave==3)?(0):(1));// мелкие буквы LOSS для третьей длины волны
}
/* Экран работы с памятью*/
void ShowMemory(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
  sprintf((char*)Str,"Память инфо") ;
  SSD1305_GotoXY(12,0);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
  sprintf((char*)Str,"! Занято !") ;
  SSD1305_GotoXY(12,8);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
  sprintf((char*)Str,"%d из 512",SettingPrm.CountMemory) ;
  SSD1305_GotoXY(12,16);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
  sprintf((char*)Str,"Очистить ?") ;
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, (IndxClrMEM)?(0):(1));
}
/* Экран установки отображение идентификатора */
void ShowVer(void)
{
  SSD1305_Fill(0); /* очистка экрана*/
  // значения
  //            sprintf ((void*)AnsData, "T7K,%s,(%d/%c:%d/%c:%d/%c)%s%s%s,%04d,SW_rev 1.%04d\r", // убираем VFL из идентификатора -VFL (у чехов оставим)
  //                   DeviceConfig.NameDevice,
  //                   (DeviceConfig.PlaceLS[0]%1000)/10,
  //                   (DeviceConfig.CfgOUT[0])?('1'):('0'), // 
  //                   (DeviceConfig.PlaceLS[1]%1000)/10,
  //                   (DeviceConfig.CfgOUT[1])?('1'):('0'), // 
  //                   (DeviceConfig.PlaceLS[2]%1000)/10,
  //                   (DeviceConfig.CfgOUT[2])?('1'):('0'), // 
  //                   (DeviceConfig.CfgPM)?((DeviceConfig.CfgPM-1)?("-PMH"):("-PM")):(""),
  //                   (DeviceConfig.CfgL)?("-L"):(""), // 
  //                   (DeviceConfig.CfgRE)?("-VFL"):(""),
  //                   DeviceConfig.NumDevice,
  //                   NumVer);//(g_ConfigDevice.ID_Device)?("EN"):("RU")
  
  sprintf((char*)Stra,"%s",DeviceConfig.NameDevice) ; // 1 строка
  sprintf((char*)Strb,"%d/%c:%d/%c:%d/%c",
          (DeviceConfig.PlaceLS[0]%1000)/10,
          (DeviceConfig.CfgOUT[0])?('1'):('0'), // 
          (DeviceConfig.PlaceLS[1]%1000)/10,
          (DeviceConfig.CfgOUT[1])?('1'):('0'), // 
          (DeviceConfig.PlaceLS[2]%1000)/10,
          (DeviceConfig.CfgOUT[2])?('1'):('0')); // 2 строка
  sprintf((char*)Strm,"%s%s%s",
          (DeviceConfig.CfgPM)?((DeviceConfig.CfgPM-1)?("-PMH"):("-PM")):(""),
          (DeviceConfig.CfgL)?("-L"):(""), // 
          (DeviceConfig.CfgRE & 0x1)?("-VFL"):("")) ; // 3 строка
  sprintf((char*)Str,"№%04d ver 1.%04d",
          DeviceConfig.NumDevice,
          NumVer) ; // 4 строка
  
  
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы 1 строки
  SSD1305_GotoXY(13,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы 2 строки
  SSD1305_GotoXY(13,16);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// мелкие буквы  3 строки
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы 4 строки
}
  /* Экран установки уровней Лазеров*/
  void ShowPowSC(void)
  {
    SSD1305_Fill(0); /* очистка экрана*/
    sprintf((char*)Str,"Мощн. источнков") ;
    // значения
    sprintf((char*)Stra,"%.2f",SettingPrm.PowSc[0]) ;
    sprintf((char*)Strb,"%.2f",SettingPrm.PowSc[1]) ;
    sprintf((char*)Strm,"%.2f",SettingPrm.PowSc[2]) ;
    
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы созранение затухания
    SSD1305_GotoXY(70,0);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2,(IndxPowSC==1)?(0):(1));// мелкие буквы Power для первой длины волны
    SSD1305_GotoXY(70,8);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxPowSC==2)?(0):(1));// мелкие буквы Power для второй длины волны
    SSD1305_GotoXY(70,16);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxPowSC==3)?(0):(1));// мелкие буквы Power для третьей длины волны
    // прорисуем длины волн источников
    // значения
    sprintf((char*)Stra,"%.2f",DeviceConfig.PlaceLS[0]/1000.) ;
    sprintf((char*)Strb,"%.2f",DeviceConfig.PlaceLS[1]/1000.) ;
    sprintf((char*)Strm,"%.2f",DeviceConfig.PlaceLS[2]/1000.) ;
    
    SSD1305_GotoXY(13,0);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// мелкие буквы для первой длины волны
    SSD1305_GotoXY(13,8);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// мелкие буквы для второй длины волны
    SSD1305_GotoXY(13,16);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// мелкие буквы  для третьей длины волны
  }
  
  /* Экран установки номера волокна, */
  void ShowNumFbr(void)
  {
    SSD1305_Fill(0); /* очистка экрана*/
    sprintf((char*)Str,"Номер волокна") ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    // распишем по символьно номер волокна
    sprintf((char*)Str,"%d", (SettingPrm.NumberFbrCount/10000)%10) ;
    SSD1305_GotoXY(48,10);
    SSD1305_PutsN((void*)Str, (void*)TabSG2,(IndxEditNumFbr==5)?(0):(1));
    sprintf((char*)Str,"%d", (SettingPrm.NumberFbrCount/1000)%10) ;
    SSD1305_GotoXY(55,10);
    SSD1305_PutsN((void*)Str, (void*)TabSG2,(IndxEditNumFbr==4)?(0):(1));
    sprintf((char*)Str,"%d", (SettingPrm.NumberFbrCount/100)%10) ;
    SSD1305_GotoXY(62,10);
    SSD1305_PutsN((void*)Str, (void*)TabSG2,(IndxEditNumFbr==3)?(0):(1));
    sprintf((char*)Str,"%d", (SettingPrm.NumberFbrCount/10)%10) ;
    SSD1305_GotoXY(69,10);
    SSD1305_PutsN((void*)Str, (void*)TabSG2,(IndxEditNumFbr==2)?(0):(1));
    sprintf((char*)Str,"%d", SettingPrm.NumberFbrCount%10) ;
    SSD1305_GotoXY(76,10);
    SSD1305_PutsN((void*)Str, (void*)TabSG2,(IndxEditNumFbr==1)?(0):(1));
    sprintf((char*)Str,"Авто+ [%c]",(SettingPrm.EnaAutoInkrNF)?('+'):(' ') ) ;
    SSD1305_GotoXY(12,20);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    
  }
  
  /* Экран установки времени, */
  void ShowCurrTime(void)
  {
    SSD1305_Fill(0); /* очистка экрана*/
    sprintf((char*)Str,"Установка часов") ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    //sprintf((char*)Str,"    времени  ") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));
    //SSD1305_GotoXY(12,8);
    //SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)StPon1[0],"20%02d",Clocks.cYear) ; // Year
    SSD1305_GotoXY(24,12);
    SSD1305_PutsN((void*)StPon1[0], (void*)TabSG2,(IndxEditClock==1)?(0):(1));
    sprintf((char*)Str,"-") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));
    SSD1305_GotoXY(48,12);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)StPon1[1],"%02d",Clocks.cMonth) ; // Month
    SSD1305_GotoXY(54,12);
    SSD1305_PutsN((void*)StPon1[1], (void*)TabSG2,(IndxEditClock==2)?(0):(1));
    sprintf((char*)Str,"-") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));
    SSD1305_GotoXY(66,12);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)StPon1[2],"%02d",Clocks.cDay) ; // Day
    SSD1305_GotoXY(72,12);
    SSD1305_PutsN((void*)StPon1[2], (void*)TabSG2,(IndxEditClock==3)?(0):(1));
    sprintf((char*)StPon1[0],"%02d",Clocks.cHour) ; // Hour
    SSD1305_GotoXY(36,24);
    SSD1305_PutsN((void*)StPon1[0], (void*)TabSG2,(IndxEditClock==4)?(0):(1));
    sprintf((char*)Str,":") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));
    SSD1305_GotoXY(48,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)StPon1[1],"%02d",Clocks.cMinutes) ; // Minutes
    SSD1305_GotoXY(54,24);
    SSD1305_PutsN((void*)StPon1[1], (void*)TabSG2,(IndxEditClock==5)?(0):(1));
    sprintf((char*)Str,":") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));
    SSD1305_GotoXY(66,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)StPon1[2],"%02d",Clocks.cSeconds) ; // Seconds
    SSD1305_GotoXY(72,24);
    SSD1305_PutsN((void*)StPon1[2], (void*)TabSG2,(IndxEditClock==6)?(0):(1));
    //  SSD1305_DrawRectangle(12,0,104,32,1);
    
  }
  
  /* Экран заставки, шрифты TabSG1 изменены - заужены
  шрифт не содержит полный алфавит*/
  void ShowWelcome(void)
  {
    SSD1305_Fill(0); /* очистка экрана*/
    sprintf((char*)Str,"№%04d 20.5%s%s",DeviceConfig.NumDevice,(DeviceConfig.CfgL)?("L"):(""),(DeviceConfig.CfgRE & 0x1)?("R"):("")) ;
    SSD1305_GotoXY(14,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)Str,"%02dг",DeviceConfig.YearManuf) ;
    SSD1305_GotoXY(94,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)Str,"НПК СвязьСервис") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));    // формат вывода, загрузка строки функцией sprintf, определение координат размещения строки, вывод строки в нужном шрифте
    //sprintf((char*)Str,"Базовый Base") ;
    SSD1305_GotoXY(13,16);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)Str,"%s",DeviceConfig.NameDevice) ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG1, 1);
    //  SSD1305_DrawRectangle(12,0,104,32,1);     // тест внешнего периметра отображения
    
  }
  
  // заполнение строки указателя базового уровня в измерителе Р1
  // поиск и привязкановог базовог уровня при изменении
  float ShowBaseLevel (uint16_t LW)
  {
    return 0.0;
  }
  
  /* индикация измерителя P1 упрощенная*/
  void ShowPM(uint8_t ModInd, uint16_t LW_I)
  {
    // Выводим значения показаний в зависимости от режима индикации  ModeI , остальные значения не меняются dBm, dB, W, и длины волны
    float Tmp1;
    //static double OptPow;
    int8_t OW; // 
    //  OptPow = GetPMValue()*CurrSpectrKoeff;//*GetCoeffSpctr(LW_I);//приведенное значение мощности в миливатах, для данной длины волны
    //  Opt_dBm = GetPMValuedBm(OptPow);
    //  Opt_dB = GetPMValuedB(OptPow);
    
    // установка предела индикации
    switch (ModInd)
    {
    case W:
      if (OptPow!=0)
      {
        Tmp1=log10(OptPow);
        OW=(int)Tmp1;
        if (Tmp1<0) OW--;
      }
      else OW=-6;
      switch (OW){
      case 2:
        sprintf((void*)Str2, " %5.1fmW ", OptPow);
      case 1:
        sprintf((void*)Str2, " %5.2fmW ", OptPow);
        break;
      case 0:
        sprintf((void*)Str2, " %5.3fmW ", OptPow);
        break;
      case -1:
        OptPow=OptPow*1000;
        sprintf((void*)Str2, " %5.1fuW ", OptPow);
        //микроватты
        break;
      case -2:
        OptPow=OptPow*1000;
        sprintf((void*)Str2, " %5.2fuW ", OptPow);
        //микроватты
        break;
      case -3:
        OptPow=OptPow*1000;
        sprintf((void*)Str2, " %5.3fuW ", OptPow);
        //микроватты
        break;
      case -4:
        OptPow=OptPow*1000000;
        sprintf((void*)Str2, " %5.1fnW ", OptPow);
        //нанноваты
        break;
      case -5:
        OptPow=OptPow*1000000;
        sprintf((void*)Str2, " %5.2fnW ", OptPow);
        //нанноваты
        break;
      default:
        OptPow=OptPow*1000000;
        sprintf((void*)Str2, " %5.3fnW ", OptPow);
        //нанноваты
        break;
      }
      // Str1[5]=' ';
      // Str1[6]=' ';
      // Str1[7]=' ';
      break;
    case dBm:
      if((Opt_dBm>=-10.) && (Opt_dBm<=10.))
        sprintf((void*)Str2, "%6.3fdBm", Opt_dBm);
      else
        sprintf((void*)Str2, "%6.2fdBm", Opt_dBm);
      break;
    case dB:
      if((Opt_dB>=-10.) && (Opt_dB<=10.))
        sprintf((void*)Str2, "%6.3fdB", Opt_dB);
      else
        sprintf((void*)Str2, "%6.2fdB", Opt_dB);
      break;
    }
    SSD1305_Fill(0); /* очистка экрана*/
    
    //выводим показания
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str2, (void*)TabSG3, 1);
    // вывод  уровня опоры (REF)
    sprintf((char*)Strb,"%.1f", CurrBaseLevel) ;
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);
    // вывод текущей длины волны
    sprintf((char*)Strc,"%dнм", LW_I) ;
    //sprintf((char*)Strc,"%d", IN_BuffADC[0]) ;
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);
    // вывод сообщения о записи если надо
    //  if(CountDrawMsg)
    //  {
    //    if(SettingPrm.CountMemory<511) // прог записи
    //    {
    //  sprintf((char*)Str2,"Wr %d", SettingPrm.CountMemory) ;
    //  SSD1305_GotoXY(52,24);
    //  SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
    //    }
    //    else
    //    {
    //  sprintf((char*)Str2,"MemFull") ;
    //  SSD1305_GotoXY(52,24);
    //  SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
    //    }
    //   CountDrawMsg--; 
    //  }
    // индикация работающего лазера
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    
    // тестовый вывод, диапазон и коды АЦП
    //sprintf((char*)Str2,"%d  %08d",GetRange(),GetPMDataNorm()) ;
    //SSD1305_GotoXY(12,24);
    //SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
    // draw icon mode device - & level battery
    //DrawMod (CondWork);
    //ShowBatLvl(BatLevel);
    
    
    
    //  SSD1305_DrawRectangle(0,0,128,64,1);
    
    //sprintf((char*)Str,"-25.39 dBm") ;
    //SSD1305_GotoXY(0,32);
    //SSD1305_Puts((void*)Str, &Font_16x26, 1);
    
  }
  
  /* индикация измерителя P1a упрощенная*/
  void ShowP1a( uint16_t LW_I)
  { 
    int i;
    uint8_t Inv=0;; // признак необходимости инвертирования показаний
    uint8_t M_PON=0; // счетчик показаний не более 3-х
    float LvlInd;
    
    // зачистим строки индикации
    for(i=0; i<3; i++) // 
    {
      sprintf((void*)StPon1[i], "--- ");
      sprintf((void*)StPon2[i], "--- ");
      sprintf((void*)StPon3[i], " ");
    }
    // sprintf((void*)Stra, "      "); // строка индикации уровня в дБм
    for(i=0; i < ARRAY_SIZE(RPON); ++i) // переберем ячейки RPON на наличие значений
    {
      // если есть длина волны в опознании (YesLW != 0) то надо прописать ячейку RPON
      if(( YesLW>100) && (LW_I == CalibrLW[i]))
      {
        RPON[i] = Opt_dBm;
        sprintf((void*)Stra, "%6.2fdBm",RPON[i]); // строка индикации уровня в дБм
        
      }
      if (RPON[i]>-100.0) // есть значение для индикации
      {
        switch(ModeIP1a)
        {
        case dBm:
          LvlInd=RPON[i];
          break;
        case dB:
          LvlInd=RPON[i]-UserMeasConfig.BaseLvl[i];
          break;
        case W:
          LvlInd=RPON[i]-BaseExtLvl[i];
          break;
        }
        switch (M_PON)
        {
        case 0: // первая строчка - отображение
          sprintf((void*)StPon1[0], "%1.2f",CalibrLW[i]/1000.0);
          sprintf((void*)StPon2[0], "%6.2f",LvlInd);
          sprintf((void*)StPon3[0], "%s", StrdB[ModeIP1a]);
          //if ((i>1)&&(i<5)) // попадаем на значения пределов сравнения 1310,1490,1550 только PON!!!
          //if (i<5) // попадаем на значения пределов сравнения 1310,1490,1550 только PON!!!
          //{
            if ((RPON[i]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv |=0x01;
          //}
          
          //sprintf(Str4, "%1.2f %6.2fdB",M_L_W_Klb[i]/1000.0,RPON[i]);// измерение лазером первого места
          M_PON++;
          break;
        case 1:
          sprintf((void*)StPon1[1], "%1.2f",CalibrLW[i]/1000.0);
          sprintf((void*)StPon2[1], "%6.2f",LvlInd);
          sprintf((void*)StPon3[1], "%s",  StrdB[ModeIP1a]);
          //if ((i>1)&&(i<5)) // попадаем на значения пределов сравнения 1310,1490,1550
          //if (i<5) // попадаем на значения пределов сравнения 1310,1490,1550
          //{
            if ((RPON[i]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv |=0x02;
          //}
          
          //sprintf(Str5, "%1.2f %6.2fdB",M_L_W_Klb[i]/1000.0,RPON[i]);// измерение лазером первого места
          M_PON++;
          break;
        case 2:
          sprintf((void*)StPon1[2], "%1.2f",CalibrLW[i]/1000.0);
          sprintf((void*)StPon2[2], "%6.2f",LvlInd);
          sprintf((void*)StPon3[2], "%s",  StrdB[ModeIP1a]);
          //if ((i>1)&&(i<5)) // попадаем на значения пределов сравнения 1310,1490,1550
          //if (i<5) // попадаем на значения пределов сравнения 1310,1490,1550
          //{
            if ((RPON[i]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv |=0x04;
          //}
          
          //sprintf(Str8, "%1.2f %6.2fdB",M_L_W_Klb[i]/1000.0,RPON[i]);// измерение лазером первого места
          M_PON++;
          break;
        default:
          break;
        }
        
      }
    } 
    sprintf((char*)Strc,"№%03d",SettingPrm.CountMemory) ; // строка номера записи

    
    SSD1305_Fill(0); /* очистка экрана*/
    
    // три строки с длинами волн и значениями 
    for(i=0; i<3; i++) // 
    {
      SSD1305_GotoXY(12,i*8);
      SSD1305_PutsN((void*)StPon1[i], (void*)TabSG2, 1); // выводим длину волны
      SSD1305_GotoXY(40,i*8);
      SSD1305_PutsN((void*)StPon2[i], (void*)TabSG2, ((1<<i)&Inv)?(0):(1)); // выводим показание
      SSD1305_GotoXY(72,i*8);
      SSD1305_PutsN((void*)StPon3[i], (void*)TabSG2, 1); // выводим дБ
    }
    // строка с текущей длиной волны и счетчиком ячеек памяти
    // вывод текущей длины волны
    if(LW_I)
      sprintf((char*)Strd,"%dнм", LW_I) ;
    else 
      sprintf((char*)Strd," ----- ") ;
    
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);
    // индикация работающего лазера
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    // индикация уровня в дБм
    //SSD1305_GotoXY(48,24);
    //SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    // индикация о записи в память
    //  if(CountDrawMsg)
    //  {
    //    if(SettingPrm.CountMemory<511) // прог записи
    //    {
    //  sprintf((char*)Str2,"Wr %d", SettingPrm.CountMemory) ;
    //  SSD1305_GotoXY(52,24);
    //  SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
    //    }
    //    else
    //    {
    //  sprintf((char*)Str2,"MemFull") ;
    //  SSD1305_GotoXY(52,24);
    //  SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
    //    }
    //   CountDrawMsg--; 
    //   NeedReDraw=1;
    //  }
      // выводим при просмотре всегда
      SSD1305_GotoXY(87,24);
      SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);//  мелкие буквы номер записи
    
  }
  // вывод сообшщения о записи в одно и то же место!!!
  void ShowMsgWrMem(void)
  {
    if(CountDrawMsg)
    {
      if(CountDrawMsg>1)
      {
      if(SettingPrm.CountMemory<511) // прог записи
      {
        sprintf((char*)Str2,"Wr %d", SettingPrm.CountMemory) ;
        SSD1305_GotoXY(49,24);
        SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
      }
      else
      {
        sprintf((char*)Str2,"MemFull") ;
        SSD1305_GotoXY(49,24);
        SSD1305_PutsN((void*)Str2, (void*)TabSG2, 1);
      }
      }
      else
      {
        CountDrawMsg=1;
      }
      CountDrawMsg--; 
      NeedReDraw=1;
    }
    
  }
  
  // прорисовка источника S
  void ShowSource (void)
  {
    // выводим значек излучения, режим источника
    // длина волны источника, тип источника (Sm/Mm), мощность источника
    // переменные: Ind_LW - индекс текущего рабочего места
    
    sprintf((char*)Strc,"%s",StrScMode[ScMode]) ;
    sprintf((char*)Stra,"%04dнм",DeviceConfig.PlaceLS[Ind_LW]) ;
    sprintf((char*)Strb,"%.2f",SettingPrm.PowSc[Ind_LW]) ;
    sprintf((char*)Strm,"%s",(DeviceConfig.PlaceLS[Ind_LW]>1300)?("S"):("M")) ;
    
    SSD1305_Fill(0); /* очистка экрана*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// крупные буквы Режим источника
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы длина волны
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы мощность источника
    SSD1305_GotoXY(64,24);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы тип источника SM/MM
    // индикация работающего лазера
    sprintf((char*)Strd,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    
  }
  // прорисовка индикатора мощности P2 ()
  void ShowP2 (float MedADC)
  {
    // выводим значение мощности (dB/dBm),
    // длина волны индикатора(калибровочные), уровень опоры 
    // переменные: длина волны калибровки (возможно индекс)IndxP2LW 
    // расчет показаний ( ЧЕРЕЗ  плавающий буффер)
    // P2Mode
    //  else
    //  {
    //     memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // Резко перезапишем буффер
    //
    //  }
    sprintf((char*)Strc,"%6.2f%s",(P2Mode==dBm)?(P2dBmValue):(P2dBValue), (P2Mode==dBm)?("dBm"):("dB")) ;// надо изменить под свои данные
    sprintf((char*)Stra,"%04dнм",CalibrLW[IndxP2LW]) ;
    sprintf((char*)Strb,"%.1f", UserMeasConfig.BaseLvlP2[IndxP2LW]) ;
    
    SSD1305_Fill(0); /* очистка экрана*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// крупные буквы Значение мощности
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы длина волны
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы базовый уровень
    // индикация работающего лазера
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    
  }
  
  // прорисовка ТЕСТЕР АВТОМАТ
  void ShowAUTO ( uint16_t LW_I)
  {
    uint8_t Inv[3][3]; // признаки инверсии показаний при выводе, по столбцам и строкам (1- столбцы, 2- строки)
    if(Mod_At == View) // своя особая индикация
    {
      switch(Mod_View)
      {
      case LOSS:
        // инициализируем показания, 15.09.2021 решили, индикацию только по источникам МАСТЕРА
        for(int x=0;x<3;x++)
        {
          sprintf((char*)StPon1[x], "  ---");  //
          //sprintf((char*)StPon1[x], "%1.2f",DeviceConfig.PlaceLS[x])/1000.0);  // тут1300 Надо проверить !!!!
          sprintf((char*)StPon2[x], "  ---\0");
          sprintf((char*)StPon3[x], "  ---\0");
          for(int y=0;y<3;y++)
            Inv[x][y]=1; //записываем 1 - нет инверсии
        }
        // строка режима
        sprintf((char*)Stra, "Loss: AB/BA");
        sprintf((char*)Strc,"№%03d",SettingPrm.CountMemory) ; // строка номера записи
        // заполняем Показаниями
        iS=0;
        for (int j=0;j<ARRAY_SIZE(CalibrLW);j++) // перебор по длинам волн калибровки
        {
          yesLS=0;
          for(int i=0;i<3;i++)
          {
            if(PonA.LW_AB[i])
            {
              if(PonA.LW_AB[i]==CalibrLW[j])
                yesLS=1;
            }            
          }
          if(yesLS) // есть такой источник, заполним поля
          {
            sprintf((char*)StPon1[iS], "%1.2f",CalibrLW[j]/1000.0);  // !!!!
            // проверим есть ли данные по затуханию AB? заполним строки
            if (PonA.LS_AB[j]>-100.0)
            {
              sprintf((char*)StPon2[iS],"%6.2f",PonA.LS_AB[j]);      
              
              // проверим порог
              if ((PonA.LS_AB[j]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv[0][iS]=0x00; // нужно получить индекс
              // проверим есть ли данные по затуханию BA? заполним строки
              if (PonA.LS_BA[j]>-100.0)
              {
                sprintf((char*)StPon3[iS],"%6.2f",PonA.LS_BA[j]);      
                // проверим порог
                if ((PonA.LS_BA[j]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv[1][iS]=0x00; // нужно получить индекс
              }
            }
            iS++;
            
          }
        }
        
        
        break;
      case ORL:
        // инициализируем показания, 15.09.2021 решили, индикацию только по источникам МАСТЕРА
        for(int x=0;x<3;x++)
        {
          sprintf((char*)StPon1[x], "  ---");  //
          //sprintf((char*)StPon1[x], "%1.2f",DeviceConfig.PlaceLS[x])/1000.0);  // тут1300 Надо проверить !!!!
          sprintf((char*)StPon2[x], "  ---\0");
          sprintf((char*)StPon3[x], "  ---\0");
          for(int y=0;y<3;y++)
            Inv[x][y]=1; //записываем 1 - нет инверсии
        }
        // строка режима
        sprintf((char*)Stra, "ORL: AB/BA");
        sprintf((char*)Strc,"№%03d",SettingPrm.CountMemory) ; // строка номера записи
        // заполняем Показаниями
        iS=0;
        for (int j=0;j<ARRAY_SIZE(CalibrLW);j++) // перебор по длинам волн калибровки
        {
          yesLS=0;
          for(int i=0;i<3;i++)
          {
            if(PonA.LW_AB[i])
            {
              if(PonA.LW_AB[i]==CalibrLW[j])
                yesLS=1;
            }            
          }
          if(yesLS) // есть такой источник, заполним поля
          {
            sprintf((char*)StPon1[iS], "%1.2f",CalibrLW[j]/1000.0);  // !!!!
            // проверим есть ли данные по затуханию AB? заполним строки
            if (PonA.ORL_AB[j]>-100.0)
            {
              sprintf((char*)StPon2[iS],"%6.2f",PonA.ORL_AB[j]);      
              
              // проверим порог
              if ((PonA.ORL_AB[j]<UserMeasConfig.Lim_ORL[0])&&(SettingPrm.EnaCmpORL)) Inv[0][iS]=0x00; // нужно получить индекс
              // проверим есть ли данные по затуханию BA? заполним строки
              if (PonA.ORL_BA[j]>-100.0)
              {
                sprintf((char*)StPon3[iS],"%6.2f",PonA.ORL_BA[j]);      
                // проверим порог
                if ((PonA.ORL_BA[j]<UserMeasConfig.Lim_ORL[0])&&(SettingPrm.EnaCmpORL)) Inv[1][iS]=0x00; // нужно получить индекс
              }
            }
            iS++;
            
          }
        }
        
        
        break;
      case LNM:
        // выводим значение длины, 
        // длина волны источника - место 2, тип источника (Sm/Mm)
        // 
        sprintf((char*)Str,"%05dm",PonA.LnA) ;// надо изменить под свои данные
        sprintf((char*)Stra,"%04dнм",DeviceConfig.PlaceLS[1]) ;
        break;
      case BasV:
        break;
      }
      // пропечатываем заполненые поля
      SSD1305_Fill(0); /* очистка экрана*/
      if(Mod_View != LNM)
      {
        // длины волн
        SSD1305_GotoXY(12,0);
        SSD1305_PutsN((void*)StPon1[0], (void*)TabSG2, 1);// мелкие буквы верхняя строка
        SSD1305_GotoXY(12,8);
        SSD1305_PutsN((void*)StPon1[1], (void*)TabSG2, 1);// мелкие буквы вторая строка
        SSD1305_GotoXY(12,16);
        SSD1305_PutsN((void*)StPon1[2], (void*)TabSG2, 1);// мелкие буквы третья строка
        SSD1305_GotoXY(12,24);
        SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);//  мелкие буквы режим индикации 
        // LOSS AB
        SSD1305_GotoXY(35,0);
        SSD1305_PutsN((void*)StPon2[0], (void*)TabSG2, Inv[0][0]);// мелкие буквы верхняя строка
        SSD1305_GotoXY(35,8);
        SSD1305_PutsN((void*)StPon2[1], (void*)TabSG2, Inv[0][1]);// мелкие буквы вторая строка
        SSD1305_GotoXY(35,16);
        SSD1305_PutsN((void*)StPon2[2], (void*)TabSG2, Inv[0][2]);// мелкие буквы третья строка
        // LOSS BA
        SSD1305_GotoXY(70,0);
        SSD1305_PutsN((void*)StPon3[0], (void*)TabSG2, Inv[1][0]);// мелкие буквы верхняя строка
        SSD1305_GotoXY(70,8);
        SSD1305_PutsN((void*)StPon3[1], (void*)TabSG2, Inv[1][1]);// мелкие буквы вторая строка
        SSD1305_GotoXY(70,16);
        SSD1305_PutsN((void*)StPon3[2], (void*)TabSG2, Inv[1][2]);// мелкие буквы третья строка
        
      }
      else // Long measure
      {
        SSD1305_GotoXY(12,0);
        SSD1305_PutsN((void*)Str, (void*)TabSG3, 1);// крупные буквы значение длины
        SSD1305_GotoXY(12,24);
        SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы длина волны
      }
      // выводим при просмотре всегда
      SSD1305_GotoXY(87,24);
      SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);//  мелкие буквы номер записи
    }
    else
    {
      // выводим в зависимости от режимов (практически все режимы)
      // 
      // переменные: Режим индикации авто измерения
      sprintf((char*)Stra,"%s",MsgAuto[0]) ; // 1 строчка сообщения
      sprintf((char*)Strb,"%s",MsgAuto[1]) ; // 2 строчка сообщения
      //sprintf((char*)Strd,"%s",MsgAuto[2]) ; // 3 строчка сообщения // тест, надо убрать
      sprintf((char*)Strd,"%04dнм",LW_I) ; // строка длина волны, режима автомат
      sprintf((char*)Strc,"№%03d",SettingPrm.CountMemory) ; // строка номера записи
      
      
      SSD1305_Fill(0); /* очистка экрана*/
      
      SSD1305_GotoXY(12,0);
      SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы верхняя строка
      SSD1305_GotoXY(12,8);
      SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы вторая строка
      //SSD1305_GotoXY(12,16);
      //SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// мелкие буквы вторая строка
      SSD1305_GotoXY(12,24);
      SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);//  мелкие буквы длина волны 
      SSD1305_GotoXY(87,24);
      SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);//  мелкие буквы номер записи
      
      // индикация работающего лазера
      sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
      SSD1305_GotoXY(pSR,0);
      SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
      // тестовый вывод (надо убрать)
      //sprintf((char*)Strm,"%d",Summx) ; // индикация значения регистра измеренной длины
      //SSD1305_GotoXY(0,16);
      //SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация регистра ДЛИНЫ
    }
  }  
  
  // прорисовка источника VFL (красный глаз)
  void ShowVFL (void)
  {
    // выводим значек излучения, режим источника
    // длина волны источника, 
    // переменные: 
    
    sprintf((char*)Strc,"%s",StrVFLMode[ReMode]) ;
    sprintf((char*)Stra,"650нм") ;
    sprintf((char*)Strb,"VFL") ;
    
    SSD1305_Fill(0); /* очистка экрана*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// крупные буквы Режим источника
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы длина волны
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы название источника
    // индикация работающего лазера
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    
  }
  // прорисовка источника Ln (измеритель длины)
  void ShowIzmLN (void)
  {
    // выводим значение длины, 
    // длина волны источника - место 2, тип источника (Sm/Mm)
    // 
    MeasLenght=(DataLN[1][IndxShowLN]>(uint16_t)DeviceConfig.MZone)?((uint16_t)((DataLN[1][IndxShowLN]-(uint16_t)DeviceConfig.MZone)*DeviceConfig.KGen)):(0);
    //sprintf((char*)Str,"%05dm",MeasLenght) ;// надо изменить под свои данные
    //sprintf((char*)Str,"%05dm",MemoryData.mLen) ;// надо изменить под свои данные
    //sprintf((char*)Str,"%05dm",DataLN[0][0]) ;// надо изменить под свои данные
    sprintf((char*)Strc,"%05dm",MeasLenght) ;// надо изменить под свои данные
    sprintf((char*)Stra,"%04dнм",DeviceConfig.PlaceLS[1]) ;
    sprintf((char*)Strb,"%s",(DeviceConfig.PlaceLS[1]>1300)?("S"):("M")) ;
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    if(IndxShowLN==0)
      sprintf((char*)Strd,"%s",(DataLN[1][1] !=0)?("!>"):(" ")) ; // индикация признака наличия еще данных
    else
      sprintf((char*)Strd,"%d",IndxShowLN) ; // индикация признака наличия еще данных

    SSD1305_Fill(0); /* очистка экрана*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// крупные буквы значение длины
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы длина волны
    SSD1305_GotoXY(64,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы тип источника SM/MM
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    SSD1305_GotoXY(100,24);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// мелкие буквы номер измерения
    // тестовый вывод списка измерений
//    for(int i=0;i<4;i++)
//    {
//    sprintf((char*)Str,"%06d",DataLN[i][0]) ;// надо изменить под свои данные
//        SSD1305_GotoXY(12,i*8);
//    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы длина волны
//  
//    }
  }
  
  // прорисовка источника Rl (обратное отражение)
  void ShowORL (void)
  {
    // выводим значение ORL, 
    // длина волны источника, тип источника (Sm/Mm), мощность источника
    // переменные: Ind_LW - индекс текущего рабочего места
    
    //  sprintf((char*)Str,"%d",IN_BuffADC[1]) ; // test
    sprintf((char*)Strc,"%.1f dB",ORLCalc) ;
    sprintf((char*)Stra,"%04dнм",DeviceConfig.PlaceLS[Ind_LW]) ;
    //sprintf((char*)Strb,"%.2f",SettingPrm.PowSc[Ind_LW]) ;// индикация мощности источника 
    // можно попробовать подменить уровнем привязки для ORL? 16/02/2023
    sprintf((char*)Strb,"%.2f",BaseRetCurrORL[indx_LW_ORL]) ;// индикация мощности источника 
    sprintf((char*)Strm,"%s",(DeviceConfig.PlaceLS[Ind_LW]>1300)?("S"):("M")) ;
    
    SSD1305_Fill(0); /* очистка экрана*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// крупные буквы Показания ORL
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы длина волны
    SSD1305_GotoXY(90,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы мощность источника
    SSD1305_GotoXY(64,24);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// мелкие буквы тип источника SM/MM
    // индикация работающего лазера
    sprintf((char*)Strd,"%s",(lSofn)?("*"):(" ")) ; // индикация излучения лазера
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
    // индикация коректируемые значения "внизу" (от -40 по дБм)
    if( CoeffPM.Pow_mW_Clbr != PowClbr_mW)
    {
    sprintf((char*)Stri,"R") ; // признак коррекции
    SSD1305_GotoXY(pSR,16); // третья строка
    SSD1305_PutsN((void*)Stri, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
      
    }
    // индикация коректируемые значения "вверху" (от -10... -18 по дБм)
    if( BaseRetCurrORL[indx_LW_ORL] != CoeffPM.BaseRet[indx_LW_ORL])
    {
    sprintf((char*)Stri,"С") ; // признак коррекции
    SSD1305_GotoXY(pSR,8); // вторая строка
    SSD1305_PutsN((void*)Stri, (void*)TabSG2, 1);// мелкие буквы индикация излучения лазера
      
    }
  }
  // индикация выбора режима Работы (FunSetup)
  void ShowChooseMode(void)
  {
    switch(ConSet)
    {
    case P1:
      sprintf((char*)Stra,"%s",StrP1[0]) ;
      sprintf((char*)Strb,"%s",StrP1[1]) ;
      break;
    case P1a:
      sprintf((char*)Stra,"%s",StrP1a[0]) ;
      sprintf((char*)Strb,"%s",StrP1a[1]) ;
      break;
    case RE:
      sprintf((char*)Stra,"%s",StrRE[0]) ;
      sprintf((char*)Strb,"%s",StrRE[1]) ;
      break;
    case Sc:
      sprintf((char*)Stra,"%s",StrSc[0]) ;
      sprintf((char*)Strb,"%s",StrSc[1]) ;
      break;
    case P2:
      sprintf((char*)Stra,"%s",StrP2[0]) ;
      sprintf((char*)Strb,"%s",StrP2[1]) ;
      break;
    case Ln:
      sprintf((char*)Stra,"%s",StrLn[0]) ;
      sprintf((char*)Strb,"%s",StrLn[1]) ;
      break;
    case Rl:
      sprintf((char*)Stra,"%s",StrRl[0]) ;
      sprintf((char*)Strb,"%s",StrRl[1]) ;
      break;
    case At:
      sprintf((char*)Stra,"%s",StrAt[0]) ;
      sprintf((char*)Strb,"%s",StrAt[1]) ;
      break;
      
    }

    SSD1305_Fill(0); /* очистка экрана*/
    // выводим время
    //Sec2Clock(GetSysTime(&hrtc));// получаем текущее время (заполняем структуру) из счетчиков срегистров RTC lkz 103
    // для 303 надо переписать структуры из RTC в регистры Clocks
    ReSetRegRTC(0, &hrtc); // for F303
    
    sprintf((char*)Strd,"20%02d/%02d/%02d %02d:%02d:%02d",Clocks.cYear,Clocks.cMonth,Clocks.cDay,Clocks.cHour,Clocks.cMinutes,Clocks.cSeconds) ;
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// мелкие буквы
    SSD1305_GotoXY(12,10);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// мелкие буквы
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// мелкие буквы
    
  }
  
  /* индикация результата ORL в ТЕСТЕРЕ АВТОМАТ*/
  void ShowORLAuto(void)
  { 
    uint8_t lD[10], limLS[10], lORL[10];
    float LW, LS, ORL;
    SSD1305_Fill(0); /* очистка экрана*/
    // готовим первую строку ( 3 строки значение), затем выходную строку из 3-х предварительных
    LW = 1.31; 
    LS = -40.55;
    ORL = 46.53;
    sprintf((char*)lD,"%.2f", LW) ;
    sprintf((char*)limLS,"%.2f", LS) ;
    sprintf((char*)lORL,"%.2f", ORL) ;
    
    // draw icon mode device - & level battery
    //DrawMod (CondWork);
    //ShowBatLvl(BatLevel);
    
    sprintf((char*)Str,"%s %s %s", lD, limLS, lORL) ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы
    
    LW = 1.49; 
    LS = -99.99;
    ORL = 12.34;
    sprintf((char*)lD,"%.2f", LW) ;
    sprintf((char*)limLS,"%.2f", LS) ;
    sprintf((char*)lORL,"%.2f", ORL) ;
    sprintf((char*)Str,"%s %s %s", lD, limLS, lORL) ;
    SSD1305_GotoXY(12,8);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы
    
    LW = 1.55; 
    LS = -56.78;
    ORL = 90.12;
    sprintf((char*)lD,"%.2f", LW) ;
    sprintf((char*)limLS,"%.2f", LS) ;
    sprintf((char*)lORL,"%.2f", ORL) ;
    sprintf((char*)Str,"%s %s %s", lD, limLS, lORL) ;
    SSD1305_GotoXY(12,16);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы
    sprintf((char*)Str,"Lim: LS/ORL") ;
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы
    sprintf((char*)Str,"№%d",439) ;
    SSD1305_GotoXY(80,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// мелкие буквы
    
    //  SSD1305_DrawRectangle(12,0,104,32,1);
    
  }
  
  // draw icon mode device - & level battery
  
  void DrawMod (uint8_t Mode)
  {
    uint8_t ModeIndDraw=1;//1
    // здесь попререключаем инверсией в зависимости от времени до выключения
    if(UserMeasConfig.AutoOff)
    {
     if(CntPwrOffCount<LASTTIMEOFF)
     {
       ModeIndDraw = CntPwrOffCount&0x1;
       //myBeep(20);
     }
     else
       ModeIndDraw = 0;
    }
    
    sprintf((char*)StrDrM,"%c",48 + Mode) ;
    SSD1305_GotoXY(102,8);
    SSD1305_PutsN((void*)StrDrM, (void*)TabSG5, ModeIndDraw);
    // если авто выкл нарисуем кружок
    //if(UserMeasConfig.AutoOff)
    //{
    // SSD1305_DrawCircle(108, 15, 7 ,1);
    //}
    // линии для прорисовки, три белые одна черная нижняя
    SSD1305_DrawLine(102, 8, 102 ,22, 1);// вертикальная правая
    SSD1305_DrawLine(114, 8, 114 ,22, 1);// вертикальная левая
    SSD1305_DrawLine(102, 22, 114 ,22, 1);// горизонт нижняя
    SSD1305_DrawLine(102, 23, 114 ,23, 0);// горизонт черная нижняя
  }
  
  void ShowBatLvl (uint8_t ProcBat)
  {
    sprintf((char*)StrBat,"%c", 'a' + (uint8_t)(ProcBat/8.5)) ;
    SSD1305_GotoXY(102,0);
    SSD1305_PutsN((void*)StrBat, (void*)TabSG5, 1);
    // тест полный экран
    //  SSD1305_DrawRectangle(0,0,128,32,1);
   
  }
 
// Общее из MAIN про режимы, что бы не править везде
      // прорисовки режимов если необходимо
  void DrawMainModes (void)
  {
    switch(CondWork)
    {
    
    case P1: // перерисовка каждые ~0.5сек
      if(NeedReDraw) ShowPM(ModeI, PMWavelenght);
    break;
    case P1a:  // перерисовка нужна когда изменнение
      if(NeedReDraw) ShowP1a(P1aWavelenght); /* индикация измерителя P1a упрощенная*/
      CntLCD_P1 = 0;// перерисовывать снова не надо!
    break;
    case SetFn:
      if(NeedReDraw) ShowChooseMode(); /* индикациz Выбора режима, так как там часы, то рисуем все время (0.48 с)*/
      
    break;
    case Sc:
      if(NeedReDraw) ShowSource(); /* индикациz источника Sc упрощенная*/
      CntLCD_P1 = 0;// перерисовывать снова не надо!
    break;
    case RE:
      if(NeedReDraw) ShowVFL(); /* индикациz источника VFL упрощенная*/
      CntLCD_P1 = 0;// перерисовывать снова не надо!
    break;
    case Ln:  //
      if(NeedReDraw) ShowIzmLN(); /* индикация Ln упрощенная*/
      CntLCD_P1 = 0;// перерисовывать снова не надо!
    break;
    case Rl:  // перерисовка каждые ~0.5сек
      if(NeedReDraw) ShowORL(); /* индикация ORL упрощенная*/
    break;
    case P2:  // перерисовка каждые ~0.5сек
      if(NeedReDraw) ShowP2(MediumP2); /* индикация P2 упрощенная*/
    break;
    case At:  //режим Автомат
      Run_At ();

      if(NeedReDraw) ShowAUTO(PMWavelenght); /* индикация ТЕСТЕР АВТОМАТ упрощенная*/
      CntLCD_P1 = 0;// перерисовывать снова не надо!
    break;
    case SetPrm:  // 
      if(NeedReDraw)
      {
        switch(SetupMod) //sTime, sNumF, sLOSS, sORL, sSCp, sMem, sSWm
        {
        case sTime:
        ShowCurrTime(); /* индикация времени упрощенная*/
        break;
        case sAutoOff:
        ShowSetAutoOff(); /* Экран установки автовыключения*/
        break;
        case sNumF:
        ShowNumFbr(); /* индикация установки номера волокна*/
        break;
        case sLOSS:
        ShowLimLOSS(); /* Экран установки порогов LOSS*/
        break;
        case sLOSSm:
        ShowLimLOSSm(); /* Экран установки порогов LOSS MM 850 1300*/
        break;
        case sMem:
        ShowMemory(); /* Экран работы с памятью*/
        break;
        case sORL:
        ShowLimORL(); /* Экран установки порогов ORL*/
        break;
        case sSWm:
        ShowEnaSave(); /* Экран установки переключачелей разрешения сохранения LOSS, ORL, Ln*/
        break;
        case sVer:
        ShowVer(); /* Экран установки отображения идентификатора (*/
        break;
        case sSCp:
        ShowPowSC(); /* Экран установки уровней Лазеров*/
        break;

        }
      }
      CntLCD_P1 = 0;// перерисовывать снова не надо!
    break;
            //

    default:
      break;
    } //switch CondWork
    // вывод сообшщения о записи в одно и то же место!!!
    ShowMsgWrMem();
    
  }
// контроль заряда батарейки , вынесено из под MAIN

  void CheckUbat(void)
  {
          // здесь уже имеем значения АЦП, можно с ними что то сделать
      // Батарейка 
      
      Ubat = IN_BuffADC[0]*UADC*2;
      if(Ubat>MAXBAT)
      {
        BatLevel =105;
      }
      else
      {
        if(Ubat<MINBAT)
        {
          BatLevel = 0;
        }
        else
        {
          BatLevel = (uint8_t)(((Ubat-MINBAT)/(MAXBAT-MINBAT))*100);
        }
      }
      if(ABS(Ubat-OldUbat)>0.05) // тогда перерисовываем индикатор батарейки
      {
        OldUbat = Ubat;
        ShowBatLvl(BatLevel);
        NeedReDraw = 1;
        
      }

  }
#ifdef __cplusplus
}
#endif
