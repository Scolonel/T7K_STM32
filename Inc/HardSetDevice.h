/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HardSetDevice_H
#define __HardSetDevice_H


#ifdef __cplusplus
 extern "C" {
#endif 

#define K1 0x80
#define K2 0x20
#define K3 0x08
// source
#define SW_L  0x04
#define SW_H  0x40
#define ON_GEN(a) (a)?(CtrlExpand(1, 1, 1)):(CtrlExpand(0, 1, 1)) // ���������� �������� OnG
#define LD_R(a) (a)?(CtrlExpand(0x100, 0x80, 1)):(CtrlExpand(0x100, 0x80, 0)) // ���������� ������� ������? ����� ������ ��� ������� ������ �������� �����
#define CW_S(a) (a)?(CtrlExpand(0x02, 0x02, 1)):(CtrlExpand(0, 0x02, 1)) // ���������� ���������� ���������� �������
// Ext mem 24lc512
#define EEPROM24LC512 0x50   // ������� ����� EEPROM24LC512

extern uint8_t TimeSCWP; // ������� ������� ��� ������������ �������

   
uint32_t InvalidDevice(); //boolean actually
void InitDevice(unsigned Err);
   
void SetRange (uint8_t Rng); // ��������� �������� ������������ ������ ��� ���������� ��������� Range = 0 - ����� ������, Range = 3 - ����� ��������������
// ����� � ��������� ������� ����� ����� ���������� �� ����� �����
int GetIndxLW (uint16_t in_LW);

void SWITCH_LW(void); // ������� ��������� �������� ��� ������ ��������� �����, 
// ���������� ������� ������
void ControlRE (uint8_t Mode); // 
// ������� ������������ ������� �� ����� � ���������� ���������� � ����
void Run_SCWP(void);
// �����/��������� ������� ����� ����� ��������� ��� �������, � ��������� �� ���� ����
uint8_t GetSetLW_SC (uint8_t Next, uint8_t Choice); // Next 1-���������,0- ������ �� �������������;
                                                     //Choice 1-� ��������� �� ����������, 0-��� ��������

// ���������� �������� ���������� (�������� CW � FW)
void ControlSC (uint8_t Mode); // 

uint32_t FindErrCoeff (void); // �������� ������� �������������
void FixErrCoeff (uint32_t Err); // ����������� ������� �������������
// �������� ��������� ������������ ��������� �������
uint32_t FindErrUserMC (void);
void FixErrUserMC (uint32_t Err); //  ���������  (���������� ���������)
    // ������� �������� ����������� ��������� P2 � ���������� �����
uint32_t   FindErrSettingPrm (void);
void  FixErrSettingPrm (uint32_t Err); //  ���������  (���������� ���������)
// ������� �������� ���� �������������
uint32_t FindErrSpec (void);
void FixErrSpec (uint32_t Err); //  ���������  (���������� ������������)

void SW_RS232 (int a);
#ifdef __cplusplus
}
#endif

#endif /* HardSetDevice_H */
   