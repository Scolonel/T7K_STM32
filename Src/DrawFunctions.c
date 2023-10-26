// ����� ������� ������ ������� ���������� ����� ����������

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

// ��������� ������ ��������� ��� ���������
#define	u_MW    0
#define	u_MKW   1
#define	u_NW    2
#define	u_PW    3
#define	pSR    93 // ����� ������ ��������� ���������� ���������
   

   const char* StrP1[]= {"   ���������� ","    ��������   "};   
   const char* StrP1a[]={"   ���������� ","   ����. ����  "};  
   const char* StrRE[]= {"   ���������� ","    �������   "};
   const char* StrSc[]= {"   ��������   ","   ��������\xff   "};
   const char* StrP2[]= {"   ���������  ","    ��������   "};
   const char* StrLn[]= {"   ���������� ","    ��. �����   "};
   const char* StrRl[]= {"   ���������� "," ���.�����.(ORL)"};
   const char* StrLs[]= {"   ���������� ","   ��������\xff   "};
   const char* StrAt[]= {"    ������    ","    �������   "};
   const char* StrScMode[]={"@ OFF","@ CW","@ 270Hz","@ 2kHz","@ CW*"};
   const char* StrVFLMode[]={"@ OFF","@ CW","@ 1Hz"};
   const char* StrNFa[]={"����� �������"};
   const char* StrNFb[]={"���� +"};
   const char* StrdB[]={"dBm","dB","dBe"};
   
     
uint8_t Str[64];
uint8_t Stra[32];
uint8_t Strb[32];
uint8_t Strc[32]; // 
uint8_t Strd[32];
uint8_t Strm[32]; // 
uint8_t Stri[8];
uint8_t Str2[32]; // ����������� ��������� � ������  
uint8_t StrDrM[4]; // ������ ����������� ������ ������ �� ����������
uint8_t StrBat[4]; // ������ ����������� ������ ������ �� ����������

uint8_t StPon1[3][5], StPon2[3][8], StPon3[3][8]; //������ ��� ������������� ������� �� ������������

uint8_t IndxEditNumFbr; // ������ ��������� �� �������� ��������
uint8_t IndxEditLOSS; // ������ ��������� �� �������� �������� LOSS
uint8_t IndxEditLOSSm; // ������ ��������� �� �������� �������� LOSS MM 850,1300
uint8_t IndxEditORL; // ������ ��������� �� �������� �������� ORL
uint8_t IndxEnaSave; // ������ ��������� �� �������� �������� ���������� ����������
uint8_t IndxPowSC; // ������ ��������� �� �������� �������� �������� ���������
uint8_t IndxClrMEM; // ������ ��������� ���������� ������� ������
uint8_t IndxEditORL; // ������ ��������� �� �������� �������� ORL
uint8_t IndxInvView; // ������ ��������� �� �������� ��� ������ ����������� � �������� (1-������ �����, 2-������, 4-������)
uint8_t yesLS; // ���� ����� �������� ����� ��������
uint8_t CountDrawMsg; // ������� ��������� ������� ( P1 - ������ � ������)
int iS; // ������ ��������� ��� ��������� �����������
uint16_t IndxShowLN=0; // ������ ��������� ���������� ���������� �� ���������

int CntPwrOffCount=20; // ������� ������� ��������������

/* ����� ��������� ������� ORL*/
void ShowLimORL(void)
{
  SSD1305_Fill(0); /* ������� ������*/
  sprintf((char*)Str,"on/off   [%c]", (SettingPrm.EnaCmpORL)?('+'):(' ')) ;
  // ��������
  sprintf((char*)Stra,"%.2f",UserMeasConfig.Lim_ORL[0]) ;
  //sprintf((char*)Strb,"%.2f",UserMeasConfig.Lim_ORL[1]) ;
  //sprintf((char*)Strm,"%.2f",UserMeasConfig.Lim_ORL[2]) ;
  sprintf((char*)Strb,"ORL") ;
  sprintf((char*)Strm,"Lim") ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� ���������� ���������
  SSD1305_GotoXY(60,12);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,1);// ������ ����� ORL ��� ��������������
  SSD1305_GotoXY(13,12);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,1);// ������ ����� ������� ORL
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// ������ �����  lim
  //SSD1305_GotoXY(70,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditORL==2)?(0):(1));// ������ ����� ORL ��� ������ ����� �����
  //SSD1305_GotoXY(70,16);
  //SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxEditORL==3)?(0):(1));// ������ ����� ORL ��� ������� ����� �����
 // ��������� ����� ���� ����������
  // ��������
  //sprintf((char*)Strb,"%.2f",DeviceConfig.PlaceLS[1]/1000.) ;
  //sprintf((char*)Strm,"%.2f",DeviceConfig.PlaceLS[2]/1000.) ;

  //SSD1305_GotoXY(13,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� �����
}
/* ����� ��������� ��������������*/
void ShowSetAutoOff(void)
{
  SSD1305_Fill(0); /* ������� ������*/
  sprintf((char*)Str,"���� ����.[%c]", (UserMeasConfig.AutoOff)?('+'):(' ')) ;
  // ��������
  sprintf((char*)Strm,"����� ") ;
  sprintf((char*)Stra,"�����") ;
  sprintf((char*)Strb,"%d",2<<UserMeasConfig.AutoOffMod) ; // ����� ���� ���� � ������� 2,4,8,16
  //sprintf((char*)Strm,"%.2f",UserMeasConfig.Lim_LS[4]) ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� ���������� ���������
  SSD1305_GotoXY(13,12);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ������� LOSS
  SSD1305_GotoXY(60,12);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,1);// ������ ����� LOSS ��������
  //SSD1305_GotoXY(70,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditLOSS==2)?(0):(1));// ������ ����� LOSS ��� ������ ����� ����� 1490
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,1);// ������ ����� LOSS ��� ������� ����� ����� 1550
 // ��������� ����� ���� ����������
  // ��������
  //sprintf((char*)Stra,"%.2f",CalibrLW[2]/1000.) ;
  //sprintf((char*)Strb,"%.2f",CalibrLW[3]/1000.) ;
  //sprintf((char*)Strm,"%.2f",CalibrLW[4]/1000.) ;

  //SSD1305_GotoXY(13,0);
  //SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� ����� 1310
  //SSD1305_GotoXY(13,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� ����� 1490
  //SSD1305_GotoXY(13,16);
  //SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// ������ �����  ��� ������� ����� ����� 1550
  
  
}

