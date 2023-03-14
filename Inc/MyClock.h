/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MyClock_H
#define __MyClock_H

#ifdef __cplusplus
extern "C" {
#endif
  
  void Sec2Clock( uint32_t TimeSec); // перевод секунд в структуры часов
    
  uint32_t TotalSec( void); // перевод текущего времени в секунды
  
  // Установка структуры часов по системному счетчику
  void SetClockFromSys(uint32_t SysT);
//  uint32_t GetSysTime(RTC_HandleTypeDef* hrtc);
  // запись в регистры время в секундах от 1 января 1970
  uint32_t ReSetRegRTC (int Dir, RTC_HandleTypeDef* hrtc); // Dir =0 пишем из RTC to Clocks

  void SetSysTime (uint32_t TimeCounter, RTC_HandleTypeDef* hrtc);
  
  
  
#ifdef __cplusplus
}
#endif

#endif /* __MyClock_H */
