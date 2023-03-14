/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DrawFunctions_H
#define __DrawFunctions_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
   
/* Define ---------------------------------------------------------*/
    // draw icon mode device - & level battery
void DrawMod (uint8_t Mode);
void ShowBatLvl (uint8_t ProcBat);
   


void ShowWelcome(void);
/* */
void ShowPM(uint8_t ModInd, uint16_t LW_I);
/* ��������� ���������� ORL � ������� �������*/
void ShowORLAuto(void);
// ��������� ������ ������ ������ (FunSetup)
void ShowChooseMode(void);
/* ��������� ���������� P1a ����������*/
void ShowP1a(uint16_t LW_I);
// ���������� ��������� S
void ShowSource (void);
// ���������� ��������� VFL (������� ����)
void ShowVFL (void);
// ���������� ��������� Ln (���������� �����)
void ShowIzmLN (void);
// ���������� ��������� Rl (�������� ���������)
void ShowORL (void);
// ���������� ���������� �������� P2 ()
void ShowP2 (float MedADC);
// ���������� ������ �������
void ShowAUTO ( uint16_t LW_I);
/* ����� ��������� �������, */
void ShowCurrTime(void);
/* ��������� ��������� ������ �������*/ 
void ShowNumFbr(void); 
/* ����� ��������� ������� ORL*/
void ShowLimORL(void);
/* ����� ��������� ������� LOSS*/
void ShowLimLOSS(void);
/* ����� ��������� ������� LOSS ����� ���� 850, 1300*/
void ShowLimLOSSm(void);

/* ����� ��������� �������������� ���������� ���������� LOSS, ORL, Ln*/
void ShowEnaSave(void);
/* ����� ������ � �������*/
void ShowMemory(void);
/* ����� ��������� ������� �������*/
void ShowPowSC(void);
// ����� ���������� � ������ � ���� � �� �� �����!!!
void ShowMsgWrMem(void);
//
void ShowVer(void); /* ����� ��������� ����������� �������������� (*/
      // ���������� ������� ���� ����������
  void DrawMainModes (void);
      // ���������� ������� ���� ����������
  void CheckUbat (void);


#ifdef __cplusplus
}
#endif
#endif /*__ DrawFunctions_H */
