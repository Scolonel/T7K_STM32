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
/* индикация результата ORL в ТЕСТЕРЕ АВТОМАТ*/
void ShowORLAuto(void);
// индикация выбора режима Работы (FunSetup)
void ShowChooseMode(void);
/* индикация измерителя P1a упрощенная*/
void ShowP1a(uint16_t LW_I);
// прорисовка источника S
void ShowSource (void);
// прорисовка источника VFL (красный глаз)
void ShowVFL (void);
// прорисовка источника Ln (измеритель длины)
void ShowIzmLN (void);
// прорисовка источника Rl (обратное отражение)
void ShowORL (void);
// прорисовка индикатора мощности P2 ()
void ShowP2 (float MedADC);
// прорисовка ТЕСТЕР АВТОМАТ
void ShowAUTO ( uint16_t LW_I);
/* Экран установки времени, */
void ShowCurrTime(void);
/* индикация установки номера волокна*/ 
void ShowNumFbr(void); 
/* Экран установки порогов ORL*/
void ShowLimORL(void);
/* Экран установки порогов LOSS*/
void ShowLimLOSS(void);
/* Экран установки порогов LOSS длины волн 850, 1300*/
void ShowLimLOSSm(void);

/* Экран установки переключачелей разрешения сохранения LOSS, ORL, Ln*/
void ShowEnaSave(void);
/* Экран работы с памятью*/
void ShowMemory(void);
/* Экран установки уровней Лазеров*/
void ShowPowSC(void);
// вывод сообшщения о записи в одно и то же место!!!
void ShowMsgWrMem(void);
//
void ShowVer(void); /* Экран установки отображения идентификатора (*/
      // прорисовки режимов если необходимо
  void DrawMainModes (void);
      // прорисовки режимов если необходимо
  void CheckUbat (void);


#ifdef __cplusplus
}
#endif
#endif /*__ DrawFunctions_H */
