/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Automat_H
#define __Automat_H

#ifdef __cplusplus
 extern "C" {
#endif

   
void Run_At (void);
void CheckTimerAT (void);// �������� ������� � ������ ������� �� �������� (��������� �������� � ��������� ���������� �������� �� 
                         // �������� Start_Cmd, ��������� 
// ������ ����� �������� �������
void GetNumActivLS(void);
// ������������� ���������� �� ������
void Ini_At_var(void); 


extern uint8_t FirstInput; // ���������� ������� ����� �� ������� � ������ ��������� ( ��� �������� ������������� ���������)
extern int Mod_At_cnt; // ������� ��������������� ������ ����� ��� ������

#ifdef __cplusplus
}
#endif
#endif /*__ Automat_H */