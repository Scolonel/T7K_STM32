/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Automat_H
#define __Automat_H

#ifdef __cplusplus
 extern "C" {
#endif

   
void Run_At (void);
void CheckTimerAT (void);// контроль таймера в режиме АВТОМАТ по командам (Инкремент счетчика и установка разрешения передачи по 
                         // значению Start_Cmd, установка 
// Запись числа активных лазеров
void GetNumActivLS(void);
// инициализация переменных Ат режима
void Ini_At_var(void); 


extern uint8_t FirstInput; // переменная первого входа по кнопкам в режиме автоматов ( для успешной инициализации подрежима)
extern int Mod_At_cnt; // Счетчик автоматического режима когда что делать

#ifdef __cplusplus
}
#endif
#endif /*__ Automat_H */