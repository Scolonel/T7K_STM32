// Здесь напишем разные функции заполнения строк индикатора

#include "main.h"
//#include "stm32f3xx_hal_rtc.h"
//#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
//#include "stm32f1xx_hal_rtc.h"
//
//#endif
#ifdef __cplusplus
 extern "C" {
#endif

   
  #define YearSecV  31622400
  #define YearSec   31536000
  #define DaySec    86400
  #define Y2021F15  1613347200 // начальное время установки релиза 15.02.2021 00:00:00
  #define Y2021J01  1609459200 // начальное время установки релиза 01.01.2021 00:00:00
   
   CLOCK_DEV Clocks;
uint8_t IndxEditClock=0; // индекс указателя на редактор элемента

void Sec2Clock( uint32_t TimeSec) // перевод секунд в структуры Clocks
{

  unsigned int _Days[]={31,28,31,30,31,30,31,31,30,31,30,31};
  
  int Year = 21;
  int Month;
  int Day;
  
  int Hour;
  int Minute;
  int Second;
  
  int tmp;
  
  TimeSec = TimeSec - Y2021J01; // вычитаем секунды до 01 january 2021 года
  //TimeSec = TimeSec - DaySec; // вычитаем секунды высокосного дня 2000 года
  //if (TimeSec>0x80000000) TimeSec = 10*YearSecV-1; // зачем это не понятно???
  tmp = YearSec;
  while(TimeSec > tmp)
  {
    TimeSec = TimeSec - tmp;
    Year++;
    if(Year%4) tmp = YearSec;
    else tmp = YearSecV;
  }
  // получили год 
  Clocks.cYear = Year;
  Month = 1;

  while(TimeSec>_Days[Month-1]*DaySec)
  {
    TimeSec -= _Days[Month-1]*DaySec;
    Month++;
  }
  if((Month>1)&&(!(Year%4))) TimeSec -=DaySec;
  // Mounth
  Clocks.cMonth = Month;
  
  Day = 1;
  
  while(TimeSec>DaySec)
  {
    TimeSec -= DaySec;
    Day++;    
  }
  // Day 
  Clocks.cDay = Day;
  
  Hour = TimeSec/3600;
  Clocks.cHour = Hour;

  TimeSec -= Hour*3600;
  
  Minute = TimeSec/60;
  Clocks.cMinutes = Minute;
  
  TimeSec -= Minute*60;
  
  Second = TimeSec;
  Clocks.cSeconds = Second;
  

  return;
}


uint32_t TotalSec( void) // перевод текущего времени в секунды
{
  uint32_t secs;

  int Year;
  unsigned int Month;
  unsigned int Date, days;
  unsigned int Hour;
  unsigned int Minut;
  unsigned int Sec;
  unsigned int _Days[]={31,28,31,30,31,30,31,31,30,31,30,31};
  days = 0;
  secs = 0;
  Sec = Clocks.cSeconds; 
  Minut = Clocks.cMinutes; 
  Hour = Clocks.cHour; 
  Date = Clocks.cDay; 
  Month = Clocks.cMonth; 
 if ((Month > 12) || (Month == 0)) Month =2; 
 Year = Clocks.cYear%100 ;//+ 14; 
 	if((!((Year) % 4))&&(Month>2)) days++; // добавляем день высокосного года
        
	if (Year > 43) Year = 1;

	Year--;
        
	while(Year>=21)
	{
		if(!((Year)%4))days+=366;
		else days+=365;
		Year--;
	}
	while(Month-1>0)
		days+=_Days[(--Month)-1];

	days+=Date-1;
	
	secs=days*24*3600+Hour*3600+Minut*60+Sec;
        //secs = secs + Y2021F15; // 15 февраля 2021 (
        secs = secs + Y2021J01; // 01 января 2021 (

    return(secs);
}

// Установить системное время 
// запись в регистры время в секундах от 1 января 1970
void SetSysTime (uint32_t TimeCounter, RTC_HandleTypeDef* hrtc)
{
  //RTC_WriteTimeCounter(hrtc,TimeCounter);
  // WRITE_REG(hrtc->Instance->CNTH, (uint16_t)((TimeCounter>>16)&0xffff));

  // WRITE_REG(hrtc->Instance->CNTH, (uint16_t)((TimeCounter>>16)&0xffff));
  // WRITE_REG(hrtc->Instance->CNTL, (uint16_t)(TimeCounter&0xffff));
}

// перезапись регистров часов и переменной структуры внутренних часов, для процессоров
// 303 - у них другя структура RTC - есть регистры времени, надо перезаписать туда , или в обратную сторну занчения
// установок в структуру Clocks 
uint32_t ReSetRegRTC (int Dir, RTC_HandleTypeDef* hrtc) // Dir =0 пишем из RTC to Clocks
{
  
  uint32_t timecounter = 0U;
#if defined(STM32F302xE) || defined(STM32F303xE) || defined(STM32F398xx) || \
    defined(STM32F302xC) || defined(STM32F303xC) || defined(STM32F358xx) || \
    defined(STM32F303x8) || defined(STM32F334x8) || defined(STM32F328xx) || \
    defined(STM32F301x8) || defined(STM32F302x8) || defined(STM32F318xx) || \
    defined(STM32F205xx)  
      
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  
  if(Dir)
  {
    sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
    sDate.Month = Clocks.cMonth;
    sDate.Date = Clocks.cDay;
    sDate.Year = Clocks.cYear;
    HAL_RTC_SetDate(hrtc, &sDate, RTC_FORMAT_BIN);
    sTime.Hours = Clocks.cHour;
    sTime.Minutes = Clocks.cMinutes;
    sTime.Seconds = Clocks.cSeconds;
    HAL_RTC_SetTime(hrtc, &sTime, RTC_FORMAT_BIN); //for F303
    
  }
  else  // перепишем из регистров RTC в регистры Clocks
  {
    // получим текущее время
    HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(hrtc, &sDate, RTC_FORMAT_BIN);
    // перепишем регистры
    Clocks.cYear = sDate.Year;
    Clocks.cMonth = sDate.Month;
    Clocks.cDay = sDate.Date; 
    Clocks.cHour = sTime.Hours;
    Clocks.cMinutes = sTime.Minutes;
    Clocks.cSeconds = sTime.Seconds;
  }
#endif
  // for F103 
#if defined (STM32F103x6) || defined (STM32F103xB) || defined (STM32F105xC) || defined (STM32F107xC) || defined (STM32F103xE) || defined (STM32F103xG)
 uint16_t high1 = 0U, high2 = 0U, low = 0U;
  
  if(Dir) // пишем в регистры секунд, текущее или установочное время
 {
           // фиксируем установленное время из структуры Clocks 
        RTC_WriteTimeCounter(hrtc,TotalSec());

 }
 else // читам из регистров секунд, текущее время
 {
  high1 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);
  low   = READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT);
  high2 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);

  if (high1 != high2)
  { /* In this case the counter roll over during reading of CNTL and CNTH registers, 
       read again CNTL register then return the counter value */
    timecounter = (((uint32_t) high2 << 16U) | READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT));
  }
  else
  { /* No counter roll over during reading of CNTL and CNTH registers, counter 
       value is equal to first value of CNTL and CNTH */
    timecounter = (((uint32_t) high1 << 16U) | low);
  }
   Sec2Clock(timecounter); // перевод секунд в структуры Clocks

 }
#endif
  // посчитаем секунды для текущего или вновь установленного времени
  timecounter = TotalSec();// перевод из структуры Clocks  в секунды
  return timecounter; 
  // сюда же надо вставить перевод в струтуру Clock для F103
}

//получить системное время
//uint32_t GetSysTime(RTC_HandleTypeDef* hrtc)
//{
//  uint16_t high1 = 0U, high2 = 0U, low = 0U;
//  uint32_t timecounter = 0U;
//
//  high1 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);
//  low   = READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT);
//  high2 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);
//
//  if (high1 != high2)
//  { /* In this case the counter roll over during reading of CNTL and CNTH registers, 
//       read again CNTL register then return the counter value */
//    timecounter = (((uint32_t) high2 << 16U) | READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT));
//  }
//  else
//  { /* No counter roll over during reading of CNTL and CNTH registers, counter 
//       value is equal to first value of CNTL and CNTH */
//    timecounter = (((uint32_t) high1 << 16U) | low);
//  }
//
//  return timecounter;
//}

// Установка структуры часов по системному счетчику
void SetClockFromSys(uint32_t SysT)
{
  
}


#ifdef __cplusplus
}
#endif