/* ����� ��������� ������� LOSS � 15.09.2021 ��� ���� ���� ���� ���� �����*/
void ShowLimLOSS(void)
{
  SSD1305_Fill(0); /* ������� ������*/
  sprintf((char*)Str,"On/Off   [%c]", (SettingPrm.EnaCmpLOSS)?('+'):(' ')) ;
  // ��������
  sprintf((char*)Strm,"Lim") ;
  sprintf((char*)Stra,"LOSS") ;
  sprintf((char*)Strb,"%.2f",UserMeasConfig.Lim_LS[0]) ; // ����� ����� ��� ���� ���� ����
  //sprintf((char*)Strm,"%.2f",UserMeasConfig.Lim_LS[4]) ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� ���������� ���������
  SSD1305_GotoXY(13,12);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ������� LOSS
  SSD1305_GotoXY(60,12);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,1);// ������ ����� LOSS ��������
  //SSD1305_GotoXY(70,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditLOSS==2)?(0):(1));// ������ ����� LOSS ��� ������ ����� ����� 1490
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxEditLOSS==3)?(0):(1));// ������ ����� LOSS ��� ������� ����� ����� 1550
 // ��������� ����� ���� ����������
  // ��������
  //sprintf((char*)Stra,"%.2f",CalibrLW[2]/1000.) ;
  //sprintf((char*)Strb,"%.2f",CalibrLW[3]/1000.) ;
  //sprintf((char*)Strm,"%.2f",CalibrLW[4]/1000.) ;

  //SSD1305_GotoXY(13,0);
  //SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� ����� 1310
  //SSD1305_GotoXY(13,8);
  //SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� ����� 1490
  //SSD1305_GotoXY(13,16);
  //SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// ������ �����  ��� ������� ����� ����� 1550
  
  
}
/* ����� ��������� ������� LOSS � 15.09.2021 ��� ���� ���� ���� ���� �����*/
void ShowLimLOSSm(void)
{
  SSD1305_Fill(0); /* ������� ������*/
    sprintf((char*)Str,"LOSS  MM") ;

  // ��������
  sprintf((char*)Stra,"%.2f",UserMeasConfig.Lim_LS[0]) ;
  sprintf((char*)Strb,"%.2f",UserMeasConfig.Lim_LS[1]) ;
  
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� ��� ��������� LOSS MM
  SSD1305_GotoXY(70,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(IndxEditLOSSm==0)?(0):(1));// ������ ����� LOSSm ��� ������ ����� ����� 850
  SSD1305_GotoXY(70,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEditLOSSm==1)?(0):(1));// ������ ����� LOSSm ��� ������ ����� ����� 1300
 // ��������� ����� ���� ����������
  // ��������
  sprintf((char*)Stra,"%.2f",CalibrLW[0]/1000.) ;
  sprintf((char*)Strb,"%.2f",CalibrLW[1]/1000.) ;

  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� ����� 850
  SSD1305_GotoXY(13,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� ����� 1300
}  
  

