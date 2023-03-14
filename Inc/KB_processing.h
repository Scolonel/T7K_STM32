/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KB_Processing_H
#define __KB_Processing_H


#ifdef __cplusplus
 extern "C" {
#endif 

void KeyBoardAnsver( uint8_t CodeKeys);
uint16_t WriteBlockMEM (uint8_t Dir); // запись блока данных в память, отделной функцией для UART EXT
uint8_t SetNeedCondWork (uint8_t SetConditionWork);   
   
#ifdef __cplusplus
}
#endif

#endif /* KB_Processing_H */
   