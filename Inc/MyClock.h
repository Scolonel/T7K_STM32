/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MyClock_H
#define __MyClock_H

#ifdef __cplusplus
extern "C" {
#endif
  
  void Sec2Clock( uint32_t TimeSec); // ������� ������ � ��������� �����
    
  uint32_t TotalSec( void); // ������� �������� ������� � �������
  
  // ��������� ��������� ����� �� ���������� ��������
  void SetClockFromSys(uint32_t SysT);
//  uint32_t GetSysTime(RTC_HandleTypeDef* hrtc);
  // ������ � �������� ����� � �������� �� 1 ������ 1970
  uint32_t ReSetRegRTC (int Dir, RTC_HandleTypeDef* hrtc); // Dir =0 ����� �� RTC to Clocks

  void SetSysTime (uint32_t TimeCounter, RTC_HandleTypeDef* hrtc);
  
  
  
#ifdef __cplusplus
}
#endif

#endif /* __MyClock_H */