/* ����� ��������� �������������� ���������� ���������� LOSS, ORL, Ln*/
void ShowEnaSave(void)
{
  SSD1305_Fill(0); /* ������� ������*/
  sprintf((char*)Str,"����. � ���.�") ;
  sprintf((char*)Stra,"Loss   ") ;
  sprintf((char*)Strb,"ORL    ") ;
  sprintf((char*)Strm,"Ln     ") ;
  // ��������
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� �����
  SSD1305_GotoXY(13,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� �����
  SSD1305_GotoXY(13,16);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// ������ �����  ��� ������� ����� �����
  SSD1305_GotoXY(13,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2,(1));// ������ �����  ��� ������� ����� �����

  
  sprintf((char*)Stra,"[%c]", (SettingPrm.EnaWrtMem&0x1)?('+'):(' ')) ;
  sprintf((char*)Strb,"[%c]", (SettingPrm.EnaWrtMem&0x2)?('+'):(' ')) ;
  sprintf((char*)Strm,"[%c]", (SettingPrm.EnaWrtMem&0x4)?('+'):(' ')) ;

  SSD1305_GotoXY(70,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(IndxEnaSave==1)?(0):(1));// ������ ����� LOSS ��� ������ ����� �����
  SSD1305_GotoXY(70,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxEnaSave==2)?(0):(1));// ������ ����� LOSS ��� ������ ����� �����
  SSD1305_GotoXY(70,16);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxEnaSave==3)?(0):(1));// ������ ����� LOSS ��� ������� ����� �����
}
/* ����� ������ � �������*/
void ShowMemory(void)
{
  SSD1305_Fill(0); /* ������� ������*/
  sprintf((char*)Str,"������ ����") ;
  SSD1305_GotoXY(12,0);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
  sprintf((char*)Str,"! ������ !") ;
  SSD1305_GotoXY(12,8);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
  sprintf((char*)Str,"%d �� 512",SettingPrm.CountMemory) ;
  SSD1305_GotoXY(12,16);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
  sprintf((char*)Str,"�������� ?") ;
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, (IndxClrMEM)?(0):(1));
}
/* ����� ��������� ����������� �������������� */
void ShowVer(void)
{
  SSD1305_Fill(0); /* ������� ������*/
  // ��������
  //            sprintf ((void*)AnsData, "T7K,%s,(%d/%c:%d/%c:%d/%c)%s%s%s,%04d,SW_rev 1.%04d\r", // ������� VFL �� �������������� -VFL (� ����� �������)
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
  
  sprintf((char*)Stra,"%s",DeviceConfig.NameDevice) ; // 1 ������
  sprintf((char*)Strb,"%d/%c:%d/%c:%d/%c",
          (DeviceConfig.PlaceLS[0]%1000)/10,
          (DeviceConfig.CfgOUT[0])?('1'):('0'), // 
          (DeviceConfig.PlaceLS[1]%1000)/10,
          (DeviceConfig.CfgOUT[1])?('1'):('0'), // 
          (DeviceConfig.PlaceLS[2]%1000)/10,
          (DeviceConfig.CfgOUT[2])?('1'):('0')); // 2 ������
  sprintf((char*)Strm,"%s%s%s",
          (DeviceConfig.CfgPM)?((DeviceConfig.CfgPM-1)?("-PMH"):("-PM")):(""),
          (DeviceConfig.CfgL)?("-L"):(""), // 
          (DeviceConfig.CfgRE & 0x1)?("-VFL"):("")) ; // 3 ������
  sprintf((char*)Str,"�%04d ver 1.%04d",
          DeviceConfig.NumDevice,
          NumVer) ; // 4 ������
  
  
  SSD1305_GotoXY(13,0);
  SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� 1 ������
  SSD1305_GotoXY(13,8);
  SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� 2 ������
  SSD1305_GotoXY(13,16);
  SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// ������ �����  3 ������
  SSD1305_GotoXY(12,24);
  SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� 4 ������
}
  /* ����� ��������� ������� �������*/
  void ShowPowSC(void)
  {
    SSD1305_Fill(0); /* ������� ������*/
    sprintf((char*)Str,"����. ���������") ;
    // ��������
    sprintf((char*)Stra,"%.2f",SettingPrm.PowSc[0]) ;
    sprintf((char*)Strb,"%.2f",SettingPrm.PowSc[1]) ;
    sprintf((char*)Strm,"%.2f",SettingPrm.PowSc[2]) ;
    
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� ���������� ���������
    SSD1305_GotoXY(70,0);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2,(IndxPowSC==1)?(0):(1));// ������ ����� Power ��� ������ ����� �����
    SSD1305_GotoXY(70,8);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2,(IndxPowSC==2)?(0):(1));// ������ ����� Power ��� ������ ����� �����
    SSD1305_GotoXY(70,16);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2,(IndxPowSC==3)?(0):(1));// ������ ����� Power ��� ������� ����� �����
    // ��������� ����� ���� ����������
    // ��������
    sprintf((char*)Stra,"%.2f",DeviceConfig.PlaceLS[0]/1000.) ;
    sprintf((char*)Strb,"%.2f",DeviceConfig.PlaceLS[1]/1000.) ;
    sprintf((char*)Strm,"%.2f",DeviceConfig.PlaceLS[2]/1000.) ;
    
    SSD1305_GotoXY(13,0);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� �����
    SSD1305_GotoXY(13,8);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2,(1));// ������ ����� ��� ������ ����� �����
    SSD1305_GotoXY(13,16);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2,(1));// ������ �����  ��� ������� ����� �����
  }
  
  /* ����� ��������� ������ �������, */
  void ShowNumFbr(void)
  {
    SSD1305_Fill(0); /* ������� ������*/
    sprintf((char*)Str,"����� �������") ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    // �������� �� ��������� ����� �������
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
    sprintf((char*)Str,"����+ [%c]",(SettingPrm.EnaAutoInkrNF)?('+'):(' ') ) ;
    SSD1305_GotoXY(12,20);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    
  }
  
  /* ����� ��������� �������, */
  void ShowCurrTime(void)
  {
    SSD1305_Fill(0); /* ������� ������*/
    sprintf((char*)Str,"��������� �����") ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    //sprintf((char*)Str,"    �������  ") ;
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
  
  /* ����� ��������, ������ TabSG1 �������� - �������
  ����� �� �������� ������ �������*/
  void ShowWelcome(void)
  {
    SSD1305_Fill(0); /* ������� ������*/
    sprintf((char*)Str,"�%04d 20.5%s%s",DeviceConfig.NumDevice,(DeviceConfig.CfgL)?("L"):(""),(DeviceConfig.CfgRE & 0x1)?("R"):("")) ;
    SSD1305_GotoXY(14,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)Str,"%02d�",DeviceConfig.YearManuf) ;
    SSD1305_GotoXY(94,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)Str,"��� �����������") ;
    //sprintf((char*)Str,"%d",RTC_SysTime(&hrtc));    // ������ ������, �������� ������ �������� sprintf, ����������� ��������� ���������� ������, ����� ������ � ������ ������
    //sprintf((char*)Str,"������� Base") ;
    SSD1305_GotoXY(13,16);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);
    sprintf((char*)Str,"%s",DeviceConfig.NameDevice) ;
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str, (void*)TabSG1, 1);
    //  SSD1305_DrawRectangle(12,0,104,32,1);     // ���� �������� ��������� �����������
    
  }
  
  // ���������� ������ ��������� �������� ������ � ���������� �1
  // ����� � ������������� ������� ������ ��� ���������
  float ShowBaseLevel (uint16_t LW)
  {
    return 0.0;
  }
  
  /* ��������� ���������� P1 ����������*/
  void ShowPM(uint8_t ModInd, uint16_t LW_I)
  {
    // ������� �������� ��������� � ����������� �� ������ ���������  ModeI , ��������� �������� �� �������� dBm, dB, W, � ����� �����
    float Tmp1;
    //static double OptPow;
    int8_t OW; // 
    //  OptPow = GetPMValue()*CurrSpectrKoeff;//*GetCoeffSpctr(LW_I);//����������� �������� �������� � ���������, ��� ������ ����� �����
    //  Opt_dBm = GetPMValuedBm(OptPow);
    //  Opt_dB = GetPMValuedB(OptPow);
    
    // ��������� ������� ���������
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
        //����������
        break;
      case -2:
        OptPow=OptPow*1000;
        sprintf((void*)Str2, " %5.2fuW ", OptPow);
        //����������
        break;
      case -3:
        OptPow=OptPow*1000;
        sprintf((void*)Str2, " %5.3fuW ", OptPow);
        //����������
        break;
      case -4:
        OptPow=OptPow*1000000;
        sprintf((void*)Str2, " %5.1fnW ", OptPow);
        //���������
        break;
      case -5:
        OptPow=OptPow*1000000;
        sprintf((void*)Str2, " %5.2fnW ", OptPow);
        //���������
        break;
      default:
        OptPow=OptPow*1000000;
        sprintf((void*)Str2, " %5.3fnW ", OptPow);
        //���������
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
    SSD1305_Fill(0); /* ������� ������*/
    
    //������� ���������
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Str2, (void*)TabSG3, 1);
    // �����  ������ ����� (REF)
    sprintf((char*)Strb,"%.1f", CurrBaseLevel) ;
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);
    // ����� ������� ����� �����
    sprintf((char*)Strc,"%d��", LW_I) ;
    //sprintf((char*)Strc,"%d", IN_BuffADC[0]) ;
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);
    // ����� ��������� � ������ ���� ����
    //  if(CountDrawMsg)
    //  {
    //    if(SettingPrm.CountMemory<511) // ���� ������
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
    // ��������� ����������� ������
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    
    // �������� �����, �������� � ���� ���
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
  
  /* ��������� ���������� P1a ����������*/
  void ShowP1a( uint16_t LW_I)
  { 
    int i;
    uint8_t Inv=0;; // ������� ������������� �������������� ���������
    uint8_t M_PON=0; // ������� ��������� �� ����� 3-�
    float LvlInd;
    
    // �������� ������ ���������
    for(i=0; i<3; i++) // 
    {
      sprintf((void*)StPon1[i], "--- ");
      sprintf((void*)StPon2[i], "--- ");
      sprintf((void*)StPon3[i], " ");
    }
    // sprintf((void*)Stra, "      "); // ������ ��������� ������ � ���
    for(i=0; i < ARRAY_SIZE(RPON); ++i) // ��������� ������ RPON �� ������� ��������
    {
      // ���� ���� ����� ����� � ��������� (YesLW != 0) �� ���� ��������� ������ RPON
      if(( YesLW>100) && (LW_I == CalibrLW[i]))
      {
        RPON[i] = Opt_dBm;
        sprintf((void*)Stra, "%6.2fdBm",RPON[i]); // ������ ��������� ������ � ���
        
      }
      if (RPON[i]>-100.0) // ���� �������� ��� ���������
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
        case 0: // ������ ������� - �����������
          sprintf((void*)StPon1[0], "%1.2f",CalibrLW[i]/1000.0);
          sprintf((void*)StPon2[0], "%6.2f",LvlInd);
          sprintf((void*)StPon3[0], "%s", StrdB[ModeIP1a]);
          //if ((i>1)&&(i<5)) // �������� �� �������� �������� ��������� 1310,1490,1550 ������ PON!!!
          //if (i<5) // �������� �� �������� �������� ��������� 1310,1490,1550 ������ PON!!!
          //{
            if ((RPON[i]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv |=0x01;
          //}
          
          //sprintf(Str4, "%1.2f %6.2fdB",M_L_W_Klb[i]/1000.0,RPON[i]);// ��������� ������� ������� �����
          M_PON++;
          break;
        case 1:
          sprintf((void*)StPon1[1], "%1.2f",CalibrLW[i]/1000.0);
          sprintf((void*)StPon2[1], "%6.2f",LvlInd);
          sprintf((void*)StPon3[1], "%s",  StrdB[ModeIP1a]);
          //if ((i>1)&&(i<5)) // �������� �� �������� �������� ��������� 1310,1490,1550
          //if (i<5) // �������� �� �������� �������� ��������� 1310,1490,1550
          //{
            if ((RPON[i]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv |=0x02;
          //}
          
          //sprintf(Str5, "%1.2f %6.2fdB",M_L_W_Klb[i]/1000.0,RPON[i]);// ��������� ������� ������� �����
          M_PON++;
          break;
        case 2:
          sprintf((void*)StPon1[2], "%1.2f",CalibrLW[i]/1000.0);
          sprintf((void*)StPon2[2], "%6.2f",LvlInd);
          sprintf((void*)StPon3[2], "%s",  StrdB[ModeIP1a]);
          //if ((i>1)&&(i<5)) // �������� �� �������� �������� ��������� 1310,1490,1550
          //if (i<5) // �������� �� �������� �������� ��������� 1310,1490,1550
          //{
            if ((RPON[i]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv |=0x04;
          //}
          
          //sprintf(Str8, "%1.2f %6.2fdB",M_L_W_Klb[i]/1000.0,RPON[i]);// ��������� ������� ������� �����
          M_PON++;
          break;
        default:
          break;
        }
        
      }
    } 
    sprintf((char*)Strc,"�%03d",SettingPrm.CountMemory) ; // ������ ������ ������

    
    SSD1305_Fill(0); /* ������� ������*/
    
    // ��� ������ � ������� ���� � ���������� 
    for(i=0; i<3; i++) // 
    {
      SSD1305_GotoXY(12,i*8);
      SSD1305_PutsN((void*)StPon1[i], (void*)TabSG2, 1); // ������� ����� �����
      SSD1305_GotoXY(40,i*8);
      SSD1305_PutsN((void*)StPon2[i], (void*)TabSG2, ((1<<i)&Inv)?(0):(1)); // ������� ���������
      SSD1305_GotoXY(72,i*8);
      SSD1305_PutsN((void*)StPon3[i], (void*)TabSG2, 1); // ������� ��
    }
    // ������ � ������� ������ ����� � ��������� ����� ������
    // ����� ������� ����� �����
    if(LW_I)
      sprintf((char*)Strd,"%d��", LW_I) ;
    else 
      sprintf((char*)Strd," ----- ") ;
    
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);
    // ��������� ����������� ������
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    // ��������� ������ � ���
    //SSD1305_GotoXY(48,24);
    //SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    // ��������� � ������ � ������
    //  if(CountDrawMsg)
    //  {
    //    if(SettingPrm.CountMemory<511) // ���� ������
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
      // ������� ��� ��������� ������
      SSD1305_GotoXY(87,24);
      SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);//  ������ ����� ����� ������
    
  }
  // ����� ���������� � ������ � ���� � �� �� �����!!!
  void ShowMsgWrMem(void)
  {
    if(CountDrawMsg)
    {
      if(CountDrawMsg>1)
      {
      if(SettingPrm.CountMemory<511) // ���� ������
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
  
  // ���������� ��������� S
  void ShowSource (void)
  {
    // ������� ������ ���������, ����� ���������
    // ����� ����� ���������, ��� ��������� (Sm/Mm), �������� ���������
    // ����������: Ind_LW - ������ �������� �������� �����
    
    sprintf((char*)Strc,"%s",StrScMode[ScMode]) ;
    sprintf((char*)Stra,"%04d��",DeviceConfig.PlaceLS[Ind_LW]) ;
    sprintf((char*)Strb,"%.2f",SettingPrm.PowSc[Ind_LW]) ;
    sprintf((char*)Strm,"%s",(DeviceConfig.PlaceLS[Ind_LW]>1300)?("S"):("M")) ;
    
    SSD1305_Fill(0); /* ������� ������*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// ������� ����� ����� ���������
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ����� �����
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ ����� �������� ���������
    SSD1305_GotoXY(64,24);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��� ��������� SM/MM
    // ��������� ����������� ������
    sprintf((char*)Strd,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    
  }
  // ���������� ���������� �������� P2 ()
  void ShowP2 (float MedADC)
  {
    // ������� �������� �������� (dB/dBm),
    // ����� ����� ����������(�������������), ������� ����� 
    // ����������: ����� ����� ���������� (�������� ������)IndxP2LW 
    // ������ ��������� ( �����  ��������� ������)
    // P2Mode
    //  else
    //  {
    //     memset(BuffP2,IN_BuffADC[1], ARRAY_SIZE(BuffP2)); // ����� ����������� ������
    //
    //  }
    sprintf((char*)Strc,"%6.2f%s",(P2Mode==dBm)?(P2dBmValue):(P2dBValue), (P2Mode==dBm)?("dBm"):("dB")) ;// ���� �������� ��� ���� ������
    sprintf((char*)Stra,"%04d��",CalibrLW[IndxP2LW]) ;
    sprintf((char*)Strb,"%.1f", UserMeasConfig.BaseLvlP2[IndxP2LW]) ;
    
    SSD1305_Fill(0); /* ������� ������*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// ������� ����� �������� ��������
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ����� �����
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ ����� ������� �������
    // ��������� ����������� ������
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    
  }
  
  // ���������� ������ �������
  void ShowAUTO ( uint16_t LW_I)
  {
    uint8_t Inv[3][3]; // �������� �������� ��������� ��� ������, �� �������� � ������� (1- �������, 2- ������)
    if(Mod_At == View) // ���� ������ ���������
    {
      switch(Mod_View)
      {
      case LOSS:
        // �������������� ���������, 15.09.2021 ������, ��������� ������ �� ���������� �������
        for(int x=0;x<3;x++)
        {
          sprintf((char*)StPon1[x], "  ---");  //
          //sprintf((char*)StPon1[x], "%1.2f",DeviceConfig.PlaceLS[x])/1000.0);  // ���1300 ���� ��������� !!!!
          sprintf((char*)StPon2[x], "  ---\0");
          sprintf((char*)StPon3[x], "  ---\0");
          for(int y=0;y<3;y++)
            Inv[x][y]=1; //���������� 1 - ��� ��������
        }
        // ������ ������
        sprintf((char*)Stra, "Loss: AB/BA");
        sprintf((char*)Strc,"�%03d",SettingPrm.CountMemory) ; // ������ ������ ������
        // ��������� �����������
        iS=0;
        for (int j=0;j<ARRAY_SIZE(CalibrLW);j++) // ������� �� ������ ���� ����������
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
          if(yesLS) // ���� ����� ��������, �������� ����
          {
            sprintf((char*)StPon1[iS], "%1.2f",CalibrLW[j]/1000.0);  // !!!!
            // �������� ���� �� ������ �� ��������� AB? �������� ������
            if (PonA.LS_AB[j]>-100.0)
            {
              sprintf((char*)StPon2[iS],"%6.2f",PonA.LS_AB[j]);      
              
              // �������� �����
              if ((PonA.LS_AB[j]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv[0][iS]=0x00; // ����� �������� ������
              // �������� ���� �� ������ �� ��������� BA? �������� ������
              if (PonA.LS_BA[j]>-100.0)
              {
                sprintf((char*)StPon3[iS],"%6.2f",PonA.LS_BA[j]);      
                // �������� �����
                if ((PonA.LS_BA[j]<UserMeasConfig.Lim_LS[0])&&(SettingPrm.EnaCmpLOSS)) Inv[1][iS]=0x00; // ����� �������� ������
              }
            }
            iS++;
            
          }
        }
        
        
        break;
      case ORL:
        // �������������� ���������, 15.09.2021 ������, ��������� ������ �� ���������� �������
        for(int x=0;x<3;x++)
        {
          sprintf((char*)StPon1[x], "  ---");  //
          //sprintf((char*)StPon1[x], "%1.2f",DeviceConfig.PlaceLS[x])/1000.0);  // ���1300 ���� ��������� !!!!
          sprintf((char*)StPon2[x], "  ---\0");
          sprintf((char*)StPon3[x], "  ---\0");
          for(int y=0;y<3;y++)
            Inv[x][y]=1; //���������� 1 - ��� ��������
        }
        // ������ ������
        sprintf((char*)Stra, "ORL: AB/BA");
        sprintf((char*)Strc,"�%03d",SettingPrm.CountMemory) ; // ������ ������ ������
        // ��������� �����������
        iS=0;
        for (int j=0;j<ARRAY_SIZE(CalibrLW);j++) // ������� �� ������ ���� ����������
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
          if(yesLS) // ���� ����� ��������, �������� ����
          {
            sprintf((char*)StPon1[iS], "%1.2f",CalibrLW[j]/1000.0);  // !!!!
            // �������� ���� �� ������ �� ��������� AB? �������� ������
            if (PonA.ORL_AB[j]>-100.0)
            {
              sprintf((char*)StPon2[iS],"%6.2f",PonA.ORL_AB[j]);      
              
              // �������� �����
              if ((PonA.ORL_AB[j]<UserMeasConfig.Lim_ORL[0])&&(SettingPrm.EnaCmpORL)) Inv[0][iS]=0x00; // ����� �������� ������
              // �������� ���� �� ������ �� ��������� BA? �������� ������
              if (PonA.ORL_BA[j]>-100.0)
              {
                sprintf((char*)StPon3[iS],"%6.2f",PonA.ORL_BA[j]);      
                // �������� �����
                if ((PonA.ORL_BA[j]<UserMeasConfig.Lim_ORL[0])&&(SettingPrm.EnaCmpORL)) Inv[1][iS]=0x00; // ����� �������� ������
              }
            }
            iS++;
            
          }
        }
        
        
        break;
      case LNM:
        // ������� �������� �����, 
        // ����� ����� ��������� - ����� 2, ��� ��������� (Sm/Mm)
        // 
        sprintf((char*)Str,"%05dm",PonA.LnA) ;// ���� �������� ��� ���� ������
        sprintf((char*)Stra,"%04d��",DeviceConfig.PlaceLS[1]) ;
        break;
      case BasV:
        break;
      }
      // ������������� ���������� ����
      SSD1305_Fill(0); /* ������� ������*/
      if(Mod_View != LNM)
      {
        // ����� ����
        SSD1305_GotoXY(12,0);
        SSD1305_PutsN((void*)StPon1[0], (void*)TabSG2, 1);// ������ ����� ������� ������
        SSD1305_GotoXY(12,8);
        SSD1305_PutsN((void*)StPon1[1], (void*)TabSG2, 1);// ������ ����� ������ ������
        SSD1305_GotoXY(12,16);
        SSD1305_PutsN((void*)StPon1[2], (void*)TabSG2, 1);// ������ ����� ������ ������
        SSD1305_GotoXY(12,24);
        SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);//  ������ ����� ����� ��������� 
        // LOSS AB
        SSD1305_GotoXY(35,0);
        SSD1305_PutsN((void*)StPon2[0], (void*)TabSG2, Inv[0][0]);// ������ ����� ������� ������
        SSD1305_GotoXY(35,8);
        SSD1305_PutsN((void*)StPon2[1], (void*)TabSG2, Inv[0][1]);// ������ ����� ������ ������
        SSD1305_GotoXY(35,16);
        SSD1305_PutsN((void*)StPon2[2], (void*)TabSG2, Inv[0][2]);// ������ ����� ������ ������
        // LOSS BA
        SSD1305_GotoXY(70,0);
        SSD1305_PutsN((void*)StPon3[0], (void*)TabSG2, Inv[1][0]);// ������ ����� ������� ������
        SSD1305_GotoXY(70,8);
        SSD1305_PutsN((void*)StPon3[1], (void*)TabSG2, Inv[1][1]);// ������ ����� ������ ������
        SSD1305_GotoXY(70,16);
        SSD1305_PutsN((void*)StPon3[2], (void*)TabSG2, Inv[1][2]);// ������ ����� ������ ������
        
      }
      else // Long measure
      {
        SSD1305_GotoXY(12,0);
        SSD1305_PutsN((void*)Str, (void*)TabSG3, 1);// ������� ����� �������� �����
        SSD1305_GotoXY(12,24);
        SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ����� �����
      }
      // ������� ��� ��������� ������
      SSD1305_GotoXY(87,24);
      SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);//  ������ ����� ����� ������
    }
    else
    {
      // ������� � ����������� �� ������� (����������� ��� ������)
      // 
      // ����������: ����� ��������� ���� ���������
      sprintf((char*)Stra,"%s",MsgAuto[0]) ; // 1 ������� ���������
      sprintf((char*)Strb,"%s",MsgAuto[1]) ; // 2 ������� ���������
      //sprintf((char*)Strd,"%s",MsgAuto[2]) ; // 3 ������� ��������� // ����, ���� ������
      sprintf((char*)Strd,"%04d��",LW_I) ; // ������ ����� �����, ������ �������
      sprintf((char*)Strc,"�%03d",SettingPrm.CountMemory) ; // ������ ������ ������
      
      
      SSD1305_Fill(0); /* ������� ������*/
      
      SSD1305_GotoXY(12,0);
      SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ������� ������
      SSD1305_GotoXY(12,8);
      SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ ����� ������ ������
      //SSD1305_GotoXY(12,16);
      //SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// ������ ����� ������ ������
      SSD1305_GotoXY(12,24);
      SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);//  ������ ����� ����� ����� 
      SSD1305_GotoXY(87,24);
      SSD1305_PutsN((void*)Strc, (void*)TabSG2, 1);//  ������ ����� ����� ������
      
      // ��������� ����������� ������
      sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
      SSD1305_GotoXY(pSR,0);
      SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
      // �������� ����� (���� ������)
      //sprintf((char*)Strm,"%d",Summx) ; // ��������� �������� �������� ���������� �����
      //SSD1305_GotoXY(0,16);
      //SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� �������� �����
    }
  }  
  
  // ���������� ��������� VFL (������� ����)
  void ShowVFL (void)
  {
    // ������� ������ ���������, ����� ���������
    // ����� ����� ���������, 
    // ����������: 
    
    sprintf((char*)Strc,"%s",StrVFLMode[ReMode]) ;
    sprintf((char*)Stra,"650��") ;
    sprintf((char*)Strb,"VFL") ;
    
    SSD1305_Fill(0); /* ������� ������*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// ������� ����� ����� ���������
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ����� �����
    SSD1305_GotoXY(92,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ ����� �������� ���������
    // ��������� ����������� ������
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    
  }
  // ���������� ��������� Ln (���������� �����)
  void ShowIzmLN (void)
  {
    // ������� �������� �����, 
    // ����� ����� ��������� - ����� 2, ��� ��������� (Sm/Mm)
    // 
    MeasLenght=(DataLN[1][IndxShowLN]>(uint16_t)DeviceConfig.MZone)?((uint16_t)((DataLN[1][IndxShowLN]-(uint16_t)DeviceConfig.MZone)*DeviceConfig.KGen)):(0);
    //sprintf((char*)Str,"%05dm",MeasLenght) ;// ���� �������� ��� ���� ������
    //sprintf((char*)Str,"%05dm",MemoryData.mLen) ;// ���� �������� ��� ���� ������
    //sprintf((char*)Str,"%05dm",DataLN[0][0]) ;// ���� �������� ��� ���� ������
    sprintf((char*)Strc,"%05dm",MeasLenght) ;// ���� �������� ��� ���� ������
    sprintf((char*)Stra,"%04d��",DeviceConfig.PlaceLS[1]) ;
    sprintf((char*)Strb,"%s",(DeviceConfig.PlaceLS[1]>1300)?("S"):("M")) ;
    sprintf((char*)Strm,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    if(IndxShowLN==0)
      sprintf((char*)Strd,"%s",(DataLN[1][1] !=0)?("!>"):(" ")) ; // ��������� �������� ������� ��� ������
    else
      sprintf((char*)Strd,"%d",IndxShowLN) ; // ��������� �������� ������� ��� ������

    SSD1305_Fill(0); /* ������� ������*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// ������� ����� �������� �����
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ����� �����
    SSD1305_GotoXY(64,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ ����� ��� ��������� SM/MM
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    SSD1305_GotoXY(100,24);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// ������ ����� ����� ���������
    // �������� ����� ������ ���������
//    for(int i=0;i<4;i++)
//    {
//    sprintf((char*)Str,"%06d",DataLN[i][0]) ;// ���� �������� ��� ���� ������
//        SSD1305_GotoXY(12,i*8);
//    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ ����� ����� �����
//  
//    }
  }
  
  // ���������� ��������� Rl (�������� ���������)
  void ShowORL (void)
  {
    // ������� �������� ORL, 
    // ����� ����� ���������, ��� ��������� (Sm/Mm), �������� ���������
    // ����������: Ind_LW - ������ �������� �������� �����
    
    //  sprintf((char*)Str,"%d",IN_BuffADC[1]) ; // test
    sprintf((char*)Strc,"%.1f dB",ORLCalc) ;
    sprintf((char*)Stra,"%04d��",DeviceConfig.PlaceLS[Ind_LW]) ;
    //sprintf((char*)Strb,"%.2f",SettingPrm.PowSc[Ind_LW]) ;// ��������� �������� ��������� 
    // ����� ����������� ��������� ������� �������� ��� ORL? 16/02/2023
    sprintf((char*)Strb,"%.2f",BaseRetCurrORL[indx_LW_ORL]) ;// ��������� �������� ��������� 
    sprintf((char*)Strm,"%s",(DeviceConfig.PlaceLS[Ind_LW]>1300)?("S"):("M")) ;
    
    SSD1305_Fill(0); /* ������� ������*/
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Strc, (void*)TabSG3, 1);// ������� ����� ��������� ORL
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ ����� ����� �����
    SSD1305_GotoXY(90,24);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ ����� �������� ���������
    SSD1305_GotoXY(64,24);
    SSD1305_PutsN((void*)Strm, (void*)TabSG2, 1);// ������ ����� ��� ��������� SM/MM
    // ��������� ����������� ������
    sprintf((char*)Strd,"%s",(lSofn)?("*"):(" ")) ; // ��������� ��������� ������
    SSD1305_GotoXY(pSR,0);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
    // ��������� ������������� �������� "�����" (�� -40 �� ���)
    if( CoeffPM.Pow_mW_Clbr != PowClbr_mW)
    {
    sprintf((char*)Stri,"R") ; // ������� ���������
    SSD1305_GotoXY(pSR,16); // ������ ������
    SSD1305_PutsN((void*)Stri, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
      
    }
    // ��������� ������������� �������� "������" (�� -10... -18 �� ���)
    if( BaseRetCurrORL[indx_LW_ORL] != CoeffPM.BaseRet[indx_LW_ORL])
    {
    sprintf((char*)Stri,"�") ; // ������� ���������
    SSD1305_GotoXY(pSR,8); // ������ ������
    SSD1305_PutsN((void*)Stri, (void*)TabSG2, 1);// ������ ����� ��������� ��������� ������
      
    }
  }
  // ��������� ������ ������ ������ (FunSetup)
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

    SSD1305_Fill(0); /* ������� ������*/
    // ������� �����
    //Sec2Clock(GetSysTime(&hrtc));// �������� ������� ����� (��������� ���������) �� ��������� ���������� RTC lkz 103
    // ��� 303 ���� ���������� ��������� �� RTC � �������� Clocks
    ReSetRegRTC(0, &hrtc); // for F303
    
    sprintf((char*)Strd,"20%02d/%02d/%02d %02d:%02d:%02d",Clocks.cYear,Clocks.cMonth,Clocks.cDay,Clocks.cHour,Clocks.cMinutes,Clocks.cSeconds) ;
    
    SSD1305_GotoXY(12,0);
    SSD1305_PutsN((void*)Stra, (void*)TabSG2, 1);// ������ �����
    SSD1305_GotoXY(12,10);
    SSD1305_PutsN((void*)Strb, (void*)TabSG2, 1);// ������ �����
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Strd, (void*)TabSG2, 1);// ������ �����
    
  }
  
  /* ��������� ���������� ORL � ������� �������*/
  void ShowORLAuto(void)
  { 
    uint8_t lD[10], limLS[10], lORL[10];
    float LW, LS, ORL;
    SSD1305_Fill(0); /* ������� ������*/
    // ������� ������ ������ ( 3 ������ ��������), ����� �������� ������ �� 3-� ���������������
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
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ �����
    
    LW = 1.49; 
    LS = -99.99;
    ORL = 12.34;
    sprintf((char*)lD,"%.2f", LW) ;
    sprintf((char*)limLS,"%.2f", LS) ;
    sprintf((char*)lORL,"%.2f", ORL) ;
    sprintf((char*)Str,"%s %s %s", lD, limLS, lORL) ;
    SSD1305_GotoXY(12,8);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ �����
    
    LW = 1.55; 
    LS = -56.78;
    ORL = 90.12;
    sprintf((char*)lD,"%.2f", LW) ;
    sprintf((char*)limLS,"%.2f", LS) ;
    sprintf((char*)lORL,"%.2f", ORL) ;
    sprintf((char*)Str,"%s %s %s", lD, limLS, lORL) ;
    SSD1305_GotoXY(12,16);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ �����
    sprintf((char*)Str,"Lim: LS/ORL") ;
    SSD1305_GotoXY(12,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ �����
    sprintf((char*)Str,"�%d",439) ;
    SSD1305_GotoXY(80,24);
    SSD1305_PutsN((void*)Str, (void*)TabSG2, 1);// ������ �����
    
    //  SSD1305_DrawRectangle(12,0,104,32,1);
    
  }
  
  // draw icon mode device - & level battery
  
  void DrawMod (uint8_t Mode)
  {
    uint8_t ModeIndDraw=1;//1
    // ����� �������������� ��������� � ����������� �� ������� �� ����������
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
    // ���� ���� ���� �������� ������
    //if(UserMeasConfig.AutoOff)
    //{
    // SSD1305_DrawCircle(108, 15, 7 ,1);
    //}
    // ����� ��� ����������, ��� ����� ���� ������ ������
    SSD1305_DrawLine(102, 8, 102 ,22, 1);// ������������ ������
    SSD1305_DrawLine(114, 8, 114 ,22, 1);// ������������ �����
    SSD1305_DrawLine(102, 22, 114 ,22, 1);// �������� ������
    SSD1305_DrawLine(102, 23, 114 ,23, 0);// �������� ������ ������
  }
  
  void ShowBatLvl (uint8_t ProcBat)
  {
    sprintf((char*)StrBat,"%c", 'a' + (uint8_t)(ProcBat/8.5)) ;
    SSD1305_GotoXY(102,0);
    SSD1305_PutsN((void*)StrBat, (void*)TabSG5, 1);
    // ���� ������ �����
    //  SSD1305_DrawRectangle(0,0,128,32,1);
   
  }
 
// ����� �� MAIN ��� ������, ��� �� �� ������� �����
      // ���������� ������� ���� ����������
  void DrawMainModes (void)
  {
    switch(CondWork)
    {
    
    case P1: // ����������� ������ ~0.5���
      if(NeedReDraw) ShowPM(ModeI, PMWavelenght);
    break;
    case P1a:  // ����������� ����� ����� ����������
      if(NeedReDraw) ShowP1a(P1aWavelenght); /* ��������� ���������� P1a ����������*/
      CntLCD_P1 = 0;// �������������� ����� �� ����!
    break;
    case SetFn:
      if(NeedReDraw) ShowChooseMode(); /* ��������z ������ ������, ��� ��� ��� ����, �� ������ ��� ����� (0.48 �)*/
      
    break;
    case Sc:
      if(NeedReDraw) ShowSource(); /* ��������z ��������� Sc ����������*/
      CntLCD_P1 = 0;// �������������� ����� �� ����!
    break;
    case RE:
      if(NeedReDraw) ShowVFL(); /* ��������z ��������� VFL ����������*/
      CntLCD_P1 = 0;// �������������� ����� �� ����!
    break;
    case Ln:  //
      if(NeedReDraw) ShowIzmLN(); /* ��������� Ln ����������*/
      CntLCD_P1 = 0;// �������������� ����� �� ����!
    break;
    case Rl:  // ����������� ������ ~0.5���
      if(NeedReDraw) ShowORL(); /* ��������� ORL ����������*/
    break;
    case P2:  // ����������� ������ ~0.5���
      if(NeedReDraw) ShowP2(MediumP2); /* ��������� P2 ����������*/
    break;
    case At:  //����� �������
      Run_At ();

      if(NeedReDraw) ShowAUTO(PMWavelenght); /* ��������� ������ ������� ����������*/
      CntLCD_P1 = 0;// �������������� ����� �� ����!
    break;
    case SetPrm:  // 
      if(NeedReDraw)
      {
        switch(SetupMod) //sTime, sNumF, sLOSS, sORL, sSCp, sMem, sSWm
        {
        case sTime:
        ShowCurrTime(); /* ��������� ������� ����������*/
        break;
        case sAutoOff:
        ShowSetAutoOff(); /* ����� ��������� ��������������*/
        break;
        case sNumF:
        ShowNumFbr(); /* ��������� ��������� ������ �������*/
        break;
        case sLOSS:
        ShowLimLOSS(); /* ����� ��������� ������� LOSS*/
        break;
        case sLOSSm:
        ShowLimLOSSm(); /* ����� ��������� ������� LOSS MM 850 1300*/
        break;
        case sMem:
        ShowMemory(); /* ����� ������ � �������*/
        break;
        case sORL:
        ShowLimORL(); /* ����� ��������� ������� ORL*/
        break;
        case sSWm:
        ShowEnaSave(); /* ����� ��������� �������������� ���������� ���������� LOSS, ORL, Ln*/
        break;
        case sVer:
        ShowVer(); /* ����� ��������� ����������� �������������� (*/
        break;
        case sSCp:
        ShowPowSC(); /* ����� ��������� ������� �������*/
        break;

        }
      }
      CntLCD_P1 = 0;// �������������� ����� �� ����!
    break;
            //

    default:
      break;
    } //switch CondWork
    // ����� ���������� � ������ � ���� � �� �� �����!!!
    ShowMsgWrMem();
    
  }
// �������� ������ ��������� , �������� �� ��� MAIN

  void CheckUbat(void)
  {
          // ����� ��� ����� �������� ���, ����� � ���� ��� �� �������
      // ��������� 
      
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
      if(ABS(Ubat-OldUbat)>0.05) // ����� �������������� ��������� ���������
      {
        OldUbat = Ubat;
        ShowBatLvl(BatLevel);
        NeedReDraw = 1;
        
      }

  }
#ifdef __cplusplus
}
#endif
